#include "GameManager.hpp"

void GameManager::update(float deltaTime)
{
    timer += deltaTime;
    
    for (auto& day : days)
    {
        if (day.game_day != current_game_day)
            continue;
        
        for (auto& event : day.events)
        {
            if (event.time >= timer && event.active)
            {
                event.active = false;
                event.func();
            }
        }
    }
}
