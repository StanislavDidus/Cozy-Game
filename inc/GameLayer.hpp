#pragma once

#include <typewriter/Typewriter.hpp>

#include "Enums.hpp"
#include "Config.hpp"

class ButtonSystem;
class FoodSpawner;
class InteractionSystem;
class CollisionSystem;
class InputSystem;
class ObjectManager;
class Player;

class GameLayer : public typewriter::Layer
{
public:
    GameLayer(int screen_width, int screen_height);
    void onAttach() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onEvent(typewriter::Event& event) override;
private:
    std::shared_ptr<typewriter::Camera> camera = nullptr;
    std::shared_ptr<typewriter::Camera> ui_camera = nullptr;
    typewriter::Scene scene;
    int screen_width;
    int screen_height;
    
    //--------// INPUT //----------//
    bool onKeyPressed(typewriter::KeyPressedEvent& event);
    bool onKeyReleased(typewriter::KeyReleasedEvent& event);
    bool onMouseMoved(typewriter::MouseMovedEvent& event);
    bool onMousePressed(typewriter::MouseButtonPressedEvent& event);
    bool onMouseReleased(typewriter::MouseButtonReleasedEvent& event);
    glm::vec2 mouse_position;
    bool mouse_down = false;
    bool mouse_up = false;
    
    //------// SYSTEMS //----------//
    std::unique_ptr<InputSystem> input_system;
    std::unique_ptr<CollisionSystem> collision_system;
    std::unique_ptr<InteractionSystem> interaction_system;
    std::unique_ptr<ObjectManager> object_manager;
    std::unique_ptr<ButtonSystem> button_system;
    
    //------// HELPERS //----------/
    std::unique_ptr<FoodSpawner> food_spawner;
    
    //---------// OBJECTS //-------//
    typewriter::Entity player = entt::null;
    typewriter::Entity delivery_zone = entt::null;
    
    //----------// STATE MACHINE //-----------//
    void init();
    GameState current_state = GameState::G_NONE;
    void setState(GameState new_state);
    void exitState(GameState state);
    void enterState(GameState state);
    void updateState(GameState state, float deltaTime);
    void renderState(GameState state);
    void renderUIState(GameState state);
    
    //----------// UI //-------------//
    void renderStats();
    //float hunger = 0.0f;
    float hunger_speed = 1.0f;
    
    //------// COMPUTER SCREEN //-------//
    typewriter::Entity exit_button = entt::null;
    typewriter::Entity food_button = entt::null;
    typewriter::Entity game_button = entt::null;
    typewriter::Entity food_order_button = entt::null;
    typewriter::Entity return_button = entt::null;
    ComputerState current_computer_state = ComputerState::G_MENU;
    
    float game_boot_timer = 0.0f;
    
    //-------// PC APPS //----------//
    float food_order_timer = FOOD_ORDER_TIME;
    
    void setComputerState(ComputerState state);
    void enterComputerState(ComputerState state);
    void exitComputerState(ComputerState state);
    void updateComputerState(ComputerState state, float deltaTime);
    void renderComputerState(ComputerState state);
    
    //----------// ASSETS //----------//
    typewriter::Sprite level_sprite;
    void initAssets();
};