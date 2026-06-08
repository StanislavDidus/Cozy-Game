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
    
    void spawnFood(typewriter::Entity delivery_zone);
private:
    typewriter::Scene& scene;
};