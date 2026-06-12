#include "Systems/PhysicsSystem.hpp"

#include "Components.hpp"
#include <typewriter/Typewriter.hpp>

PhysicsSystem::PhysicsSystem(typewriter::Scene& scene)
    : scene(scene)
{
}

void PhysicsSystem::update(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<typewriter::Transform2D, Components::Player>();
    for (auto [entity, transform, player] : view.each())
    {
        transform.position += player.velocity * deltaTime; 
    }
}
