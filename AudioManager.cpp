#include "AudioManager.h"

#include "Debug.h"

AudioManager* AudioManager::_instance = nullptr;
unsigned int AudioManager::nextHandle = 1;

AudioInstance* AudioManager::FindInstance(unsigned int handleID)
{
    if (activeInstances.find(handleID) == activeInstances.end())
    {
        return nullptr;
    }
    return &activeInstances[handleID];
}

void AudioManager::DestroyInstance(AudioInstance* instance)
{
    if (instance->voice)
    {
        instance->voice->DestroyVoice();
        instance->voice = nullptr;
    }
}

enum
{
    fourccRIFF = 'FFIR',
    fourccDATA = 'atad',
    fourccFMT = ' tmf',
    fourccWAVE = 'EVAW',
    fourccXWMA = 'AMWX',
    fourccDPDS = 'sdpd'

};


HRESULT AudioManager::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT AudioManager::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}


bool AudioManager::Init()
{

    DebugOut(L"[INFO] Initializing XAudio2\n");
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] CoInitializeEx failed\n");
        return false;
    }

    hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] XAudio2Create failed\n");
        xAudio2 = nullptr;
        return false;
    }

    hr = xAudio2->CreateMasteringVoice(&masteringVoice);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] CreateMasteringVoice failed\n");

        xAudio2->Release();
        xAudio2 = nullptr;
        return false;
    }

    DebugOut(L"[INFO] XAudio2 initialized successfully\n");
    return true;
}

void AudioManager::Shutdown()
{
    StopAll();

    for (auto& it : activeInstances)
    {
        auto& instance = it.second;
        DestroyInstance(&instance);
    }
    activeInstances.clear();

    soundData.clear();

    if (masteringVoice)
    {
        masteringVoice->DestroyVoice();
        masteringVoice = nullptr;
    }

    if (xAudio2)
    {
        xAudio2->Release();
        xAudio2 = nullptr;
    }

    CoUninitialize();

    musicHandle = 0;
    nextHandle = 1;
}

void AudioManager::Update()
{

    for (auto& it : activeInstances)
    {
        auto& instance = it.second;

        if (instance.markedForDelete || instance.voice == nullptr)
            continue;

        if (instance.paused)
            continue;

        XAUDIO2_VOICE_STATE state{};
        instance.voice->GetState(&state);

        if (!instance.looping && state.BuffersQueued == 0)
        {
            if (instance.onEnd)
                instance.onEnd();

            instance.markedForDelete = true;

            if (instance.handle == musicHandle)
                musicHandle = -1;
        }
    }

    for (auto& it : activeInstances)
    {
        auto& v = it.second;

        if (v.markedForDelete)
            DestroyInstance(&v);
    }
    for (auto it = activeInstances.begin(); it != activeInstances.end(); )
    {
        if (it->second.markedForDelete)
        {
            if (it->second.voice)
            {
                it->second.voice->DestroyVoice();
                it->second.voice = nullptr;
            }

            it = activeInstances.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool AudioManager::LoadWAV(int id, LPCWSTR filePath)
{
    if (GetIdForWAVFile(filePath).hasValue)
        return false;


    WAVEFORMATEXTENSIBLE wfx = {};
    XAUDIO2_BUFFER buffer = {};

    const HANDLE hFile = CreateFile(
        filePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        DebugOut(L"[Error]");
        return false;

    }

    if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        CloseHandle(hFile);
        DebugOut(L"[Error]");
        return false;
    }

    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
    {
        DebugOut(L"[Error]");
        return false;
    }

    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);


    //fill out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    vector<BYTE> bytes(dwChunkSize);
    ReadChunkData(hFile, bytes.data(), dwChunkSize, dwChunkPosition);
    CloseHandle(hFile);
	

    SoundData data;
    data.id = id;
    data.format = wfx;
    data.audioBytes = std::move(bytes);
    soundData[id] = std::move(data);
    DebugOut(L"[INFO] Loaded WAV id=%d\n", id);
    filePathToID[filePath] = id;

    return true;
}

unsigned int AudioManager::Play(int soundId, bool looping, std::function<void()> onEnd)
{

	auto it = soundData.find(soundId);
	if (it == soundData.end())
	{
		DebugOut(L"[ERROR] Sound id %d not loaded\n", soundId);
		return -1;
	}
	SoundData& data = it->second;

    IXAudio2SourceVoice* sourceVoice = nullptr;
    HRESULT hr = xAudio2->CreateSourceVoice(
        &sourceVoice,
        reinterpret_cast<WAVEFORMATEX*>(&data.format)
    );

    if (FAILED(hr) || sourceVoice == nullptr)
    {
        DebugOut(L"[ERROR] Cannot create source voice for sound id %d\n", soundId);
        return -1;
    }

    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = static_cast<UINT32>(data.audioBytes.size());
    buffer.pAudioData = data.audioBytes.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopBegin = 0;
    buffer.LoopLength = 0;
    buffer.LoopCount = looping ? XAUDIO2_LOOP_INFINITE : 0;
    buffer.pContext = nullptr;

    hr = sourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] SubmitSourceBuffer failed for sound id %d\n", soundId);
        sourceVoice->DestroyVoice();
        return -1;
    }

    hr = sourceVoice->Start(0);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] Start failed for sound id %d\n", soundId);
        sourceVoice->DestroyVoice();
        return -1;
    }

    AudioInstance instance;
    instance.handle = nextHandle++;
    instance.soundId = soundId;
    instance.voice = sourceVoice;
    instance.paused = false;
    instance.looping = looping;
    instance.markedForDelete = false;
    instance.onEnd = std::move(onEnd);

    auto handle = instance.handle;
    activeInstances[handle] = std::move(instance);
    return handle;
}

unsigned int AudioManager::PlaySFX(int soundId)
{
    return Play(soundId, false, nullptr);
}

unsigned int AudioManager::PlayMusic(int soundId, bool looping, std::function<void()> onEnd)
{
    StopMusic();
    musicHandle = Play(soundId, looping, std::move(onEnd));
    return musicHandle;
}

void AudioManager::Stop(unsigned int playbackHandle)
{
    AudioInstance* instance = FindInstance(playbackHandle);
    if (instance == nullptr || instance->voice == nullptr)
        return;

    instance->voice->Stop(0);
    instance->voice->FlushSourceBuffers();
    instance->markedForDelete = true;

    if (playbackHandle == musicHandle)
        musicHandle = -1;
}

void AudioManager::Pause(unsigned int playbackHandle)
{
    AudioInstance* instance = FindInstance(playbackHandle);
    if (instance == nullptr || instance->voice == nullptr || instance->paused)
        return;

    instance->voice->Stop(0);
    instance->paused = true;
}

void AudioManager::Resume(unsigned int playbackHandle)
{
    AudioInstance* instance = FindInstance(playbackHandle);
    if (instance == nullptr || instance->voice == nullptr || !instance->paused)
        return;

    instance->voice->Start(0);
    instance->paused = false;
}

void AudioManager::StopMusic()
{
    if (musicHandle != 0)
    {
        Stop(musicHandle);
        musicHandle = -1;
    }
}

void AudioManager::PauseMusic()
{
    if (musicHandle != 0)
        Pause(musicHandle);
}

void AudioManager::ResumeMusic()
{
	Resume(musicHandle);
}

void AudioManager::StopAll()
{
    for (auto& it : activeInstances)
    {
        auto& instance = it.second;
        if (instance.voice)
        {
            instance.voice->Stop(0);
            instance.voice->FlushSourceBuffers();
        }
        instance.markedForDelete = true;
    }

    musicHandle = -1;
}
