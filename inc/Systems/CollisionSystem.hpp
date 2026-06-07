#pragma once

namespace typewriter
{
  inline namespace core
  { 
    class Scene;
  }
}

class CollisionSystem
{
public:
    CollisionSystem(typewriter::Scene& scene);
    ~CollisionSystem() = default;

    void update(float deltaTime);
private:
    typewriter::Scene& scene; 
};