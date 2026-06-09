#include "Systems/ObjectManager.hpp"

#include "Components.hpp"
#include "Config.hpp"
#include "core/ecs/Scene.hpp"

ObjectManager::ObjectManager(typewriter::Scene& scene)
    : scene(scene)
{
}

void ObjectManager::update(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<Components::Microwave>();
    for (auto [entity, microwave] : view.each())
    {
        if (microwave.status == Components::Microwave::Status::COOKING)
        {
            microwave.heat_timer += deltaTime;
            
            if (registry.all_of<Components::SpriteAnimation>(entity))
            {
                registry.get<Components::SpriteAnimation>(entity).frame = 1;
            }
        
            if (microwave.heat_timer >= microwave.heat_time)
            {
                microwave.status = Components::Microwave::Status::DONE;
                microwave.heat_timer = 0.0f;
                
                if (registry.all_of<Components::SpriteAnimation>(entity))
                {
                    registry.get<Components::SpriteAnimation>(entity).frame = 2;
                }
            }
        }
    }
    
    auto view1 = registry.view<Components::Window>();
    for (auto [entity, window] : view1.each())
    {
        auto view2 = registry.view<Components::Player>();
        for (auto [entity, player] : view2.each())
        {
            if (window.opened)
            {
                player.temperature -= HEAT_DOWN * deltaTime;
            }
            else
            {
                player.temperature += HEAT_UP * deltaTime;
            }
            
            player.temperature = glm::clamp(player.temperature, 0.0f, 1.0f);
        }
    }
}
