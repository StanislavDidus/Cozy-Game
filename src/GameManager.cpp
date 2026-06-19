#include "GameManager.hpp"

#include "Components.hpp"
#include "Config.hpp"
#include "SoundManager.hpp"
#include "graphics/Renderer2D.hpp"
#include "graphics/ResourceManager.hpp"

GameManager::GameManager(typewriter::Scene& scene, typewriter::Entity player, typewriter::Entity window)
    : scene(scene)
    , window(window)
    , player(player)
{
    std::vector<int> frames = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    eye_animation = std::make_unique<typewriter::SpriteAnimation>(typewriter::ResourceManager::loadSpriteSheet("assets/Eye.png", 32,16), 14.0f, true, frames);
}

void GameManager::update(float deltaTime)
{
    if (scene.getRegistry().get<Components::Player>(player).sanity < 0.0f)
        scene.getRegistry().get<Components::Player>(player).sanity = 0.0f; 
    
    if (progress_stopped) return;
    
    timer += deltaTime;
    
    eye_animation->update(deltaTime);
   
    if (current_day < days.size())
    {
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
    
    // Update drones
    if (drones_active)
    {
        if (scene.getRegistry().get<Components::Window>(window).opened)
        {
            window_open_timer += deltaTime;
        }
        else
        {
            window_open_timer = 0.0f;
        }
    }
    
    float hunger_up = 0.0f;
    float heat_up = 0.0f;
    float sanity_up = 0.0f;
    if (current_day < days.size())
    {
        auto& day = days[current_day];
        hunger_up = day.hunger_up.value_or(HUNGER_UP);
        heat_up = day.heat_up.value_or(HEAT_UP);
        sanity_up = day.sanity_up.value_or(SANITY_UP);
    }
    else
    {
        hunger_up = HUNGER_UP;
        heat_up = HEAT_UP;
        sanity_up = SANITY_UP;
    }
    
    scene.getRegistry().get<Components::Player>(player).hunger += hunger_up * deltaTime;
    scene.getRegistry().get<Components::Player>(player).sanity += sanity_up * deltaTime;
    
    auto& window_component = scene.getRegistry().get<Components::Window>(window);
    auto& player_component = scene.getRegistry().get<Components::Player>(player);
    if (window_component.opened)
    {
        float heat_mult = 1.5f;
        player_component.temperature -= hunger_up * heat_mult * deltaTime;
    }
    else
    {
        player_component.temperature += heat_up * deltaTime;
    }
    
    player_component.temperature = glm::clamp(player_component.temperature, 0.0f, 1.0f);
    
}

void GameManager::render()
{
    // Render drone attack warning    
    if (window_open_timer >= DRONE_ATTACK_TIME - DRONE_WARNING_TIME && drones_active)
    {
       typewriter::Renderer2D::drawSprite(*eye_animation.get()
           ,130.0f, 150.0f, 100.0f, 65.0f);
        
        SoundManager::get().getSound("Warning").play();
    }
    else
    {
        SoundManager::get().getSound("Warning").stop();
    }
}

bool GameManager::isDayEnded() const
{
    return timer >= DAY_TOTAL_DURATION;
}

bool GameManager::isDroneWarning() const
{
    if (window_open_timer >= DRONE_ATTACK_TIME - DRONE_WARNING_TIME && drones_active)
    {
        return true;
    }
    return false;
}

bool GameManager::isDroneAttack() const
{
    return window_open_timer >= DRONE_ATTACK_TIME && drones_active;
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

float GameManager::getTimer() const
{
    return timer;
}

void GameManager::setPlayer(typewriter::Entity player)
{
    this->player = player;
}

void GameManager::setWindow(typewriter::Entity window)
{
    this->window = window;
}

void GameManager::setGameDay(int new_day)
{
    this->current_day = new_day;
    resetTimer();
}

void GameManager::stopProgress(bool stopped)
{
    progress_stopped = stopped;
}

void GameManager::setCanDroneAttack(bool value)
{
    drones_active = value;
}

void GameManager::setNextDay()
{
    setGameDay(current_day + 1);
}

void GameManager::restartDay()
{
    setGameDay(current_day);
}

void GameManager::resetTimer()
{
    timer = 0.0f;
}

void GameManager::resetDroneTimer()
{
    window_open_timer = 0.0f;
}

void GameManager::initDays(const std::vector<Day>& days)
{
    this->days = days;
}
