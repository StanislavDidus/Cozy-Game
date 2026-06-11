#include "Button.hpp"
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
    registry.emplace<Components::Sprite2D>(exit_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,0,12,5}), 1, true);
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
           registry.emplace<Components::Sprite2D>(food_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{0,0,8,8}), 1, true);
           registry.emplace<Components::Button>(food_button, [this]{setComputerState(ComputerState::G_FOOD);});
           
           game_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(game_button, glm::vec2{205.0f, 170.0f}, glm::vec2{90.0f, 90.0f});
           registry.emplace<Components::Sprite2D>(game_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{16,0,8,8}), 1, true);
           registry.emplace<Components::Button>(game_button, [this]{setComputerState(ComputerState::G_GAME);});
           
           news_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(news_button, glm::vec2{310.0f, 170.0f}, glm::vec2{90.0f, 90.0f});
           registry.emplace<Components::Sprite2D>(news_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{8,0,8,8}), 1, true);
           registry.emplace<Components::Button>(news_button, [this]{setComputerState(ComputerState::G_NEWS);});
           
           break;
       }
   case ComputerState::G_FOOD:
       {
           food_order_button = scene.createEntity();
           registry.emplace<Components::Transform2D>(food_order_button, glm::vec2{400.0f, 325.0f}, glm::vec2{190.0f, 100.0f});
           registry.emplace<Components::Sprite2D>(food_order_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{0,8,22,7}), 1, true);
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
           registry.emplace<Components::Sprite2D>(return_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,5,5,5}), 1, true);
           registry.emplace<Components::Button>(return_button, [this]{setComputerState(ComputerState::G_MENU);});
           break;
       }
    case ComputerState::G_NEWS:
        return_button = scene.createEntity();
        registry.emplace<Components::Transform2D>(return_button, glm::vec2{570.0f, 130.0f}, glm::vec2{70.0f, 70.0f});
        registry.emplace<Components::Sprite2D>(return_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,5,5,5}), 1, true);
        registry.emplace<Components::Button>(return_button, [this]
        {
            // If player is reading something we close the message 
            // If not we just go to menu
            if (reading_message != std::nullopt)
                reading_message = std::nullopt;
            else
            {
                setComputerState(ComputerState::G_MENU);
            }
        });
        break;
    case ComputerState::G_GAME:
        {
            return_button = scene.createEntity();
            registry.emplace<Components::Transform2D>(return_button, glm::vec2{570.0f, 130.0f}, glm::vec2{70.0f, 70.0f});
            registry.emplace<Components::Sprite2D>(return_button, typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{24,5,5,5}), 1, true);
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
            registry.destroy(news_button);
        }
        break;
    case ComputerState::G_FOOD:
        {
            registry.destroy(food_order_button);
            registry.destroy(return_button);
        }
        break;
    case ComputerState::G_NEWS:
        {
            registry.destroy(return_button);
        }
        break;
    case ComputerState::G_GAME:
        game_boot_timer = 0.0f;
        obstacle_spawn_timer = 0.0f;
        registry.destroy(return_button);
        obstacles.clear();
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
    case ComputerState::G_GAME:
        {
            game_boot_timer += deltaTime;
            
            if (game_boot_timer > GAME_BOOT_TIME)
            {
                // Update players position
                if (player_y_pos >= 0.0f)
                {
                    player_y_pos = 0.0f;
                    player_velocity_y = 0.0f;
                    
                    if (typewriter::Input::isKeyPressed(typewriter::SCANCODE_SPACE))
                    {
                        player_velocity_y = -player_jump_force;
                    }
                }
                else
                {
                    player_velocity_y += player_gravity * deltaTime; 
                }
                
                player_y_pos += player_velocity_y * deltaTime;
                
                if (player_y_pos > 0.0f)
                {
                    player_y_pos = 0.0f;
                    player_velocity_y = 0.0f;
                }
                
                // Spawn obstacles
                obstacle_spawn_timer += deltaTime;
                if (obstacle_spawn_timer > OBSTACLE_SPAWN_TIME)
                {
                    obstacle_spawn_timer = 0.0f;     
                    
                    typewriter::AABB obstacle{glm::vec2{OBSTACLE_SPAWN_POSITION_X, OBSTACLE_SPAWN_POSITION_Y}, glm::vec2{OBSTACLE_SPAWN_POSITION_X + OBSTACLE_SPAWN_WIDTH, OBSTACLE_SPAWN_POSITION_Y + OBSTACLE_SPAWN_HEIGHT}};
                    obstacles.push_back(obstacle);
                }
                
                // Move obstacles
                for (auto& obstacle : obstacles)
                {
                    obstacle.min.x -= OBSTACLE_MOVEMENT_SPEED * deltaTime;
                    obstacle.max.x -= OBSTACLE_MOVEMENT_SPEED * deltaTime;
                }
                
                // Check if player hits obstacle
                for (const auto& obstacle : obstacles)
                {
                    float x = 115.0f;
                    float y = 320.0f + player_y_pos;
                    float width = 40.0f;
                    float height = 70.0f;
                    typewriter::AABB a{glm::vec2{x, y}, glm::vec2{x + width, y + height}};
                    if (a.overlap(obstacle))
                    {
                        setComputerState(ComputerState::G_MENU);
                    }
                }
                
                // Delete obstacles that are offscreen
                obstacles.erase(std::ranges::remove_if(obstacles, [](const typewriter::AABB& a)
                {
                    return a.min.x < OBSTACLE_DESTROY_POSITION;
                }).begin(), obstacles.end());
                
                // Update sanity timer
                sanity_timer += deltaTime;
                if (sanity_timer >= SANITY_TIME)
                {
                    sanity_timer = 0.0f;
                    scene.getRegistry().get<Components::Player>(player).sanity -= SANITY_GIVE;
                }
            }
            else
            {
                
            }
        }
        break;
    case ComputerState::G_NEWS:
        {
            // If player is reading something we don't update buttons
            if (reading_message != std::nullopt) break;
        
            auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", MESSAGE_TEXT_SIZE);
            int pos = 0;
            //int to_show = glm::clamp(MESSAGE_MAX_SHOWN, 0, static_cast<int>(messages.size()));
            for (int i = starting_point; i < starting_point + MESSAGE_MAX_SHOWN; ++i)
            {
                if (i >= messages.size()) break;
                
                auto& msg = messages[i];
                float offset_y = pos * MESSAGE_HEIGHT;
                
                // Read button
                Button button{glm::vec2{MESSAGE_POSITION_X + MESSAGE_WIDTH, MESSAGE_POSITION_Y + offset_y,}, glm::vec2{MESSAGE_READ_WIDTH, MESSAGE_READ_HEIGHT}};
                if (button.isPressed(mouse_position, mouse_up))
                {
                    reading_message = msg;
                    msg.read = true;
                }
                
                ++pos;
            }
        }
        
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
                typewriter::Renderer2D::drawText(text.get(), 400.0f, 200.0f);
            }
            else
            {
                float time_left = FOOD_ORDER_TIME -  food_order_timer;
                auto text = typewriter::ResourceManager::loadText(font, std::format("Wait {} to order.", time_left));
                typewriter::Renderer2D::drawText(text.get(), 400.0f, 200.0f);
            }
            break;
        }
    case ComputerState::G_GAME:
        {
            if (game_boot_timer > GAME_BOOT_TIME)
            {
                // Draw ground
                typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Game.png", typewriter::RectI{0,0,16,8}), 100.0f, 400.0f, 650.0f, 65.0f);
                
                // Draw player
                typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Game.png", typewriter::RectI{0,8,6,8}), 115.0f, 320.0f + player_y_pos, 40.0f, 70.0f);
                
                // Draw obstacles
                for (const auto& obstacle : obstacles)
                {
                    typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Game.png", typewriter::RectI{16,0,16,16}), obstacle.min.x, obstacle.min.y, obstacle.max.x - obstacle.min.x, obstacle.max.y - obstacle.min.y);
                }
            }
            else
            {
                typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/UI.png", typewriter::RectI{0,0,16,16}), 400.0f, 300.0f, 100.0f, 100.0f);
            }
        }
        break;
    case ComputerState::G_NEWS:
        {
            auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", MESSAGE_TEXT_SIZE);
            if (reading_message == std::nullopt)
            {
                int pos = 0;
                //int to_show = glm::clamp(MESSAGE_MAX_SHOWN, 0, static_cast<int>(messages.size()));
                for (int i = starting_point; i < starting_point + MESSAGE_MAX_SHOWN; ++i)
                {
                    if (i >= messages.size()) break;
                    
                    auto& msg = messages[i];
                    float offset_y = pos * MESSAGE_HEIGHT;
                    
                    typewriter::Sprite sprite = typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{36,0 ,32,9});
                    typewriter::Renderer2D::drawSprite(sprite, MESSAGE_POSITION_X, MESSAGE_POSITION_Y + offset_y, MESSAGE_WIDTH, MESSAGE_HEIGHT);
                    auto text = typewriter::ResourceManager::loadText(font, msg.title);
                    typewriter::Renderer2D::drawText(text.get(), MESSAGE_POSITION_X, MESSAGE_POSITION_Y + offset_y);
                    
                    // Read button
                    typewriter::Sprite read_button_sprite_green = typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{36,9 ,15,6});
                    typewriter::Sprite read_button_sprite_red = typewriter::ResourceManager::loadSprite("assets/Buttons.png", typewriter::RectI{51,9 ,15,6});
                    if (!msg.read)
                        typewriter::Renderer2D::drawSprite(read_button_sprite_green, MESSAGE_POSITION_X + MESSAGE_WIDTH, MESSAGE_POSITION_Y + offset_y, MESSAGE_READ_WIDTH, MESSAGE_READ_HEIGHT);
                    else
                        typewriter::Renderer2D::drawSprite(read_button_sprite_red, MESSAGE_POSITION_X + MESSAGE_WIDTH, MESSAGE_POSITION_Y + offset_y, MESSAGE_READ_WIDTH, MESSAGE_READ_HEIGHT);
                    auto read_button_text = typewriter::ResourceManager::loadText(font, "READ");
                    typewriter::Renderer2D::drawText(read_button_text.get(), MESSAGE_POSITION_X + MESSAGE_WIDTH, MESSAGE_POSITION_Y + offset_y);
                    
                    ++pos;
                }
            }
            else
            {
                auto text = typewriter::ResourceManager::loadText(font, reading_message->text);
                typewriter::Renderer2D::drawText(text.get(), MESSAGE_TEXT_POSITION_X, MESSAGE_POSITION_Y);
                
                typewriter::Renderer2D::drawRectangle(MESSAGE_POSITION_X - 20.0f, MESSAGE_POSITION_Y - 20.0f, 300.0f, 300.0f, typewriter::Color::DarkSlateGrey);
            }
        }
        break;
    default:
        break;
    }
    
}
