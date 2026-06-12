#include "Button.hpp"

Button::Button(const glm::vec2& position, const glm::vec2& size)
    : position(position)
    , size(size)
{
}

bool Button::isPressed(const glm::vec2& mouse_position, bool mouse_up) const
{
    typewriter::AABB bounds{position, size};
    return mouse_up && bounds.contains(mouse_position);
}
