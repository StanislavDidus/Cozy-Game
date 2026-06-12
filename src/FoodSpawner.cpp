#include "FoodSpawner.hpp"

#include "Components.hpp"
#include <typewriter/Typewriter.hpp>

using namespace typewriter;

FoodSpawner::FoodSpawner(Scene& scene)
    : scene(scene)
{
    
}

constexpr int MAX_FOOD = 3;
constexpr float FOOD_WIDTH = 70.0f;
constexpr float FOOD_HEIGHT = 70.0f;
void FoodSpawner::spawnFood(Entity delivery_zone)
{
    auto& registry = scene.getRegistry();
    
    auto& delivery_zone_component  = registry.get<Components::DeliveryZone>(delivery_zone);
    
    // If there is no food on the delivery zone
    if (delivery_zone_component.food == entt::null)
    {
       // Create food object 
        Entity food = registry.create(); 
        glm::vec2 delivery_position = registry.get<Transform2D>(delivery_zone).position;
        registry.emplace<Transform2D>(food, glm::vec2{delivery_position.x, delivery_position.y}, glm::vec2{FOOD_WIDTH, FOOD_HEIGHT});
        registry.emplace<Components::Sprite2D>(food, ResourceManager::loadSprite("assets/FoodPackage.png", math::RectI(0,0,16,16)));
        registry.emplace<Components::FoodPackage>(food);
        
        // Assign food to delivery zone
        delivery_zone_component.food = food;
        
        // On Interact
        registry.emplace<Components::InteractableObject>(delivery_zone, [&delivery_zone_component, &registry](Entity player, Entity object)
        {
            auto& player_component = registry.get<Components::Player>(player);
            if (delivery_zone_component.food != entt::null &&
                player_component.inv_food < player_component.max_inv_food)
            {
                player_component.inv_food += registry.get<Components::FoodPackage>(delivery_zone_component.food).food_count;
                if (player_component.inv_food > player_component.max_inv_food)
                    player_component.inv_food = player_component.max_inv_food;
                registry.destroy(delivery_zone_component.food);
                delivery_zone_component.food = entt::null;
                registry.remove<Components::InteractableObject>(object);
            }
        });
    }
    // If there is already food
    else
    {
       // Increase food count 
        int& food_count = registry.get<Components::FoodPackage>(delivery_zone_component.food).food_count;
        
        // If reaches limit
        if (++food_count > MAX_FOOD)
        {
            //TODO: for now just type a warning 
            //LOG_WARN("Food limit reached");
            food_count = MAX_FOOD;
        }
    }
    
}
