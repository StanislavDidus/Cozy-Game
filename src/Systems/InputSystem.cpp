#include "Systems/InputSystem.hpp"
#include <typewriter/Typewriter.hpp>

#include "Components.hpp"

InputSystem::InputSystem(typewriter::Scene& scene)
    : scene{scene}
{
}

void InputSystem::update(float deltaTime)
{
    auto view = scene.getRegistry().view<Components::Transform2D, Components::Player>();
    for (auto [entity, transform, player] : view.each())
    {
        glm::vec2 direction {0.0f, 0.0f};
        if (typewriter::Input::isKeyPressed(typewriter::SCANCODE_W))
        {
            direction.y = -1.0f;
        }
        if (typewriter::Input::isKeyPressed(typewriter::SCANCODE_S))
        {
            direction.y = 1.0f;
        }
        if (typewriter::Input::isKeyPressed(typewriter::SCANCODE_A))
        {
            direction.x = -1.0f;
        }
        if (typewriter::Input::isKeyPressed(typewriter::SCANCODE_D))
        {
            direction.x = 1.0f;
        }
        
        if (std::abs(direction.x) > std::abs(direction.y))
        {
            direction.y = 0;
        }
        else
        {
            direction.x = 0;
        }
        
        glm::vec2 velocity = direction * player.movement_speed; 
        
        transform.position += velocity * deltaTime;
    }
}
