#pragma once

namespace typewriter
{
   inline namespace core
   {
      class Scene;
   }
}

class InteractionSystem
{
public:
   InteractionSystem(typewriter::Scene& scene);
   ~InteractionSystem() = default; 
   
   void update(float deltaTime, bool interact);
   void render();
private:
   typewriter::Scene& scene;
};
