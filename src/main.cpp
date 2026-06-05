#include <typewriter/Typewriter.hpp>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr const char* TITLE = "Cozy Game";

class CozyGame : public typewriter::Game {
public:
    CozyGame(std::string_view title, int screen_width, int screen_height):
        Game(title, screen_width, screen_height)
    {}

    void init() override {};
    void shutdown() override {};
};

class MainLayer : public typewriter::Layer {
public:
    void onAttach() override;
    void onRender() override;
    void onEvent(typewriter::Event& event) override {};
private:
    std::shared_ptr<typewriter::Camera> camera = nullptr;
};

void MainLayer::onAttach() {
    camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(SCREEN_WIDTH), 0.f, static_cast<float>(SCREEN_HEIGHT));
}

void MainLayer::onRender() {
    typewriter::Renderer2D::setClearColor(typewriter::Color::DarkGray);
    typewriter::Renderer2D::startScene(camera);
    // <--- Render here 
    typewriter::Renderer2D::endScene();
}

int main() {
    auto game = CozyGame(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.pushLayer(new MainLayer());
    game.start();
}
