#include "GameLayer.hpp"

#include "Components.hpp"
#include "Config.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/InteractionSystem.hpp"
#include "Systems/ObjectManager.hpp"
#include "Systems/ButtonSystem.hpp"
#include "FoodSpawner.hpp"
#include "EmailMessage.hpp"

GameLayer::GameLayer(int screen_width, int screen_height)
    : screen_width(screen_width)
    , screen_height(screen_height)
{
}

void GameLayer::onAttach()
{
    Layer::onAttach();
    
    camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width),  static_cast<float>(screen_height), 0.0f);
    ui_camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width), static_cast<float>(screen_height), 0.0f);
    
    initAssets();
    
    setState(GameState::G_GAME);
    init();
}

void GameLayer::onUpdate(float deltaTime)
{
    Layer::onUpdate(deltaTime);
    
    updateState(current_state, deltaTime);
    
    interact = false;
    mouse_down = false;
    mouse_up = false;
}

void GameLayer::onRender()
{
    Layer::onRender();
    
    typewriter::Renderer2D::setClearColor(typewriter::Color{215, 178, 143, 255}); // Color of the walls
    typewriter::Renderer2D::startScene(camera);
    
    renderState(current_state);
    
    typewriter::Renderer2D::endScene();
    
    typewriter::Renderer2D::startScene(ui_camera);
    
    renderUIState(current_state);
    
    typewriter::Renderer2D::endScene();
}

void GameLayer::onEvent(typewriter::Event& event)
{
    Layer::onEvent(event);
    
    typewriter::EventDispatcher dispatcher(event);
    dispatcher.dispatch<typewriter::KeyPressedEvent>(std::bind(&GameLayer::onKeyPressed, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::KeyReleasedEvent>(std::bind(&GameLayer::onKeyReleased, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::MouseMovedEvent>(std::bind(&GameLayer::onMouseMoved, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::MouseButtonPressedEvent>(std::bind(&GameLayer::onMousePressed, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::MouseButtonReleasedEvent>(std::bind(&GameLayer::onMouseReleased, this, std::placeholders::_1));
    dispatcher.dispatch<typewriter::MouseScrolledEvent>(std::bind(&GameLayer::onMouseScrolled, this, std::placeholders::_1));
}

bool GameLayer::onKeyPressed(typewriter::KeyPressedEvent& event)
{
    return true;
}

bool GameLayer::onKeyReleased(typewriter::KeyReleasedEvent& event)
{
    if (event.getKeyCode() == SDLK_SPACE) interact = true;
    return true;
}

bool GameLayer::onMouseMoved(typewriter::MouseMovedEvent& event)
{
    mouse_position = glm::vec2{event.getX(), event.getY()};
    return true;
}

bool GameLayer::onMousePressed(typewriter::MouseButtonPressedEvent& event)
{
    mouse_down = event.getMouseButton() == BUTTON_LEFT;
    return true;
}

bool GameLayer::onMouseReleased(typewriter::MouseButtonReleasedEvent& event)
{
    mouse_up = event.getMouseButton() == BUTTON_LEFT;
    return true;
}

bool GameLayer::onMouseScrolled(typewriter::MouseScrolledEvent& event)
{
    if (current_computer_state == ComputerState::G_NEWS && messages.size() > 4)
    {
        starting_point -= event.getYOffset();
        starting_point = glm::clamp(starting_point, 0, static_cast<int>(messages.size()) - MESSAGE_MAX_SHOWN);
    }
    return true;
}

void GameLayer::renderSystem(bool ui)
{
    auto view = scene.getRegistry().view<typewriter::Transform2D, Components::Sprite2D>();
    for (const auto& [entity, transform, sprite] : view.each())
    {
        if ((ui && sprite.ui) || (!ui && !sprite.ui))
            typewriter::Renderer2D::drawSprite(sprite.sprite, transform.position.x, transform.position.y, transform.size.x, transform.size.y);
    }
            
    auto view1 = scene.getRegistry().view<typewriter::Transform2D, Components::SpriteAnimation>();
    for (const auto& [entity, transform, sprite_animation] : view1.each())
    {
        if ((ui && sprite_animation.ui) || (!ui && !sprite_animation.ui))
            typewriter::Renderer2D::drawSprite(sprite_animation.sprite_animation[sprite_animation.frame], transform.position.x, transform.position.y, transform.size.x, transform.size.y);
    }
}

void GameLayer::init()
{
    input_system = std::make_unique<InputSystem>(scene);
    collision_system = std::make_unique<CollisionSystem>(scene);
    interaction_system = std::make_unique<InteractionSystem>(scene);
    object_manager = std::make_unique<ObjectManager>(scene);
    button_system = std::make_unique<ButtonSystem>(scene);
    
    food_spawner = std::make_unique<FoodSpawner>(scene);
    
    // Init Player
    player = scene.createEntity();
    typewriter::Registry& registry = scene.getRegistry();
    registry.emplace<Components::Player>(player, glm::vec2{150.0f, 150.0f}, glm::vec2{50.0f, 50.0f}, glm::vec2{0.0f}, 250.0f, 300.0f);
    registry.emplace<typewriter::Transform2D>(player, glm::vec2{200.0f, 200.0f}, glm::vec2{40.0f,80.0f});
    registry.emplace<Components::Sprite2D>(player, typewriter::ResourceManager::loadSprite("assets/Player.png"));
    registry.emplace<typewriter::Collision2D>(player, typewriter::AABB{{}, {}}, typewriter::CollisionType::DYNAMIC);
    registry.emplace<Components::CanInteract>(player, PLAYER_INTERACT_RADIUS);
    
    // Init colliders
    typewriter::Entity west_wall = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(west_wall, glm::vec2{0.0f, 0.0f}, glm::vec2{125.0f, 540.0f});
    registry.emplace<typewriter::Collision2D>(west_wall, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    typewriter::Entity east_wall = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(east_wall, glm::vec2{870.0f, 0.0f}, glm::vec2{90.0f, 540.0f});
    registry.emplace<typewriter::Collision2D>(east_wall, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    typewriter::Entity north_wall = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(north_wall, glm::vec2{0.0f, 0.0f}, glm::vec2{960.0f, 150.0f});
    registry.emplace<typewriter::Collision2D>(north_wall, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    typewriter::Entity south_wall = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(south_wall, glm::vec2{0.0f,540.0f}, glm::vec2{960.0f, 550.0f});
    registry.emplace<typewriter::Collision2D>(south_wall, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    // Init interactable object
    typewriter::Entity microwave = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(microwave, glm::vec2{175.0f, 125.0f}, glm::vec2{125.0f, 125.0f});
    registry.emplace<typewriter::Collision2D>(microwave, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    registry.emplace<Components::SpriteAnimation>(microwave, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Microwave.png", 20, 21)});
    registry.emplace<Components::InteractableObject>(microwave, [&registry](typewriter::Entity player, typewriter::Entity object)
    {
        auto& player_component = registry.get<Components::Player>(player);
        auto& microwave_component = registry.get<Components::Microwave>(object);
        
        if (player_component.inv_food > 0 && microwave_component.status == Components::Microwave::Status::EMPTY)
        {
            // Cook     
            player_component.inv_food -= 1;
            //std::cout << player_component.inv_food << std::endl;
            microwave_component.status = Components::Microwave::Status::COOKING;
        }
        
        if (microwave_component.status == Components::Microwave::Status::DONE)
        {
            // Eat food
            microwave_component.status = Components::Microwave::Status::EMPTY;
            player_component.hunger -= FOOD_REPLENISHMENT;
            if (player_component.hunger < 0.0f)
            {   
                player_component.hunger = 0.0f;
            }
            if (registry.all_of<Components::SpriteAnimation>(object))
            {
                registry.get<Components::SpriteAnimation>(object).frame = 0;
            }
        }
        
        // if (microwave_component.status == Components::Microwave::Status::COOKING)
        // {
        //     // Speed up the process of cooking
        //     microwave_component.heat_timer += deltaTime;
        // }
    });
    //registry.emplace<Components::Sprite2D>(microwave, typewriter::ResourceManager::loadSprite("assets/Microwave.png", typewriter::RectI(0,0,20,7)));
    registry.emplace<Components::Microwave>(microwave, FOOD_COOK_TIME);
    
    delivery_zone = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(delivery_zone, glm::vec2{170.0f, 430.0f}, glm::vec2{80.0f, 80.0f});
    registry.emplace<Components::Sprite2D>(delivery_zone, typewriter::ResourceManager::loadSprite("assets/Carpet.png"));
    registry.emplace<Components::DeliveryZone>(delivery_zone);
    
    // Window
    typewriter::Entity window = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(window, glm::vec2{400.0f, 50.0f}, glm::vec2{150.0f, 80.0f});
    registry.emplace<Components::SpriteAnimation>(window, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Window.png", 32, 22)});
    registry.emplace<Components::Window>(window, false);
    registry.emplace<Components::InteractableObject>(window, [&registry](typewriter::Entity player, typewriter::Entity object)
    {
           registry.get<Components::Window>(object).opened = !registry.get<Components::Window>(object).opened;

           registry.get<Components::SpriteAnimation>(object).frame = registry.get<Components::Window>(object).opened;
    });
    
    // Computer
    typewriter::Entity computer = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(computer, glm::vec2{750.0f, 430.0f}, glm::vec2{90.0f, 90.0f});
    registry.emplace<Components::SpriteAnimation>(computer, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Computer.png", 16, 16)});
    //registry.emplace<Components::Window>(window, false);
    registry.emplace<Components::InteractableObject>(computer, [&registry, this](typewriter::Entity player, typewriter::Entity object)
    {
        setState(GameState::G_COMPUTER);
    });
    
    // Game manager
    
    game_manager = std::make_unique<GameManager>();
    
    // Test
    
    food_spawner->spawnFood(delivery_zone);
    food_spawner->spawnFood(delivery_zone);
    food_spawner->spawnFood(delivery_zone);
    food_spawner->spawnFood(delivery_zone);
    
    // Test messages
    EmailMessage message{"Introduction letter.", "This is an introduction letter."};
    EmailMessage message1{"Test letter.", "SPAM"};
    EmailMessage message2{"Last Message", "Last message."};
    messages.push_back(message);
    messages.push_back(message1);
    messages.push_back(message2);
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
    case GameState::G_COMPUTER:
        exitComputerState(current_computer_state);
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
        break;
    case GameState::G_COMPUTER:
        enterComputerState(current_computer_state);
        break;
    default:
        break;
    }   
}

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
            interaction_system->update(deltaTime, interact);
            object_manager->update(deltaTime);
            button_system->update(deltaTime, mouse_position, mouse_down, mouse_up);
        
            // Make camera follow the player
            const auto& [position, size] = scene.getRegistry().get<typewriter::Transform2D>(player);
            glm::vec2 half_screen{screen_width * 0.5f, screen_height * 0.5f};
            glm::vec2 current_camera_centre = glm::vec2{camera->getPosition().x, camera->getPosition().y} + half_screen;
            glm::vec2 to_player = position - current_camera_centre;
            float distance = glm::length(to_player);
            
            float move_radius = 100.0f;
            
            if (distance > move_radius)
            {
                float excess_distance = distance - move_radius;
                glm::vec2 direction = to_player / distance;
                
                glm::vec2 new_camera_centre = current_camera_centre + (direction * excess_distance);
                
                camera->setPosition(new_camera_centre - half_screen);
            }
            
            // Update stats
            scene.getRegistry().get<Components::Player>(player).hunger += HUNGER_UP * deltaTime;
            scene.getRegistry().get<Components::Player>(player).sanity += SANITY_UP * deltaTime;
            if (scene.getRegistry().get<Components::Player>(player).sanity < 0.0f)
                scene.getRegistry().get<Components::Player>(player).sanity = 0.0f;
            
            food_order_timer += deltaTime;
            
        }
        break;
    case GameState::G_COMPUTER:
        // Update stats
        scene.getRegistry().get<Components::Player>(player).hunger += HUNGER_UP * deltaTime;
        scene.getRegistry().get<Components::Player>(player).sanity += SANITY_UP * deltaTime;
        if (scene.getRegistry().get<Components::Player>(player).sanity < 0.0f)
            scene.getRegistry().get<Components::Player>(player).sanity = 0.0f;
        food_order_timer += deltaTime;
        
        button_system->update(deltaTime, mouse_position, mouse_down, mouse_up);
        
        updateComputerState(current_computer_state, deltaTime);
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
        
            renderSystem(false);
            
            interaction_system->render();
        }
        break;
    case GameState::G_COMPUTER:
        {
            // Render main level and game objects
            // PC UI is rendered in renderUIState
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
            
            renderSystem(false);
        }
        break;
    default:
        break;
    }   
}

void GameLayer::renderUIState(GameState state)
{
    switch (state)
    {
    case GameState::G_NONE:
        break;
    case GameState::G_MENU:
        break;
    case GameState::G_GAME:
        {
            renderStats(); 
        }
        break;
    case GameState::G_COMPUTER:
        {
            typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Computer_Screen.png"), 50.0f, 25.0f, screen_width - 100.0f, screen_height - 50.0f);
            
            renderStats();
            
            renderSystem(true);
        
            renderComputerState(current_computer_state);
    }
        break;
    default:
        break;
    }   
}

void GameLayer::renderStats()
{
    auto& player_component = scene.getRegistry().get<Components::Player>(player);
    auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 35);
    auto text = typewriter::ResourceManager::loadText(font, std::format("Hunger: {}", player_component.hunger));
    typewriter::Renderer2D::drawText(text.get(), 0.0f, 0.0f);
            
    auto text1 = typewriter::ResourceManager::loadText(font, std::format("Food: {}", player_component.inv_food));
    typewriter::Renderer2D::drawText(text1.get(), 0.0f, 50.0f);
            
    auto text2 = typewriter::ResourceManager::loadText(font, std::format("Temperature: {}", player_component.temperature));
    typewriter::Renderer2D::drawText(text2.get(), 0.0f, 100.0f);
            
    auto text3 = typewriter::ResourceManager::loadText(font, std::format("Sanity: {}", player_component.sanity));
    typewriter::Renderer2D::drawText(text3.get(), 0.0f, 150.0f);
}

void GameLayer::initAssets()
{
    level_sprite = typewriter::ResourceManager::loadSprite("assets/Level.png");
}
