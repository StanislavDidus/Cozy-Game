#include "Systems/CollisionSystem.hpp"

#include "Components.hpp"
#include "../../extern/typewriter/externals/audio/src/miniaudio.h"
#include "core/ecs/Scene.hpp"

static bool AABB(const Components::Collider& a, const Components::Collider& b)
{
   if (a.max.x < b.min.x ||
       a.min.x > b.max.x ||
       a.max.y < b.min.y ||
       a.min.y > b.max.y) return false;
    return true;
}

CollisionSystem::CollisionSystem(typewriter::Scene& scene)
    : scene(scene)
{
}

void CollisionSystem::update(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto view = registry.view<Components::Transform2D, Components::Collider, Components::Player>();
    for (auto [entity, transform, collider, player] : view.each())
    {
        auto view_ = registry.view<Components::Transform2D, Components::Collider>();
        for (auto [entity_, transform_, collider_] : view_.each())
        {
            if (entity == entity_) continue;
            
            Components::Collider a {transform.position + collider.min, transform.position + transform.size + collider.max};
            Components::Collider b {transform_.position + collider_.min, transform_.position + transform_.size + collider_.max};
            
            if (!AABB(a, b)) continue;
            
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
