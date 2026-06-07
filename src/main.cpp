#include <typewriter/Typewriter.hpp>

#include "GameLayer.hpp"

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;
constexpr const char* TITLE = "Cozy Game";

class CozyGame : public typewriter::Game {
public:
    CozyGame(std::string_view title, int screen_width, int screen_height):
        Game(title, screen_width, screen_height)
    {}

    void init() override {}
    void shutdown() override {}
};

int main() {
    auto game = CozyGame(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.pushLayer(new GameLayer(SCREEN_WIDTH, SCREEN_HEIGHT));
    game.start();
}
