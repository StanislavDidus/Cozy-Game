#include "Systems/ButtonSystem.hpp"

#include "Components.hpp"
#include "core/ecs/Scene.hpp"

ButtonSystem::ButtonSystem(typewriter::Scene& scene)
    : scene(scene)
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

void ButtonSystem::update(float deltaTime, const glm::vec2& mouse_position, bool mouse_down, bool mouse_up)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<Components::Transform2D, Components::Button>();
    for (auto [entity, transform, button] : view.each())
    {
        typewriter::RectF rect {transform.position.x, transform.position.y,
        transform.size.x, transform.size.y};
        
        if (isPointInsideRect(mouse_position, rect) && mouse_up)
        {
            functions.push_back(button.func);
        }
    }
    
    for (const auto& func : functions)
    {
        func();
    }
    functions.clear();
}
