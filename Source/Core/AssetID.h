#pragma once
#include <cstdint>
#include <string>

// Compile-time FNV-1a hash function
constexpr int Hash(const char* str)
{
    uint32_t hash = 0x811C9DC5;
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        hash ^= static_cast<uint32_t>(str[i]);
        hash *= 0x01000193;
    }
    return static_cast<int>(hash);
}

//ID Wrapper Struct
struct AssetID
{
    int hash; // The numeric hash value (Always present)

#ifdef _DEBUG
    const char* name; // The original string name (Only present in Debug builds)
#endif

    // Constructor for automatic compile-time hashing
    constexpr AssetID(const char* str) : hash(Hash(str))
#ifdef _DEBUG
        , name(str)
#endif
    {
    }

    // Implicit conversion operator to 'int' for seamless use in switch-case statements
    constexpr operator int() const { return hash; }

    // Utility function for logging and debugging
    std::string ToString() const
    {
#ifdef _DEBUG
        return std::string(name); // Returns the actual string name (e.g., "MARIO_RUN_ANIM_ID")
#else
        return std::to_string(hash); // Returns the numeric hash value (e.g., "28471928")
#endif
    }
};