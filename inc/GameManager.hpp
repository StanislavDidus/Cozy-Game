#pragma once

#include "Enums.hpp"

struct EventPoint
{
	std::function<void()> func;
	float time = 0.0f;
	bool active = true;
};

struct Day
{
	float morning_duration = 0.0f;
	float afternoon_duration = 0.0f;
	float evening_duration = 0.0f;
	
	std::vector<EventPoint> events;
	
	GameDay game_day = GameDay::NONE;
	
	float getTotalDuration() const
	{
		return morning_duration + afternoon_duration + evening_duration;
	}
};

class GameManager
{
public:
    GameManager() = default;
	~GameManager() = default;

	void update(float deltaTime);
private:
	float timer = 0.0f;
	
	GameDay current_game_day = GameDay::NONE;
	std::vector<Day> days;
};