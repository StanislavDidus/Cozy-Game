#include "GameLayer.hpp"

#include "Components.hpp"
#include "Player.hpp"

GameLayer::GameLayer(int screen_width, int screen_height)
    : screen_width(screen_width)
    , screen_height(screen_height)
{
}

void GameLayer::onAttach()
{
    Layer::onAttach();
    
    camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width), static_cast<float>(screen_height), 0.0f);
    ui_camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width), 0.f, static_cast<float>(screen_height));
    
    initAssets();
    
    setState(GameState::G_GAME);
}

void GameLayer::onUpdate(float deltaTime)
{
    Layer::onUpdate(deltaTime);
    
    updateState(current_state);
}

void GameLayer::onRender()
{
    Layer::onRender();
    
    typewriter::Renderer2D::setClearColor(typewriter::Color::DarkGray);
    typewriter::Renderer2D::startScene(camera);
    
    renderState(current_state);
    
    typewriter::Renderer2D::endScene();
}

void GameLayer::setState(GameState new_state)
{
    if (new_state == current_state) return;
    
    exitState(current_state);
    
    current_state = new_state;
    
    enterState(current_state);
}

void GameLayer::exitState(GameState state)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        break;
    default:
        break;
    }
}

void GameLayer::enterState(GameState state)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        {
            player = scene.createEntity();
            typewriter::Registry& registry = scene.getRegistry();
            registry.emplace<Components::Transform2D>(player, glm::vec2{0.0f, 0.0f}, glm::vec2{40.0f,80.0f});
            registry.emplace<Components::Sprite2D>(player, typewriter::ResourceManager::loadSprite("assets/Player.png"));
            break;
        }
    default:
        break;
    }   
}

void GameLayer::updateState(GameState state)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        break;
    default:
        break;
    }   
}

void GameLayer::renderState(GameState state)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        {
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
        
            auto view = scene.getRegistry().view<Components::Transform2D, Components::Sprite2D>();
            for (auto [entity, transform, sprite] : view.each())
            {
                typewriter::Renderer2D::drawSprite(sprite.sprite, transform.position.x, transform.position.y, transform.size.x, transform.size.y);
            }
        }
        break;
    default:
        break;
    }   
}

void GameLayer::initAssets()
{
    level_sprite = typewriter::ResourceManager::loadSprite("assets/Level.png");
}
