#pragma once
#include "graphics/Sprite.hpp"

class Button
{
public:
    Button(const glm::vec2& position, const glm::vec2& size);
    ~Button() = default; 
    
    bool isPressed(const glm::vec2& mouse_position, bool mouse_up) const;
private:
    glm::vec2 position;
    glm::vec2 size;
};
