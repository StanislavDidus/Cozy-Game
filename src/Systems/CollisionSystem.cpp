#include "Systems/CollisionSystem.hpp"

#include "Components.hpp"
#include <typewriter/Typewriter.hpp>

CollisionSystem::CollisionSystem(typewriter::Scene& scene)
    : scene(scene)
{
}

void CollisionSystem::update(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<typewriter::Transform2D, typewriter::Collision2D, Components::Player>();
    for (const auto& [entity, transform, collider, player] : view.each())
    {
        auto view_ = registry.view<typewriter::Transform2D, typewriter::Collision2D>();
        for (const auto& [entity_, transform_, collider_] : view_.each())
        {
            if (entity == entity_) continue;
            
            typewriter::AABB a {transform.position + collider.bounds.min, transform.position + transform.size + collider.bounds.max};
            typewriter::AABB b {transform_.position + collider_.bounds.min, transform_.position + transform_.size + collider_.bounds.max};

            if (!a.intersect(b)) continue;

            glm::vec2 overlap
            {
                std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x),
                std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y),
            };

            glm::vec2 normal {0.0f};

            if (overlap.x < overlap.y)
            {
                if (a.min.x < b.min.x)
                {
                    normal.x = -1.0f;
                }
                else
                {
                    normal.x = 1.0f;
                }
                player.velocity.x = 0.0f;
            }
            else
            {
                if (a.min.y < b.min.y)
                {
                    normal.y = -1.0f;
                }
                else
                {
                    normal.y = 1.0f;
                }
                player.velocity.y = 0.0f;
            }

            transform.position += overlap * normal;
        }
    }
}
