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
}
