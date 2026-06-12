#include "GameManager.hpp"

#include "Config.hpp"

void GameManager::update(float deltaTime)
{
    timer += deltaTime;
   
    if (current_day >= days.size()) return;
    
    auto& day = days[current_day];
        
    for (auto& event : day.events)
    {
        if (timer >= event.time && event.active)
        {
            event.active = false;
            event.func();
        }
    }
}

bool GameManager::isDayEnded() const
{
    return timer >= DAY_TOTAL_DURATION;
}

int GameManager::getCurrentday() const
{
    return current_day;
}

DayPhase GameManager::getDayPhase() const
{
    if (timer < DAY_MORNING_DURATION) return DayPhase::MORNING;
    if (timer < DAY_MORNING_DURATION + DAY_AFTERNOON_DURATION) return DayPhase::AFTERNOON;
    if (timer < DAY_MORNING_DURATION + DAY_AFTERNOON_DURATION + DAY_EVENING_DURATION) return DayPhase::EVENING;
    return DayPhase::UNDEFINED;
}

void GameManager::setGameDay(int new_day)
{
    this->current_day = new_day;
    timer = 0.0f;
}

void GameManager::setNextDay()
{
    setGameDay(current_day + 1);
}

void GameManager::initDays(const std::vector<Day>& days)
{
    this->days = days;
}
