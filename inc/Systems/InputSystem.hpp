#pragma once

namespace typewriter
{
   inline namespace core
   {
      class Scene;
   }
}

class InputSystem
{
public:
   InputSystem(typewriter::Scene& scene);
   ~InputSystem() = default; 
   
   void update(float deltaTime);
private:
   typewriter::Scene& scene;
};
