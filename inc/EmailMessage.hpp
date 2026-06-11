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
};
