#pragma once
#include <cmath>
#include <string>

#include "Config.hpp"

class Dialogue
{
public:
    Dialogue(const std::string& text);
    ~Dialogue() = default;
    
    void update(float deltaTime)
    {
        timer += deltaTime;
    }
    
    std::string getCurrentText() const
    {
        float total_time = CHARACTER_APPEAR_SPEED * full_text.length();
        int character_index = static_cast<int>(std::floor(timer / total_time));
        
        return full_text.substr(0, character_index);
    }
    
    void skipDialogue()
    {
        float total_time = CHARACTER_APPEAR_SPEED * full_text.length();
        timer = total_time;
    }
private:
    std::string full_text;
    float timer = 0.0f;
    float speed = 1.0f;
};
