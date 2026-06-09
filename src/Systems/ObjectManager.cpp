#include "Systems/ObjectManager.hpp"

#include "Components.hpp"
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
        microwave.heat_timer += deltaTime;
        
        if (microwave.heat_timer >= microwave.heat_time)
        {
            microwave.status = Components::Microwave::Status::DONE;
        }
    }
}
