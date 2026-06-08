#pragma once

#include <typewriter/Typewriter.hpp>

#include "Enums.hpp"

class FoodSpawner;
class InteractionSystem;
class CollisionSystem;
class InputSystem;
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
    glm::vec2 mouse_position;
    
    //------// SYSTEMS //----------//
    std::unique_ptr<InputSystem> input_system;
    std::unique_ptr<CollisionSystem> collision_system;
    std::unique_ptr<InteractionSystem> interaction_system;
    
    //------// HELPERS //----------/
    std::unique_ptr<FoodSpawner> food_spawner;
    
    //---------// OBJECTS //-------//
    typewriter::Entity player;
    
    //----------// STATE MACHINE //-----------//
    GameState current_state = GameState::G_NONE;
    void setState(GameState new_state);
    void exitState(GameState state);
    void enterState(GameState state);
    void updateState(GameState state, float deltaTime);
    void renderState(GameState state);
    void renderUIState(GameState state);
    
    //----------// ASSETS //----------//
    typewriter::Sprite level_sprite;
    void initAssets();
};
