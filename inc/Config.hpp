#pragma once

constexpr float PLAYER_INTERACT_RADIUS = 150.0f;
constexpr float FOOD_REPLENISHMENT = 0.2f; // How much hunger does good decrease
constexpr float FOOD_COOK_TIME = 5.0f;
constexpr float HUNGER_UP = 0.005f;
constexpr float HEAT_UP = 0.005f;
constexpr float HEAT_DOWN = 0.010f;
constexpr float SANITY_UP = 0.009f; 

// Game
constexpr float GAME_BOOT_TIME = 1.0f;
constexpr float OBSTACLE_SPAWN_TIME = 1.5f;
constexpr float OBSTACLE_SPAWN_POSITION_X = 625.0f;
constexpr float OBSTACLE_SPAWN_POSITION_Y = 340.0f;
constexpr float OBSTACLE_SPAWN_WIDTH = 40.0f;
constexpr float OBSTACLE_SPAWN_HEIGHT = 60.0f;
constexpr float OBSTACLE_MOVEMENT_SPEED = 200.0f;
constexpr float OBSTACLE_DESTROY_POSITION = 50.0f;
constexpr float SANITY_TIME = 1.0f;
constexpr float SANITY_GIVE = 0.03f; // How much sanity is given per timer clock

// News
constexpr float MESSAGE_WIDTH = 500.0f;
constexpr float MESSAGE_HEIGHT = 55.0f;
constexpr float MESSAGE_READ_WIDTH = 65.0f;
constexpr float MESSAGE_READ_HEIGHT = MESSAGE_HEIGHT;
constexpr float MESSAGE_POSITION_X = 150.0f;
constexpr float MESSAGE_POSITION_Y = 250.0f;
constexpr int MESSAGE_TEXT_SIZE = 32;
constexpr int MESSAGE_MAX_SHOWN = 4;
constexpr float MESSAGE_MOUSE_SCROLL = 10.0f;
constexpr float MESSAGE_TEXT_POSITION_X = 200.0f;
constexpr float MESSAGE_TEXT_POSITION_Y = 300.0f;

constexpr float INTERACT_TIME = 0.5f; // Time between interactions
constexpr float FOOD_ORDER_TIME = 3.0f; // How often can player order food
