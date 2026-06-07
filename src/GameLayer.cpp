#include "GameLayer.hpp"

#include "Components.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"

GameLayer::GameLayer(int screen_width, int screen_height)
    : screen_width(screen_width)
    , screen_height(screen_height)
{
}

void GameLayer::onAttach()
{
    Layer::onAttach();
    
    camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width),  static_cast<float>(screen_height), 0.0f);
    ui_camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width), 0.f, static_cast<float>(screen_height));
    
    initAssets();
    
    setState(GameState::G_GAME);
}

void GameLayer::onUpdate(float deltaTime)
{
    Layer::onUpdate(deltaTime);
    
    updateState(current_state, deltaTime);
}

void GameLayer::onRender()
{
    Layer::onRender();
    
    typewriter::Renderer2D::setClearColor(typewriter::Color{215, 178, 143, 255}); // Color of the walls
    typewriter::Renderer2D::startScene(camera);
    
    renderState(current_state);
    
    typewriter::Renderer2D::endScene();
}

void GameLayer::onEvent(typewriter::Event& event)
{
    Layer::onEvent(event);
    
    typewriter::EventDispatcher dispatcher(event);
    dispatcher.dispatch<typewriter::KeyPressedEvent>(std::bind(&GameLayer::onKeyPressed, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::KeyReleasedEvent>(std::bind(&GameLayer::onKeyReleased, this, std::placeholders::_1));
}

bool GameLayer::onKeyPressed(typewriter::KeyPressedEvent& event)
{
    return true;
}

bool GameLayer::onKeyReleased(typewriter::KeyReleasedEvent& event)
{
    return true;
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
            input_system = std::make_unique<InputSystem>(scene);
            collision_system = std::make_unique<CollisionSystem>(scene);
            
            // Init Player
            player = scene.createEntity();
            typewriter::Registry& registry = scene.getRegistry();
            registry.emplace<Components::Player>(player, glm::vec2{150.0f, 150.0f}, glm::vec2{50.0f, 50.0f}, glm::vec2{0.0f}, 210.0f, 250.0f);
            registry.emplace<Components::Transform2D>(player, glm::vec2{200.0f, 200.0f}, glm::vec2{40.0f,80.0f});
            registry.emplace<Components::Sprite2D>(player, typewriter::ResourceManager::loadSprite("assets/Player.png"));
            registry.emplace<Components::Collider>(player, glm::vec2{0.0f}, glm::vec2{0.0f});
            
            // Init colliders
            typewriter::Entity west_wall = scene.createEntity();
            registry.emplace<Components::Transform2D>(west_wall, glm::vec2{0.0f, 0.0f}, glm::vec2{125.0f, 540.0f});
            registry.emplace<Components::Collider>(west_wall, glm::vec2{0.0f}, glm::vec2{0.0f});
            
            typewriter::Entity east_wall = scene.createEntity();
            registry.emplace<Components::Transform2D>(east_wall, glm::vec2{870.0f, 0.0f}, glm::vec2{90.0f, 540.0f});
            registry.emplace<Components::Collider>(east_wall, glm::vec2{0.0f}, glm::vec2{0.0f});
            
            typewriter::Entity north_wall = scene.createEntity();
            registry.emplace<Components::Transform2D>(north_wall, glm::vec2{0.0f, 0.0f}, glm::vec2{960.0f, 150.0f});
            registry.emplace<Components::Collider>(north_wall, glm::vec2{0.0f}, glm::vec2{0.0f});
            
            typewriter::Entity south_wall = scene.createEntity();
            registry.emplace<Components::Transform2D>(south_wall, glm::vec2{0.0f,540.0f}, glm::vec2{960.0f, 550.0f});
            registry.emplace<Components::Collider>(south_wall, glm::vec2{0.0f}, glm::vec2{0.0f});
            break;
        }
    default:
        break;
    }   
}

float camera_x = 0.0f;
void GameLayer::updateState(GameState state, float deltaTime)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        {
            input_system->update(deltaTime);
            collision_system->update(deltaTime);
        
            const Components::Transform2D& player_transform = scene.getRegistry().get<Components::Transform2D>(player);
            camera->setPosition(player_transform.position - glm::vec2{screen_width, screen_height} * 0.5f);
            //camera->setPosition(glm::vec2{camera_x, 0.0f});
            //camera->setPosition({glm::vec2{1.0f, 0.0f}});
            //camera_x += deltaTime * 0.05f;
            //std::cout << camera_x << std::endl;
        }
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
