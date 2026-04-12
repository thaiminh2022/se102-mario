#pragma once
#include <functional>
#include <string>
#include <xaudio2.h>
#include <unordered_map>
#include  <vector>

#include "OptionalType.h"

using std::unordered_map;
using std::vector;

struct SoundData
{
	int id;
	WAVEFORMATEXTENSIBLE format;
	vector<BYTE> audioBytes;
};

struct AudioInstance
{
	int soundId = -1;
	unsigned int handle = 0;
	IXAudio2SourceVoice* voice = nullptr;
	bool paused = false;
	bool looping = false;
	bool markedForDelete = false;
	std::function<void()> onEnd = nullptr;
};    



class AudioManager
{
	static AudioManager* _instance;

	IXAudio2* xAudio2 = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;

	unordered_map<int, SoundData> soundData;
	unordered_map<unsigned int, AudioInstance> activeInstances;
	unordered_map<std::wstring, int> filePathToID;

	unsigned int musicHandle = 0;


	//helper
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	static unsigned int nextHandle;
	AudioInstance* FindInstance(unsigned int handleID);
	static void DestroyInstance(AudioInstance* instance);

public:
	static AudioManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new AudioManager;

		return _instance;
	}

	bool Init();
	void Shutdown();
	void Update();

	bool LoadWAV(int id, LPCWSTR filePath);

	unsigned int Play(int soundId, bool looping = false, std::function<void()> onEnd = nullptr);
	unsigned int PlaySFX(int soundId);
	unsigned int PlayMusic(int soundId, bool looping = true, std::function<void()> onEnd = nullptr);

	void Stop(unsigned int playbackHandle);
	void Pause(unsigned int playbackHandle);
	void Resume(unsigned int playbackHandle);
	Optional<int> GetIdForWAVFile(const std::wstring& filePath)
	{
		Optional<int> id{};
		if (filePathToID.find(filePath) != filePathToID.end())
		{
			id.Set(filePathToID[filePath]);
		} 
		return id;
	}


	void StopMusic();
	void PauseMusic();
	void ResumeMusic();

	void StopAll();
	
	~AudioManager()
	{
		if (masteringVoice) masteringVoice->DestroyVoice();
		if (xAudio2) xAudio2->Release();
		xAudio2 = nullptr;
		masteringVoice = nullptr;
	}
};


