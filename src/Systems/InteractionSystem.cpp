#include "Systems/InteractionSystem.hpp"

#include "Components.hpp"
#include "glm/glm.hpp"
#include <typewriter/Typewriter.hpp>

InteractionSystem::InteractionSystem(typewriter::Scene& scene)
    : scene(scene)
{
    
}

void InteractionSystem::update(float deltaTime, bool interact)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<typewriter::Transform2D, Components::CanInteract, Components::Player>();
    
    for (const auto& [entity, transform, can_interact, player] : view.each())
    {
        auto view_ = registry.view<typewriter::Transform2D, Components::InteractableObject>();
        
        for (const auto& [entity_, transform_, interactable_object_] : view_.each())
        {
            float distance = glm::distance(transform.position, transform_.position);
            
            if (distance < can_interact.radius && interact)
            {
                interactable_object_.func(entity, entity_);
            }
        }
    }
}

void InteractionSystem::render()
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<typewriter::Transform2D, Components::CanInteract>();
    
    for (const auto& [entity, transform, can_interact] : view.each())
    {
        auto view_ = registry.view<typewriter::Transform2D, Components::InteractableObject>();
        
        for (const auto& [entity_, transform_, interactable_object_] : view_.each())
        {
            float distance = glm::distance(transform.position, transform_.position);
            
            if (distance < can_interact.radius)
            {
                auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 24);
                auto text = typewriter::ResourceManager::loadText(font, "Press SPACE to interact");
                typewriter::Renderer2D::drawText(text.get(), transform_.position.x - 40.0f, transform_.position.y - 25.0f);
            }
        }
    }
}
