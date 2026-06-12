#pragma once

#include "glm/vec2.hpp"
#include <typewriter/Typewriter.hpp>

namespace Components
{
    struct Sprite2D
    {
        typewriter::Sprite sprite;
        bool ui = false;
    };
    
    struct SpriteAnimation
    {
        typewriter::SpriteAnimation sprite_animation;
        int frame = 0;
        bool ui = false;
        
    };
    
    struct Player
    {
        glm::vec2 acceleration;
        glm::vec2 deceleration;
        glm::vec2 velocity;
        float movement_speed;
        float max_movement_speed;
        
        int inv_food = 0;
        int max_inv_food = 3;
        
        float hunger = 0.0f;
        float temperature = 0.0f;
        float sanity = 0.0f;
    };
    
    struct CanInteract
    {
        float radius;
    };
    
    struct InteractableObject
    {
        std::function<void(typewriter::Entity player, typewriter::Entity object)> func;
    };
    
    struct Microwave
    {
        enum class Status
        {
            EMPTY,
            COOKING,
            DONE,
        };
        
        float heat_time;
        float heat_timer = 0.0f;
        Status status = Status::EMPTY;
    };
    
    struct Window
    {
        bool opened = false;
    };
    
    struct DeliveryZone
    {
        typewriter::Entity food = entt::null;
    };
    
    struct FoodPackage
    {
        int food_count = 1;    
    };

    typewriter::Entity createButton(typewriter::Registry& registry, const std::function<bool()>& onClick, const glm::vec2& position, const glm::vec2& size, const typewriter::Sprite& sprite);
}
