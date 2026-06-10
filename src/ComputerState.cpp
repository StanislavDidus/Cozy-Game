#include "Components.hpp"
#include "Config.hpp"
#include "FoodSpawner.hpp"
#include "GameLayer.hpp"

#include "Systems/InteractionSystem.hpp"

void GameLayer::setComputerState(ComputerState state)
{
    if (state == current_computer_state) return;
    
    exitComputerState(current_computer_state);
    
    current_computer_state = state;
    
    enterComputerState(current_computer_state);
}

void GameLayer::enterComputerState(ComputerState state)
{
    auto& registry = scene.getRegistry();
    exit_button = scene.createEntity();
    registry.emplace<Components::Transform2D>(exit_button, glm::vec2{675.0f, 130.0f}, glm::vec2{150.0f, 70.0f});
    registry.emplace<Components::Sprite2D>(exit_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,0,12,5}), 1);
    registry.emplace<Components::Button>(exit_button, [this]{setState(GameState::G_GAME);});
    
    switch (current_computer_state)
   {
   case ComputerState::G_NONE:
       setComputerState(ComputerState::G_MENU);
       break;
   case ComputerState::G_MENU:
       {
           food_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(food_button, glm::vec2{100.0f, 170.0f}, glm::vec2{90.0f, 90.0f});
           registry.emplace<Components::Sprite2D>(food_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{0,0,8,8}), 1);
           registry.emplace<Components::Button>(food_button, [this]{setComputerState(ComputerState::G_FOOD);});
           
           game_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(game_button, glm::vec2{205.0f, 170.0f}, glm::vec2{90.0f, 90.0f});
           registry.emplace<Components::Sprite2D>(game_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{16,0,8,8}), 1);
           registry.emplace<Components::Button>(game_button, [this]{setComputerState(ComputerState::G_GAME);});
           break;
       }
   case ComputerState::G_FOOD:
       {
           food_order_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(food_order_button, glm::vec2{400.0f, 325.0f}, glm::vec2{190.0f, 100.0f});
           registry.emplace<Components::Sprite2D>(food_order_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{0,8,22,7}), 1);
           registry.emplace<Components::Button>(food_order_button, [this]
           {
               if (food_order_timer >= FOOD_ORDER_TIME)
               {
                   food_spawner->spawnFood(delivery_zone);
                   food_order_timer = 0.0f;
               }
           });
           
           return_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(return_button, glm::vec2{570.0f, 130.0f}, glm::vec2{70.0f, 70.0f});
           registry.emplace<Components::Sprite2D>(return_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,5,5,5}), 1);
           registry.emplace<Components::Button>(return_button, [this]{setComputerState(ComputerState::G_MENU);});
           break;
       }
    case ComputerState::G_NEWS:
        break;
    case ComputerState::G_GAME:
        {
            return_button = scene.createEntity();
            registry.emplace<Components::Transform2D>(return_button, glm::vec2{570.0f, 130.0f}, glm::vec2{70.0f, 70.0f});
            registry.emplace<Components::Sprite2D>(return_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,5,5,5}), 1);
            registry.emplace<Components::Button>(return_button, [this]{setComputerState(ComputerState::G_MENU);});
            
            break;
        }
   default:
       break;
   }
}

void GameLayer::exitComputerState(ComputerState state)
{
    auto& registry = scene.getRegistry(); 
    registry.destroy(exit_button);
    
    switch (current_computer_state)
    {
    case ComputerState::G_NONE:
        break;
    case ComputerState::G_MENU:
        {
            registry.destroy(food_button);
            registry.destroy(game_button);
        }
        break;
    case ComputerState::G_FOOD:
        {
            registry.destroy(food_order_button);
            registry.destroy(return_button);
        }
        break;
    case ComputerState::G_NEWS:
        break;
    case ComputerState::G_GAME:
        registry.destroy(return_button);
        break;
    default:
        break;
    }
}

void GameLayer::updateComputerState(ComputerState state, float deltaTime)
{
    switch (current_computer_state)
    {
    case ComputerState::G_NONE:
        break;
    case ComputerState::G_MENU:
        break;
    case ComputerState::G_FOOD:
        break;
    default:
        break;
    }
}

void GameLayer::renderComputerState(ComputerState state)
{
    switch (current_computer_state)
    {
    case ComputerState::G_NONE:
        break;
    case ComputerState::G_MENU:
        break;
    case ComputerState::G_FOOD:
        {
            auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 30);
        
            if (food_order_timer >= FOOD_ORDER_TIME)
            {
                auto text = typewriter::ResourceManager::loadText(font, std::format("You can order food"));
                text->setColor(typewriter::Color::Green);
                typewriter::Renderer2D::drawText(text.get(), 400.0f, 200.0f, 1);
            }
            else
            {
                float time_left = FOOD_ORDER_TIME -  food_order_timer;
                auto text = typewriter::ResourceManager::loadText(font, std::format("Wait {} to order.", time_left));
                typewriter::Renderer2D::drawText(text.get(), 400.0f, 200.0f, 1);
            }
            break;
        }
    default:
        break;
    }
    
}
