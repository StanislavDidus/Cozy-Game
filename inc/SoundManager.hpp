#pragma once
#include <filesystem>
#include <unordered_map>

#include "Audio/Sound.hpp"

class SoundManager
{
public:
	static SoundManager& get()
	{
		static SoundManager s;
		return s;
	}
	
	void loadSound(const std::string& name, const std::filesystem::path& path, Audio::Sound::Type type = Audio::Sound::Type::Sound)
	{
		sounds[name] = Audio::Sound{path, type};
	}
	
	Audio::Sound& getSound(const std::string& name)
	{
		return sounds.at(name);
	}
private:
	SoundManager() = default;
	~SoundManager() = default;
	
	 std::unordered_map<std::string, Audio::Sound> sounds;
};
