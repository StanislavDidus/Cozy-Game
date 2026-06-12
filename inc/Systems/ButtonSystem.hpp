#pragma once
#include "glm/vec2.hpp"

namespace typewriter
{
  inline namespace core
  { 
    class Scene;
  }
}

class ButtonSystem
{
public:
  ButtonSystem(typewriter::Scene& scene);
  
  void update(float deltaTime, const glm::vec2& mouse_position, bool mouse_down, bool mouse_up);
private:
  typewriter::Scene& scene;
  
  std::vector<std::function<bool()>> functions;
};