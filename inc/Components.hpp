#pragma once

#include "glm/vec2.hpp"
#include "graphics/Sprite.hpp"
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
    };
    
    struct Player
    {
        glm::vec2 acceleration;
        glm::vec2 deceleration;
        glm::vec2 velocity;
        float movement_speed;
        float max_movement_speed;
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
        std::function<void()> func;
    };
}
