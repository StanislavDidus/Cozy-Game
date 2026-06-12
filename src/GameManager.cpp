#include "GameManager.hpp"

#include "Config.hpp"
#include "graphics/Renderer2D.hpp"
#include "graphics/ResourceManager.hpp"

void GameManager::update(float deltaTime, bool is_window_open)
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
    
    // Update drones
    if (!drones_active) return;
    
    if (is_window_open)
    {
        window_open_timer += deltaTime;
    }
    else
    {
        window_open_timer = 0.0f;
    }
}

void GameManager::render()
{
    // Render drone attack warning    
    if (window_open_timer >= DRONE_ATTACK_TIME - DRONE_WARNING_TIME)
    {
       typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/UI.png", typewriter::RectI{48, 32, 16, 16})
           ,432.0f, -25.0f, 80.0f, 80.0f);
        
    }
}

bool GameManager::isDayEnded() const
{
    return timer >= DAY_TOTAL_DURATION;
}

bool GameManager::isDroneAttack() const
{
    return window_open_timer >= DRONE_ATTACK_TIME;
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

void GameManager::restartDay()
{
    setGameDay(current_day);
}

void GameManager::initDays(const std::vector<Day>& days)
{
    this->days = days;
}
