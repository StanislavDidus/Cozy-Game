#include "Systems/ButtonSystem.hpp"

#include "Components.hpp"
#include <typewriter/Typewriter.hpp>

ButtonSystem::ButtonSystem(typewriter::Scene& scene)
    : scene(scene)
{
    
}

void ButtonSystem::update(float deltaTime, const glm::vec2& mouse_position, bool mouse_down, bool mouse_up)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<typewriter::Clickable>();
    for (const auto& [entity, button] : view.each())
    {
        if (button.clickableArea.contains(mouse_position) && mouse_up)
        {
            functions.push_back(button.onClick);
        }
    }
    
    for (const auto& func : functions)
    {
        func();
    }
    functions.clear();
}
