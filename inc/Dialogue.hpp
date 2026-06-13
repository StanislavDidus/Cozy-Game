#pragma once
#include <cmath>
#include <string>

#include "Config.hpp"

class Dialogue
{
public:
    Dialogue(const std::string& text)
        : full_text{text}
    {
        
    }
    ~Dialogue() = default;
    
    void update(float deltaTime)
    {
        timer += deltaTime;
        
        float total_time = CHARACTER_APPEAR_SPEED * full_text.length();
        if (timer >= total_time)
            is_finished = true;
    }
    
    std::string getCurrentText() const
    {
        float total_time = CHARACTER_APPEAR_SPEED * full_text.length();
        int character_index = static_cast<int>(std::floor(timer / total_time * full_text.length()));
        return full_text.substr(0, character_index);
    }
    
    bool isFinished() const
    {
        return is_finished;
    }
    
    void skipDialogue()
    {
        float total_time = CHARACTER_APPEAR_SPEED * full_text.length();
        timer = total_time;
        is_finished = true;
    }
private:
    std::string full_text;
    float timer = 0.0f;
    float speed = 1.0f;
    bool is_finished = false;
};
