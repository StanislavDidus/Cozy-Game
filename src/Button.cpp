#include "Button.hpp"

Button::Button(const glm::vec2& position, const glm::vec2& size)
    : position(position)
    , size(size)
{
}

static bool isPointInsideRect(const glm::vec2& point, const typewriter::RectF& rect)
{
    if (point.x < rect.left ||
        point.x > rect.left + rect.width ||
        point.y < rect.top ||
        point.y > rect.top + rect.height) return false;
    return true;
}

bool Button::isPressed(const glm::vec2& mouse_position, bool mouse_up) const
{
    typewriter::RectF rect{position.x, position.y, size.x, size.y};
    if (mouse_up && isPointInsideRect(mouse_position, rect))
    {
        return true;
    }
    return false;
}
