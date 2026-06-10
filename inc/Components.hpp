#pragma once

#include "Layers.hpp"
#include "core/ecs/Scene.hpp"
#include "glm/vec2.hpp"
#include "graphics/Sprite.hpp"
#include "graphics/SpriteAnimation.hpp"
#include "graphics/SDL/shared/SDLFont.hpp"

namespace Components
{
    struct Transform2D
    {
        glm::vec2 position;
        glm::vec2 size;
    };
    
    struct Sprite2D
    {
        typewriter::Sprite sprite;
        int layer = BASE_LAYER;
        bool ui = false;
    };
    
    struct SpriteAnimation
    {
        typewriter::SpriteAnimation sprite_animation;
        int frame = 0;
        int layer = BASE_LAYER;
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
    
    struct Collider
    {
        glm::vec2 min;
        glm::vec2 max;
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
    
    struct Button
    {
        std::function<void()> func;
    };
}
