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

   // Returns true if microwave has finished cooking
   bool update(float deltaTime);
private:
   typewriter::Scene& scene;
};