#include <Components.hpp>

namespace Components {
    typewriter::Entity createButton(typewriter::Registry& registry, const std::function<bool()>& onClick, const glm::vec2& position, const glm::vec2& size, const typewriter::Sprite& sprite) {
        const typewriter::Entity button = typewriter::createButton(registry, onClick, position, size, typewriter::AABB(position, size));
        registry.emplace<Components::Sprite2D>(button, sprite, true);
        return button;
    }
}