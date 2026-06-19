#pragma once

#include "Components.hpp"
#include "Enums.hpp"
#include "core/ecs/Scene.hpp"

struct EventPoint
{
	std::function<void()> func;
	float time = 0.0f;
	bool active = true;
};

struct Day
{
	std::vector<EventPoint> events;
	
	std::optional<float> hunger_up;
	std::optional<float> heat_up;
	std::optional<float> sanity_up;
};

class GameManager
{
public:
    GameManager(typewriter::Scene& scene, typewriter::Entity player, typewriter::Entity window);
	~GameManager() = default;

	void update(float deltaTime);
	void render();
	
	bool isDayEnded() const;
	bool isDroneWarning() const;
	bool isDroneAttack() const;
	
	int getCurrentday() const;
	DayPhase getDayPhase() const;
	float getTimer() const;
	
	void setPlayer(typewriter::Entity player);
	void setWindow(typewriter::Entity window);
	void setGameDay(int day);
	void stopProgress(bool stopped);
	void setCanDroneAttack(bool value);
	void setNextDay();
	void restartDay();
	void resetTimer();
	void resetDroneTimer();
	void initDays(const std::vector<Day>& days);
private:
	float timer = 0.0f;
	
	int current_day = 0;
	std::vector<Day> days;
	
	bool progress_stopped = false;	
	// DRONE ATTACK SYSTEM
	bool drones_active = true;
	float window_open_timer = 0.0f;
	
	typewriter::Scene& scene;
	typewriter::Entity player;
	typewriter::Entity window;
	
	std::unique_ptr<typewriter::SpriteAnimation> eye_animation = nullptr;
};