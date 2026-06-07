#pragma once

namespace typewriter
{
   inline namespace core
   {
      class Scene;
   }
}

class PhysicsSystem
{
public:
   PhysicsSystem(typewriter::Scene& scene);
   ~PhysicsSystem() = default; 
   
   void update(float deltaTime);
private:
   typewriter::Scene& scene;
};
