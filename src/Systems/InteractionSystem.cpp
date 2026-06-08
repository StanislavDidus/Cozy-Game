#include "Systems/InteractionSystem.hpp"

#include "Components.hpp"
#include "core/ecs/Scene.hpp"
#include "glm/glm.hpp"
#include "graphics/Renderer2D.hpp"
#include "graphics/ResourceManager.hpp"

InteractionSystem::InteractionSystem(typewriter::Scene& scene)
    : scene(scene)
{
    
}

void InteractionSystem::update(float deltaTime)
{
}

void InteractionSystem::render()
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<Components::Transform2D, Components::CanInteract>();
    
    for (auto [entity, transform, can_interact] : view.each())
    {
        auto view_ = registry.view<Components::Transform2D, Components::InteractableObject>();
        
        for (auto [entity_, transform_, interactable_object_] : view_.each())
        {
            float distance = glm::distance(transform.position, transform_.position);
            
            if (distance < can_interact.radius)
            {
                auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 24);
                auto text = typewriter::ResourceManager::loadText(font, "Press SPACE to interact");
                typewriter::Renderer2D::drawText(text.get(), 0, 0);
            }
        }
    }
}
