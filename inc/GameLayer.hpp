#pragma once

#include <typewriter/Typewriter.hpp>

#include "Enums.hpp"
#include "Config.hpp"
#include "EmailMessage.hpp"
#include "GameManager.hpp"

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
    bool onMouseScrolled(typewriter::MouseScrolledEvent& event);
    glm::vec2 mouse_position;
    bool interact = false;
    bool mouse_down = false;
    bool mouse_up = false;
    
    //------// SYSTEMS //----------//
    void renderSystem(bool ui);
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
    typewriter::Entity window = entt::null;
    
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
    
    //-------// GAME MENU //----------//
    typewriter::Entity start_button = entt::null;
    typewriter::Entity exit_menu_button = entt::null;
    
    //------// COMPUTER SCREEN //-------//
    typewriter::Entity exit_button = entt::null;
    typewriter::Entity food_button = entt::null;
    typewriter::Entity game_button = entt::null;
    typewriter::Entity news_button = entt::null;
    typewriter::Entity food_order_button = entt::null;
    typewriter::Entity return_button = entt::null;
    ComputerState current_computer_state = ComputerState::G_MENU;
    
    //------// PC GAME //--------//
    float game_boot_timer = 0.0f;
    float player_y_pos = 0.0f;
    float player_velocity_y = 0.0f;
    float player_jump_force = 400.0f;
    float player_gravity = 700.0f;
    std::vector<typewriter::AABB> obstacles;
    float obstacle_spawn_timer = 0.0f;
    float sanity_timer = 0.0f;
    
    //-------// PC APPS //----------//
    float food_order_timer = FOOD_ORDER_TIME;
    
    //--------// PC NEWS //---------//
    std::vector<EmailMessage> messages;
    int starting_point = 0;
    std::optional<EmailMessage> reading_message = std::nullopt;
    
    //---------// GAME LOOP //-----------//
    void initGameStory();
    void checkDayEnd();
    void renderDayInfo();
    void checkLoseCondition();
    std::unique_ptr<GameManager> game_manager;
    
    //-------// DAY END TRANSITION //-----//
    float day_end_timer = 0.0f;
    int displayed_hint = 0;
    float day_transit_timer = 0.0f;
    
    void setComputerState(ComputerState state);
    void enterComputerState(ComputerState state);
    void exitComputerState(ComputerState state);
    void updateComputerState(ComputerState state, float deltaTime);
    void renderComputerState(ComputerState state);
    
    //----------// ASSETS //----------//
    typewriter::Sprite level_sprite;
    void initAssets();
};