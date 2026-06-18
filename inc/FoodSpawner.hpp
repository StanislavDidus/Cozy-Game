#pragma once
#include "core/ecs/Scene.hpp"

namespace typewriter
{
    inline namespace core
    { 
        class Scene;
    }
}

class FoodSpawner
{
public:
    FoodSpawner(typewriter::Scene& scene);
    ~FoodSpawner() = default;
    
    void update(float deltaTime);
    void spawnFood(typewriter::Entity delivery_zone);
    
    void render(uint8_t day, uint8_t night);
private:
    void spawnFoodObject();
    typewriter::Scene& scene;
    float spawn_time = 0.5f;
    float spawn_timer = 0.0f;
    bool spawn = false;
    typewriter::Entity delivery_zone = entt::null;
};