#pragma once

namespace typewriter
{
   inline namespace core
   {
      class Scene;
   }
}

class ObjectManager
{
public:
   ObjectManager(typewriter::Scene& scene);
   ~ObjectManager() = default;
   
   void update(float deltaTime);
private:
   typewriter::Scene& scene;
};