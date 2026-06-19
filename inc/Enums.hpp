#pragma once

enum class GameState
{
    G_NONE,
    G_MENU,
    G_GAME,
    G_COMPUTER,
    G_DAY_END,
    G_GAME_OVER,
    G_DIALOGUE,
};

enum class ComputerState
{
    G_NONE,
    G_MENU,
    G_FOOD,
    G_NEWS,
    G_GAME,
};

enum class DayPhase
{
    UNDEFINED,
    MORNING,
    AFTERNOON,
    EVENING,
};

enum class WindowAnimationType
{
    NONE,
    OPEN,
    CLOSE,
};