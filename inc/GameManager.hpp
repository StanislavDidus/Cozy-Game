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
	std::vector<EventPoint> events;
};

class GameManager
{
public:
    GameManager() = default;
	~GameManager() = default;

	void update(float deltaTime);
	
	bool isDayEnded() const;
	
	int getCurrentday() const;
	DayPhase getDayPhase() const;
	
	void setGameDay(int day);
	void setNextDay();
	void initDays(const std::vector<Day>& days);
private:
	float timer = 0.0f;
	
	int current_day = 0;
	std::vector<Day> days;
};