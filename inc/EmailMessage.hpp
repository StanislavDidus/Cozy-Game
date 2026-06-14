#pragma once
#include <string>

struct EmailMessage
{
    EmailMessage(const std::string& title, const std::string& text)
	: title(title)
	, text(text)
{
}
    ~EmailMessage() = default;
    
    std::string title;
    std::string text;
    bool read = false;
    
    std::optional<std::function<void()>> close_function; // function that will be called when message is closed for the first time
    bool func_active = true;
};
