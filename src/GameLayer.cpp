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
#include "Dialogue.hpp"
#include "imgui_internal.h"
#include "SoundManager.hpp"
#include "Audio/Device.hpp"
#include "Audio/Sound.hpp"
#include "glm/gtc/random.hpp"
#include "typewriter/Typewriter.hpp"

static std::array<std::string, 4> hints = 
    {
        "Try to think of your plans ahead."
        "You cannot have more then three food packages by your door. EAT them.",
        "Always check your laptop for new story details.",
        "Try to have fun."
};

GameLayer::GameLayer(int screen_width, int screen_height)
    : screen_width(screen_width)
    , screen_height(screen_height)
{
}

void GameLayer::showDialogue(const std::string& text)
{
    std::cout << text << std::endl;
    
    dialogues.emplace_back(Dialogue{text});
}

void GameLayer::updateDialogues(float deltaTime)
{
   if (!dialogues.empty())
   {
       auto& dialogue = dialogues[0];
       
       dialogue.update(deltaTime);
       
       setState(GameState::G_DIALOGUE);
       
       if (interact)
       {
           if (!dialogue.isFinished()) 
               dialogue.skipDialogue();
           else
           {
               dialogues.erase(dialogues.begin());
           }
       }
   }
}

void GameLayer::renderDialogues()
{
   if (!dialogues.empty())
   {
       typewriter::Renderer2D::drawRectangle(DIALOGUE_WINDOW_POS_X, DIALOGUE_WINDOW_POS_Y, DIALOGUE_WINDOW_SIZE_X, DIALOGUE_WINDOW_SIZE_Y, typewriter::Color::DarkGrey);
       
       auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", DIALOGUE_TEXT_SIZE);
       auto text = typewriter::ResourceManager::loadText(font, dialogues[0].getCurrentText());
       text->setWrapWidth(DIALOGUE_TEXT_WRAP_WIDTH);
       
       typewriter::Renderer2D::drawText(text.get(), DIALOGUE_TEXT_POS_X, DIALOGUE_TEXT_POS_Y);
   }
}

void GameLayer::initGameStory()
{
    std::vector<Day> days;
    
    // Day one
    {
        Day day;
        day.hunger_up = 0.004f;
        day.sanity_up = 0.005f;
        day.heat_up = 0.000f;
        
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(false);
    
            food_spawner->spawnFood(delivery_zone);
            food_spawner->spawnFood(delivery_zone);
            door_light_anim->reset();
            door_dark_anim->reset();
     can_order_food = false;
                }, 0.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("We were going to hang out with Marks today. We are to meet in the park in 3 a.m.");
        }, 3.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("I need to eat something.");
            showDialogue("Ooh the food is already delivered.");
        }, 6.0f});
        events.push_back(EventPoint{[this]
        {
            game_manager->stopProgress(true);
        }, 7.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("Its time for me to go.");
        }, 30.0f});
        events.push_back(EventPoint{[this]
        {
            SoundManager::get().getSound("Siren").replay();
        }, 32.0f});
        events.push_back(EventPoint{[this]
        {
            // Play siren sound
            showDialogue("What is happening! I should check out the news");
            
            EmailMessage message{"The Government", 
                "Attention citizens of the capital, We come to you with an important message from our dear leader.\n"
                " In the last few days several people have come down with a severe case of \"sun fever\"."
                " A highly lethal and highly contagious disease. We believe coronal mass ejections from the sun are to blame."
                " To prevent any further spreading of the disease we are putting the area in immediate lockdown."
                " Anyone outside of the designated necessary jobs leaving their house will be severely punished."
                " We understand this is a lot to process, but we expect your full **compliance**.\n"
                " Live long and die strong,\n The government"};
            message.close_function = [this]
            {
                showDialogue("What is happening? What do they mean that the sun is dangerous.");
                showDialogue("I can't even leave my house now.");
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
                }, 35.0f});
        events.push_back(EventPoint{[this]
        {
            SoundManager::get().getSound("Siren").stop();
        }, 37.0f});
        events.push_back(EventPoint{[this]
        {
            food_spawner->spawnFood(delivery_zone);
            showDialogue("What is this box in my doorway. Is this food?");
            EmailMessage message{"To all residents of this complex", 
                "Our dear leader has given us a gracious supply of food and water.\n"
                " Enough to divide among each resident."
                " With it they have shared with us a set of rules to get us through these trying times: First of all, you must not consume any other food except for the one delivered at your doorstep by the government."
                " The special packaging protects the meal from the sun."
                " Second of all, you must make sure as little sunlight as possible enters your apartment."
                " Close the curtains and block your windows."
                " Third of all, do not look at the sun."
                " It has been shown to induce madness in those that see it.\n"
                " We have full confidence in your ability to follow these simple rules.\n"
                " Live long and die strong,\n The landlord (This message has been approved by our dear leader)"};
            message.close_function = [this]
            {
            showDialogue("I guess I will have to eat it. I have no other choice.");
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 70.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("I am so tired. There is nothing for me to do now. I will probably go to sleep.");
            //End day
        }, 110.0f});
        days.push_back(day);
    }
    
    // Day two
    {
        Day day;
        day.hunger_up = 0.0065f;
        day.sanity_up = 0.006f;
        day.heat_up = 0.010f;
        
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(true);
            can_order_food = false;
        }, 0.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("There is no food. I am starving.");
        }, 3.0f});
        events.push_back(EventPoint{[this]
        {
            EmailMessage message{"Dear fellows", 
             "Tired of the governments rations? Want more than the taste of cardboard in your mouth?"
             " Say no more! The quickest delivery in the whole capital, reasonable prices.\n"
             " Beef, pasta, soups, anything your stomach could ever want.\n"
             " At our shop we make sure no citizen leaves hungry!\n"
             " So don't make that old grumbler wait! Order now!"};
            message.close_function = [this]
            {
                showDialogue("Now I can order food from their webside. Lets try it.");
                can_order_food = true;
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
            
        }, 12.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("Umm. It is delicious. It is much better than what the government gave us yesterday."
                         " I wonder why. The food so good cant be bad for anyone.");
        }, 45.0f});
        events.push_back(EventPoint{[this]
        {
            EmailMessage message{"I need help", 
                "My dear son."
                " I don't know what is happening. Some man in black suits came to our home and took your father, they also said that the sun is dangerous."
                " They are taking every man in our neighborhood."
                " I hope you are good son."
                " I am so worried about him.\n"
                " Why would they take him for and where?\n Diana"};
            message.close_function = [this]
            {
                showDialogue("Omg what is going on there. Where did they take my father.");
                showDialogue("Lets hope the government will explain it.");
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 55.0f});
        events.push_back(EventPoint{[this]
        {
            EmailMessage message{"Important", 
                "I shouldn't 122215 you now but 512078 me out Because I think I 41242 what is 4081."
                " Do you 413444 the meteorite that 901 here 5 years ago."
                " The are 1667098.\n"
                " When I was an engineer I was 16009 on a 778 and I was tOld that the government was 348951 big."
                " I 9015678 the details but it is 09986 dangerous."
                " This 000 53162 is a 412677 The sun is 088054 dangerous."
                " I aM 12224 the sun 14468 dangerous."
                " I think I am going to go 721124 now to see what is happening there.\n"
                " I will Be 45124.\n Marks"};
            message.close_function = [this]
            {
                showDialogue("What could he possibly mean by that? I remember that he was an engineer on some factory long ago. He never let anyone go there.");
                showDialogue("He mentioned the meteorite. I just moved here when it fell. The government evacuated the area and built a factory on its place. They said it was for safety reasons.");
                showDialogue("Why would he go outside? I am worried about him. If he gets caught they will... Its better not to thing about it.");
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 85.0f});
        events.push_back(EventPoint{[this]
        {
        }, 110.0f});
        events.push_back(EventPoint{[this]
        {
            // Play gunshot sound
            SoundManager::get().getSound("Shot1").replay();
                showDialogue("A gunshot? What the hell is this supposed to mean?");
        }, 115.0f});
        days.push_back(day);
    }
    // Day three
    {
        Day day;
        
        day.hunger_up = 0.008f;
        day.sanity_up = 0.007f;
        day.heat_up = 0.012f;
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(true);
        }, 0.0f});
        events.push_back(EventPoint{[this]
        {
            // Play scream sound
            SoundManager::get().getSound("Scream1").replay();
        }, 3.0f});
        events.push_back(EventPoint{[this]
        {
            EmailMessage message{"I don't feel good\n",
                            "You know I always had bad health but they don't allow me to open a window.\n"
                             " I can't handle it anymore."
                             " Also the food they give us.\n"
                             " My doctor's interest took me and I looked at what is inside it and I found that there are special medicines that influence muscle and brain work."
                             " Why would they even add it? I have heard there is going to be a protest tomorrow."
                             " I am worried of what might happen.\n I love you son, \nDiana, your mother"};
            message.close_function = [this]
            {
                
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 20.0f});
        events.push_back(EventPoint{[this]
        {
            // Play scream sound
            SoundManager::get().getSound("Shot2").replay();
        }, 30.0f});
        events.push_back(EventPoint{[this]
        {
            EmailMessage message{"WE WILL TOLERATE THIS NO LONGER", "We demand answers from the government.\n"
                                        " They have kept us locked in our homes for days."
                                        " They tried to make us eat some shitty food but all in vain."
                                        " They say the sun is dangerous but we will have none of it."
                                        " If you want to be a free man join us tomorrow morning on the main street.\n"
                                        " We will see what they are hiding from us on the streets!"};
            message.close_function = [this]
            {
                showDialogue(" A protest. But they said they would kill anyone who dares to go outside. ");
                showDialogue(" I live too far away from the main street there is no way I can get there without being noticed. Lets hope for the best.");
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 75.0f});
        
        events.push_back(EventPoint{[this]
        {
            // Play scream sound
            SoundManager::get().getSound("Scream2").replay();
        }, 90.0f});
        
        events.push_back(EventPoint{ [this]
        {
           showDialogue("I am so worried about tomorrow day. Father, Marks, protest. They have to give answers");
        }, 110.0f});
        days.push_back(day);
    } 
    // Day four
    {
        Day day;
        
        day.hunger_up = 0.01f;
        day.sanity_up = 0.008f;
        day.heat_up = 0.03f;
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(true);
            
            EmailMessage message{"Son", "Hi son."
                                        " Father is back! I decided to go to that protest but as soon as I arrived to the main street the police stopped everyone."
                                        " Before anything bad could happen they brought all people they took earlier."
                                        " They brought your father as well."
                                        " He wasn't very happen."
                                        " He doesn't want to talk about it."
                                        " He says we should all forget it."
                                        " They also said that the sun is not dangerous anymore and we can go back to our normal lives."
                                        " Oh I am so happy son."};
            message.close_function = [this]
            {
                showDialogue("Father is alive! I am so happy. But why would they do that. And where is Marks.");
                showDialogue("I still feel that something isn't right.");
                
                EmailMessage msg{"Thank you!", "Thank you for playing our game.\n"
                                               "You have finished the main story but if you want you can still continue playing in a free mode!\n"
                                               "We hope you enjoyed this game and please leave a comment on the itch.io page.\n"
                                               "Kind regards,\n"
                                                "Infinite Monkey Theorem Team"
                };
                SoundManager::get().getSound("Laptop-message").replay();
                messages.push_back(msg);
            };
            SoundManager::get().getSound("Laptop-message").replay();
            messages.push_back(message);
        }, 3.0f});
        days.push_back(day);
    }
    
    game_manager->initDays(days);
    game_manager->setGameDay(0);
}

void GameLayer::checkDayEnd()
{
    // See if day has ended
    if (game_manager->isDayEnded())
    {
        setState(GameState::G_DAY_END);
    }
}

void GameLayer::renderDayInfo()
{
    auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 35);
    std::string day_number = "Day " + std::to_string(game_manager->getCurrentday() + 1);
    
    auto text = typewriter::ResourceManager::loadText(font, day_number);
    typewriter::Renderer2D::drawText(text.get(), 400.0f, 0.0f);
    
    DayPhase day_phase = game_manager->getDayPhase();
    
    std::string day_phase_text;
    if (day_phase == DayPhase::MORNING)
    {
       day_phase_text = "MORNING";
    }
    if (day_phase == DayPhase::AFTERNOON)
    {
        day_phase_text = "AFTERNOON";
    }
    if (day_phase == DayPhase::EVENING)
    {
        day_phase_text = "EVENING";
    }
    
    auto text1 = typewriter::ResourceManager::loadText(font, day_phase_text);
    typewriter::Renderer2D::drawText(text1.get(), 500.0f, 0.0f);
}

void GameLayer::checkLoseCondition()
{
    auto& player_stats = scene.getRegistry().get<Components::Player>(player);
    if (game_manager->isDroneAttack() ||
        player_stats.hunger >= 1.0f ||
        player_stats.temperature >= 1.0f ||
        player_stats.sanity >= 1.0f)
    {
        // Play death animation and restart the day
        setState(GameState::G_GAME_OVER);
    }
}

void GameLayer::onAttach()
{
    Layer::onAttach();
    
    // Init rand
    srand(time(NULL));
    
    camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width),  static_cast<float>(screen_height), 0.0f);
    ui_camera = std::make_shared<typewriter::Camera>(0.f, static_cast<float>(screen_width), static_cast<float>(screen_height), 0.0f);
    
    initAssets();
    
    input_system = std::make_unique<InputSystem>(scene);
    collision_system = std::make_unique<CollisionSystem>(scene);
    interaction_system = std::make_unique<InteractionSystem>(scene);
    object_manager = std::make_unique<ObjectManager>(scene);
    button_system = std::make_unique<ButtonSystem>(scene);
    food_spawner = std::make_unique<FoodSpawner>(scene);
    
    setState(GameState::G_MENU);
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
    
    typewriter::Renderer2D::setClearColor(typewriter::Color{0}); // Color of the walls
    typewriter::Renderer2D::startScene(camera);
    
    typewriter::Renderer2D::drawRectangle(-1000.0f, -1000.0f, 10000.0f, 10000.0f, typewriter::Color{215,178,143,day});
    typewriter::Renderer2D::drawRectangle(-1000.0f, -1000.0f, 10000.0f, 10000.0f, typewriter::Color{93,37,71,night});
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
    mouse_position = typewriter::Window::clientToImage(event.getX(), event.getY(), static_cast<float>(screen_width), static_cast<float>(screen_height));
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

void GameLayer::initPlayerAnimations()
{
    // Walking animations
    {
        std::vector<int> frames = {0,1,2,3,4,5,6,7};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerWalking.png", 32, 48);
        down_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 8.0f, true, frames);
    }
    {
        
        std::vector<int> frames = {8,9,10,11,12,13,14,15};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerWalking.png", 32, 48);
        top_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 8.0f, true, frames);
    }
    {
        std::vector<int> frames = {16,17,18,19,20,21,22,23};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerWalking.png", 32, 48);
        right_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 8.0f, true, frames);
    }
    {
        
        std::vector<int> frames = {24,25,26,27,28,29,30,31};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerWalking.png", 32, 48);
        left_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 8.0f, true, frames);
    }
    
    // Idle animations
    {
        std::vector<int> frames = {0,1,2,3,4,5};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerIdle.png", 32, 48);
        down_idle = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 6.0f, true, frames);
    }
    {
        std::vector<int> frames = {6,7,8,9,10,11};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerIdle.png", 32, 48);
        top_idle = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 6.0f, true, frames);
    }
    {
        std::vector<int> frames = {12,13,14,15,16,17};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerIdle.png", 32, 48);
        right_idle = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 6.0f, true, frames);
    }
    {
        std::vector<int> frames = {18,19,20,21,22,23};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/PlayerIdle.png", 32, 48);
        left_idle = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 6.0f, true, frames);
    }
}

void GameLayer::playPlayerSounds()
{
    auto& registry = scene.getRegistry();
    auto& player_component = registry.get<Components::Player>(player);
    
    if (glm::length(player_component.velocity) >= 10.0f)
    {
        SoundManager::get().getSound("Walking").play();
    }
    SoundManager::get().getSound("Breathing").play();
}

void GameLayer::updatePlayerAnimations(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto& player_component = registry.get<Components::Player>(player);
    auto& render_component = registry.get<Components::Sprite2D>(player);
    
    float idle_movement_length = 10.0f;
    
    if (player_component.velocity.x < -idle_movement_length)
    {
        left_movement->update(deltaTime);
        render_component.sprite = *left_movement.get();
        player_component.last_direction = Components::Player::Direction::G_LEFT;
    }
    else if (player_component.velocity.x > idle_movement_length)
    {
        
        right_movement->update(deltaTime);
        render_component.sprite = *right_movement.get();
        player_component.last_direction = Components::Player::Direction::G_RIGHT;
    }
    else
    {
        //Idle
        if (player_component.last_direction == Components::Player::Direction::G_LEFT)
        {
            left_idle->update(deltaTime);
            render_component.sprite = *left_idle.get();
        }
        else if (player_component.last_direction == Components::Player::Direction::G_RIGHT)
        {
            
            right_idle->update(deltaTime);
            render_component.sprite = *right_idle.get();
        }
    }
    
    
    if (player_component.velocity.y < -idle_movement_length)
    {
        top_movement->update(deltaTime);
        render_component.sprite = *top_movement.get();
        player_component.last_direction = Components::Player::Direction::G_UP;
    }
    else if (player_component.velocity.y > idle_movement_length)
    {
        down_movement->update(deltaTime);
        render_component.sprite = *down_movement.get();
        player_component.last_direction = Components::Player::Direction::G_DOWN;
    }
    else
    {
        //Idle
        if (player_component.last_direction == Components::Player::Direction::G_UP)
        {
            top_idle->update(deltaTime);
            render_component.sprite = *top_idle.get();
        }
        else if (player_component.last_direction == Components::Player::Direction::G_DOWN)
        {
        
            down_idle->update(deltaTime);
            render_component.sprite = *down_idle.get();
        }
    }
}

void GameLayer::renderSystem(bool ui)
{
    using SpriteRect = std::pair<typewriter::Sprite, typewriter::RectF>;
    using RenderCommand = std::pair<int, SpriteRect>;
    std::vector<RenderCommand> commands;
    
    auto view = scene.getRegistry().view<typewriter::Transform2D, Components::Sprite2D>();
    for (const auto& [entity, transform, sprite] : view.each())
    {
        if ((ui && sprite.ui) || (!ui && !sprite.ui))
            commands.push_back(RenderCommand{sprite.layer, SpriteRect{sprite.sprite, {transform.position.x, transform.position.y, transform.size.x, transform.size.y}}});
    }
            
    auto view1 = scene.getRegistry().view<typewriter::Transform2D, Components::SpriteAnimation>();
    for (const auto& [entity, transform, sprite_animation] : view1.each())
    {
        if ((ui && sprite_animation.ui) || (!ui && !sprite_animation.ui))
            commands.push_back(RenderCommand{sprite_animation.layer, SpriteRect{sprite_animation.sprite_animation[sprite_animation.frame], {transform.position.x, transform.position.y, transform.size.x, transform.size.y}}});
    }

    std::stable_sort(commands.begin(), commands.end(), [](const auto& a, const auto& b) {
       return a.first < b.first;
   });   
    
    for (const auto& sprite : commands | std::views::values)
    {
        typewriter::Renderer2D::drawSprite(sprite.first, sprite.second.left, sprite.second.top, sprite.second.width, sprite.second.height);
    }
}

void GameLayer::init()
{
    // Init Player
    player = scene.createEntity();
    game_manager->setPlayer(player);
    typewriter::Registry& registry = scene.getRegistry();
    registry.emplace<Components::Player>(player, glm::vec2{150.0f, 150.0f}, glm::vec2{50.0f, 50.0f}, glm::vec2{0.0f}, 250.0f, 300.0f);

    registry.emplace<typewriter::Transform2D>(player, glm::vec2{200.0f, 325.0f}, glm::vec2{60.0f,93.0f});
    registry.emplace<Components::Sprite2D>(player, typewriter::ResourceManager::loadSprite("assets/PlayerIdle.png", typewriter::RectI{0,0,32,48}), PLAYER_LAYER);
    registry.emplace<typewriter::Collision2D>(player, typewriter::AABB{{10.0f, 60.0f}, {-10.0f, 0.0f}}, typewriter::CollisionType::DYNAMIC);
    registry.emplace<Components::CanInteract>(player, PLAYER_INTERACT_RADIUS);
    
    initColliders();
    initObjectsAnimation();
    
    // Init interactable object
    microwave = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(microwave, glm::vec2{650.0f, 420.0f}, glm::vec2{100.0f, 100.0f});
    registry.emplace<typewriter::Collision2D>(microwave, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    //registry.emplace<Components::Sprite2D>(microwave, Components::Sprite2D{typewriter::ResourceManager::loadSprite("assets/MicrowaveLight.png", typewriter::RectI{288,51,48,51})});
    registry.emplace<Components::InteractableObject>(microwave, [&registry, this](typewriter::Entity player, typewriter::Entity object)
    {
        auto& player_component = registry.get<Components::Player>(player);
        auto& microwave_component = registry.get<Components::Microwave>(object);
        
        if (player_component.inv_food > 0 && microwave_component.status == Components::Microwave::MicrowaveStatus::EMPTY)
        {
            // Cook     
            player_component.inv_food -= 1;
            //std::cout << player_component.inv_food << std::endl;
            microwave_component.status = Components::Microwave::MicrowaveStatus::COOKING;
            
            SoundManager::get().getSound("Microwave-started").play();
            SoundManager::get().getSound("Microwave-cooking").play();
            
            microwave_close_light->reset();
            microwave_close_dark->reset();
        }
        
        if (microwave_component.status == Components::Microwave::MicrowaveStatus::DONE)
        {
            // Eat food
            microwave_component.status = Components::Microwave::MicrowaveStatus::EMPTY;
            player_component.hunger -= FOOD_REPLENISHMENT;
            is_food_eaten = true;
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
    
    door = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(door, glm::vec2{559.0f, 26.0f}, glm::vec2{77.0f, 163.0f});
    //registry.emplace<Components::Sprite2D>(door, typewriter::ResourceManager::loadSprite("assets/DoorLight.png", typewriter::RectI{0,0, 32, 67}));
    
    delivery_zone = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(delivery_zone, glm::vec2{567.0f, 176.0f}, glm::vec2{77.0f, 163.0f});
    registry.emplace<Components::DeliveryZone>(delivery_zone);
    
    initWindowAnimations();
    // Window view    
    window_view = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(window_view, glm::vec2{120.0f, 150.0f}, glm::vec2{115.0f, 60.0f});
    registry.emplace<Components::Sprite2D>(window_view, typewriter::ResourceManager::loadSprite("assets/Windowview.png", typewriter::RectI{0,0,46,25}));
    
    // Window
    window = scene.createEntity();
    game_manager->setWindow(window);
    registry.emplace<typewriter::Transform2D>(window, glm::vec2{105.0f, 140.0f}, glm::vec2{150.0f, 80.0f});

    registry.emplace<Components::Sprite2D>(window, typewriter::ResourceManager::loadSprite("assets/Window.png", typewriter::RectI{0,0,64,32}));
    registry.emplace<Components::Window>(window, true);
    registry.emplace<Components::InteractableObject>(window, [&registry, this](typewriter::Entity player, typewriter::Entity object)
    {
           registry.get<Components::Window>(object).opened = !registry.get<Components::Window>(object).opened;
        
            if (registry.get<Components::Window>(object).opened)
            {
                SoundManager::get().getSound("Window-open").replay();
                window_open_animation->reset();
            }
            else
            {
                SoundManager::get().getSound("Window-close").replay();
                window_close_animation->reset();
            }
    });
    
    // Bed
    typewriter::Entity bed = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(bed, glm::vec2{400.0f, 250.0f}, glm::vec2{150.0f, 75.0f});
    //registry.emplace<Components::SpriteAnimation>(bed, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Bed.png")});
    registry.emplace<typewriter::Collision2D>(bed, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    // Computer
    computer = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(computer, glm::vec2{50.0f, 400.0f}, glm::vec2{80.0f, 80.0f});
    //registry.emplace<Components::SpriteAnimation>(computer, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Computer.png", 16, 16)});
    registry.emplace<typewriter::Collision2D>(computer, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    registry.emplace<Components::InteractableObject>(computer, [&registry, this](typewriter::Entity player, typewriter::Entity object)
    {
        setState(GameState::G_COMPUTER);
    });
    
    initPlayerAnimations();
    
    SoundManager::get().getSound("Ambient").replay();
    SoundManager::get().getSound("Yawn").replay();
    
    if (game_manager->getCurrentday() == 1)
    {
        SoundManager::get().getSound("MusicDay2").replay();
    }
    else
    {
        SoundManager::get().getSound("MusicDay2").stop();
    }
    if (game_manager->getCurrentday() == 2)
    {
        SoundManager::get().getSound("MusicDay3").replay();
    }
    else
    {
        SoundManager::get().getSound("MusicDay3").stop();
    }
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
        {
            SoundManager::get().getSound("MenuMusic").stop();
        }
        break;
    case GameState::G_GAME:
        break;
    case GameState::G_COMPUTER:
        SoundManager::get().getSound("Laptop-close");
        exitComputerState(current_computer_state);
        break;
    case GameState::G_DAY_END:
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
        {
            
            SoundManager::get().getSound("MenuMusic").play();
        }
        break;
    case GameState::G_GAME:
        break;
    case GameState::G_COMPUTER:
        SoundManager::get().getSound("Laptop-open").replay();
        enterComputerState(current_computer_state);
        break;
    case GameState::G_DAY_END:
        {
            displayed_hint = glm::linearRand(0,3);
        }
        break;
    case GameState::G_GAME_OVER:
        {
            game_manager->resetTimer();
            game_manager->resetDroneTimer();
        }
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
        
        text_position_timer += deltaTime;
        if(typewriter::Input::isKeyPressed(typewriter::SCANCODE_SPACE))
        {
            game_manager = std::make_unique<GameManager>(scene, player, window);
                
            initGameStory();
                
            init();
            setState(GameState::G_GAME);
        }
            
        button_system->update(deltaTime, mouse_position, mouse_down, mouse_up);
        break;
    case GameState::G_GAME:
        {
            input_system->update(deltaTime);
            
            updatePlayerAnimations(deltaTime);
            playPlayerSounds();
            updateObjects(deltaTime);
            updateLevel(deltaTime);
            updateWindowAnimations(deltaTime);
            food_spawner->update(deltaTime);
            
            collision_system->update(deltaTime);
            interaction_system->update(deltaTime, interact);
            
            // This is by far the hackiest thing I have ever done
            if (object_manager->update(deltaTime))
            {
                microwave_done_light->reset();
                microwave_done_dark->reset();
            }
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
            
            game_manager->update(deltaTime);
            checkLoseCondition();
            
            updateDialogues(deltaTime);
            
            // Resume progress when you eat something
            if (is_food_eaten == true && is_food_eaten_active == true)
            {
                is_food_eaten_active = false;
                game_manager->stopProgress(false);
                showDialogue("Food was delicious. Let me play video games before I go out");
            }
            
            if (game_manager->isDroneWarning() && drone_warning_active)
            {
                showDialogue("I have kept the window open for too long. They might notice it. I need to close it as soon as possible");
                drone_warning_active = false;
            }
            
            checkDayEnd(); 
            
            food_order_timer += deltaTime;
            
        }
        break;
    case GameState::G_COMPUTER:
        // This is by far the hackiest thing I have ever done
        if (object_manager->update(deltaTime))
        {
            microwave_done_light->reset();
            microwave_done_dark->reset();
        }
        
        food_order_timer += deltaTime;
        
        game_manager->update(deltaTime);
        checkLoseCondition();
        updateObjects(deltaTime);
        updateLevel(deltaTime);
        updateWindowAnimations(deltaTime);
        food_spawner->update(deltaTime);
        
        checkDayEnd();
        
        updateDialogues(deltaTime);
        
        if (game_manager->isDroneWarning() && drone_warning_active)
        {
            showDialogue("I have kept the window open for too long. They might notice it. I need to close it as soon as possible");
            drone_warning_active = false;
        }
        
        button_system->update(deltaTime, mouse_position, mouse_down, mouse_up);
        
        updateComputerState(current_computer_state, deltaTime);
        break;
    case GameState::G_DAY_END:
        {
            day_end_timer += deltaTime;
            day_transit_timer += deltaTime;
            
            if (day_transit_timer < DAY_TRANSIT_TIME)
            {
                
            }
            
            if (day_end_timer >= DAY_END_TIME)
            {
                day_end_timer = 0.0f;
                day_transit_timer = 0.0f;
                // Remove all objects
                scene.getRegistry().clear();
                messages.clear();
                init();
                game_manager->setNextDay();
                setState(GameState::G_GAME);
            }
            break;
        }
    case GameState::G_GAME_OVER:
        {
            game_over_timer += deltaTime;
            game_over_transit_timer += deltaTime;
            
            if (game_over_timer >= DAY_END_TIME)
            {
                game_over_timer = 0.0f;
                game_over_transit_timer = 0.0f;
                
                scene.getRegistry().clear();
                
                is_food_eaten = false;
                is_food_eaten_active = true;
                drone_warning_active = true;
                messages.clear();
                
                init();
                game_manager->restartDay();
                setState(GameState::G_GAME);
            }
        }
        break;
    case GameState::G_DIALOGUE:
        {
            updateDialogues(deltaTime);
            
            if (dialogues.empty())
            {
                setState(GameState::G_GAME);
            }
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
        {   
            auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 40);
            auto text = typewriter::ResourceManager::loadText(font, "Press (SPACE)");
            
            float floating_value = 30.0f;
            float to_move = std::sin(text_position_timer) * floating_value;
            typewriter::Renderer2D::drawText(text.get(), 400.0f, text_position_y + to_move);
            
            auto font1 = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 28);
            auto text1 = typewriter::ResourceManager::loadText(font1, 
                "Welcome to our game Compliance!\n"
                         "The rules in the game are simple.\n"
                         "* Pick up food packages and cook them in a microwave to remove hunger.\n"
                         " * Play videogames on your PC to improve your well being.\n"
                         " * Finally, if it gets too hot in a room open a window, but not for long because the government might notice!\n"
                         "Enjoy our game.\n"
                         "Infinite Monkey Theorem Team\n");
            text1->setWrapWidth(500);
            typewriter::Renderer2D::drawText(text1.get(), 500.0f, 25.0f);
            
            typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Icon.jpg"), 25.0f, 25.0f, 435.0f, 280.0f);
        }
        break;
    case GameState::G_GAME:
        {
            renderLevel();
        
            renderObjects();
            food_spawner->render(day, night);
            renderSystem(false);
            game_manager->render();
            
            interaction_system->render();
        }
        break;
    case GameState::G_COMPUTER:
        {
            renderLevel();
            
            renderObjects();
            food_spawner->render(day, night);
            renderSystem(false);
            game_manager->render();
        }
        break;
    case GameState::G_DAY_END:
        {
            renderLevel();
            renderSystem(false);
            renderObjects();
            food_spawner->render(day, night);
            game_manager->render();
        }
        break;
    case GameState::G_GAME_OVER:
        {
            renderLevel();
            renderSystem(false);
            renderObjects();
            food_spawner->render(day, night);
            game_manager->render();
            break;
        }
    case GameState::G_DIALOGUE:
        {
            renderLevel();
            renderSystem(false);
            renderObjects();
            food_spawner->render(day, night);
            game_manager->render();
            break;
        }
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
        renderSystem(true);
        break;
    case GameState::G_GAME:
        {
            renderDayInfo();
            renderFilter();
            renderStats(); 
            renderDialogues();
        }
        break;
    case GameState::G_COMPUTER:
        {
            typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/PCUI.png", typewriter::RectI{2,31, 240, 135}), 50.0f, 31.0f, screen_width - 100.0f, screen_height - 50.0f);
            
            renderDayInfo();
            
            renderSystem(true);
            renderFilter();
        
            renderComputerState(current_computer_state);
            
            renderStats();
            renderDialogues();
        }
        break;
    case GameState::G_DAY_END:
        {
            if (day_transit_timer < DAY_TRANSIT_TIME)
            {
                float percentage = static_cast<float>(day_transit_timer) / DAY_TRANSIT_TIME;
                uint8_t black = static_cast<uint8_t>(255.0f * percentage);
                typewriter::Color color_black{0, 0, 0,  black};
                typewriter::Renderer2D::drawRectangle(0.0f, 0.0f, screen_width, screen_height, color_black);
            }
            else
            {
                typewriter::Renderer2D::drawRectangle(0.0f, 0.0f, screen_width, screen_height, typewriter::Color::Black);
                auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 35);
                std::string hint = hints[displayed_hint];
                auto text = typewriter::ResourceManager::loadText(font, hint);
                text->setWrapWidth(400);
                typewriter::Renderer2D::drawText(text.get(), 200.0f, 225.0f);
            }
          
            break;
        }
    case GameState::G_GAME_OVER:
        {
            if (game_over_transit_timer < DAY_TRANSIT_TIME)
            {
                float percentage = static_cast<float>(game_over_transit_timer) / DAY_TRANSIT_TIME;
                uint8_t black = static_cast<uint8_t>(255.0f * percentage);
                typewriter::Color color_black{0, 0, 0,  black};
                typewriter::Renderer2D::drawRectangle(0.0f, 0.0f, screen_width, screen_height, color_black);
            }
            else
            {
                typewriter::Renderer2D::drawRectangle(0.0f, 0.0f, screen_width, screen_height, typewriter::Color::Black);
                auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 35);
                //std::string hint = hints[displayed_hint];
                auto text = typewriter::ResourceManager::loadText(font, "GAME OVER");
                text->setWrapWidth(400);
                typewriter::Renderer2D::drawText(text.get(), 200.0f, 225.0f);
            }
            break;
        }
    case GameState::G_DIALOGUE:
        {
            renderStats(); 
            renderDayInfo();
            renderFilter(); 
            renderDialogues();
            break;
        }
    default:
        break;
    }   
}

void GameLayer::initWindowAnimations()
{
    {
        std::vector<int> frames = {0,1,2,3,4,5,6,7};
        window_close_animation = std::make_unique<typewriter::SpriteAnimation>(typewriter::ResourceManager::loadSpriteSheet("assets/Window.png", 64, 32), 14.0f, false, frames);
    }
    
    {
        std::vector<int> frames = {8,9,10,11,12,13,14,15};
        window_open_animation = std::make_unique<typewriter::SpriteAnimation>(typewriter::ResourceManager::loadSpriteSheet("assets/Window.png", 64, 32), 14.0f, false, frames);
    }
    
    {
        std::vector<int> frames = {0,1,2,3,4,5};
        window_view_animation = std::make_unique<typewriter::SpriteAnimation>(typewriter::ResourceManager::loadSpriteSheet("assets/Windowview.png", 46, 25), 7.0f, true, frames);
    }
}

void GameLayer::updateWindowAnimations(float deltaTime)
{
    window_close_animation->update(deltaTime);
    window_open_animation->update(deltaTime);
    window_view_animation->update(deltaTime);
    
    scene.getRegistry().get<Components::Sprite2D>(window_view).sprite = *window_view_animation.get();
    
    if (window_open_animation->isDone() == false)
        scene.getRegistry().get<Components::Sprite2D>(window).sprite = *window_open_animation.get();
    else if (window_close_animation->isDone() == false)
        scene.getRegistry().get<Components::Sprite2D>(window).sprite = *window_close_animation.get();
    
    scene.getRegistry().get<Components::Sprite2D>(window).layer = WINDOW_LAYER;
}

void GameLayer::initColliders()
{
    auto& registry = scene.getRegistry();
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{0.0f, 0.0f}, glm::vec2{560.0f, 245.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{322.0f, 249.0f}, glm::vec2{80.0f, 81.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{561.0f, 3.0f}, glm::vec2{84.0f, 163.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{643.0f, 0.0f}, glm::vec2{158.0f, 244.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{1.0f, 490.0f}, glm::vec2{320.0f, 83.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{0.0f, 83.0f}, glm::vec2{28.0f, 405.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{294.0f, 557.0f}, glm::vec2{26.0f, 84.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{320.0f, 542.0f}, glm::vec2{482.0f, 30.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
    {
        typewriter::Entity col = registry.create();
        registry.emplace<typewriter::Transform2D>(col, glm::vec2{768.0f, 83.0f}, glm::vec2{33.0f, 489.0f});
        registry.emplace<typewriter::Collision2D>(col, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    }
}

void GameLayer::initObjectsAnimation()
{
    //Microwave animations
    
    // Close light
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveLightClose.png", 48, 53);
        std::vector<int> frames = {0,1,2,3,4,5,6,7};
        microwave_close_light = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, false, frames);
    }
    // Close Dark
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveDarkClose.png", 48, 53);
        std::vector<int> frames = {0,1,2,3,4,5,6,7};
        microwave_close_dark = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, false, frames);
    }
    
    //  Cooking Light
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveLightCooking.png", 48, 55);
        std::vector<int> frames = {0,1,2,3,4,5,6};
        microwave_cooking_light = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, true, frames);
    }
    // Cooking Dark
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveDarkCooking.png", 48, 55);
        std::vector<int> frames = {0,1,2,3,4,5,6};
        microwave_cooking_dark = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, true, frames);
    }
    
    //  Done Light
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveLightDone.png", 48, 53);
        std::vector<int> frames = {0,1,2,3,4,5,6,7,8,};
        microwave_done_light = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, false, frames);
    }
    // Done Dark
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/Microwave/MicrowaveDarkDone.png", 48, 53);
        std::vector<int> frames = {0,1,2,3,4,5,6,7,8};
        microwave_done_dark = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, false, frames);
    }
    
    // Door animations
    //  Anim light
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/DoorLight.png", 32, 67);
        std::vector<int> frames = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
        door_light_anim = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 14.0f, false, frames);
    }
    // Anim dark
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/DoorDark.png", 32, 67);
        std::vector<int> frames = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
        door_dark_anim = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 14.0f, false, frames);
    }
    
    // Laptop
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/LaptopLight.png", 39, 54);
        std::vector<int> frames = {0,1,2,3,4,5,6};
        laptop_light_anim = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, true, frames);
    }
    {
        auto spritesheet = typewriter::ResourceManager::loadSpriteSheet("assets/LaptopDark.png", 39, 54);
        std::vector<int> frames = {0,1,2,3,4,5,6};
        laptop_dark_anim = std::make_unique<typewriter::SpriteAnimation>(spritesheet, 7.0f, true, frames);
    }
}

void GameLayer::updateLevel(float deltaTime)
{
    world_theme_transit_timer += deltaTime;
    
    if (world_theme_transit_timer >= WORLD_THEME_TRANSIT_TIME)
        world_theme_transit_timer = WORLD_THEME_TRANSIT_TIME;
    
    if (was_day != scene.getRegistry().get<Components::Window>(window).opened)
        world_theme_transit_timer = 0.0f;
    
    was_day = scene.getRegistry().get<Components::Window>(window).opened;
    
    uint8_t alpha = static_cast<uint8_t>(world_theme_transit_timer / WORLD_THEME_TRANSIT_TIME * 255.0f);
    day = was_day ? alpha : 255 - alpha;
    night = was_day ? 255 - alpha : alpha;
}

void GameLayer::updateObjects(float deltaTime)
{
    microwave_close_light->update(deltaTime);
    microwave_close_dark->update(deltaTime);
    microwave_cooking_light->update(deltaTime);
    microwave_cooking_dark->update(deltaTime);
    microwave_done_light->update(deltaTime);
    microwave_done_dark->update(deltaTime);
    
    door_light_anim->update(deltaTime);
    door_dark_anim->update(deltaTime);
    
    laptop_dark_anim->update(deltaTime);
    laptop_light_anim->update(deltaTime);
}

void GameLayer::renderLevel()
{
    float level_width = 800.0f;
    float level_height = level_width / 1.4f;
    
    auto house_light_sprite = typewriter::ResourceManager::loadSprite("assets/HouseLight.png");
    house_light_sprite.setColor(typewriter::Color{255,255,255,day});
    typewriter::Renderer2D::drawSprite(house_light_sprite, 0.0f, 0.0f, level_width, level_height);
    
    auto house_dark_sprite = typewriter::ResourceManager::loadSprite("assets/HouseDark.png");
    house_dark_sprite.setColor(typewriter::Color{255,255,255,night});
    typewriter::Renderer2D::drawSprite(house_dark_sprite, 0.0f, 0.0f, level_width, level_height);
}

void GameLayer::renderObjects()
{
    auto& registry = scene.getRegistry();
    if (microwave != entt::null)
    {
        const auto& ts = registry.get<typewriter::Transform2D>(microwave);
        
        // If started cooking
        if(microwave_close_light->isDone() == false || microwave_close_dark->isDone() == false)
        {
            typewriter::Sprite sprite1 = *microwave_close_light.get();
            sprite1.setColor({255,255,255,day});
            typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            typewriter::Sprite sprite2 = *microwave_close_dark.get();
            sprite2.setColor({255,255,255,night});
            typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        }
        // Else if cooking
        else if (registry.get<Components::Microwave>(microwave).status == Components::Microwave::MicrowaveStatus::COOKING)
        {
            typewriter::Sprite sprite1 = *microwave_cooking_light.get();
            std::cout << "play cooking animation" << std::endl;
            sprite1.setColor({255,255,255,day});
            typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            typewriter::Sprite sprite2 = *microwave_cooking_dark.get();
            sprite2.setColor({255,255,255,night});
            typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        }
        // Else if done cooking
        else if(microwave_done_light->isDone() == false || microwave_done_dark->isDone() == false)
        {
            typewriter::Sprite sprite1 = *microwave_done_light.get();
            sprite1.setColor({255,255,255,day});
            typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            typewriter::Sprite sprite2 = *microwave_done_dark.get();
            sprite2.setColor({255,255,255,night});
            typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        }
        // Else draw common microwave
        else
        {
            if (registry.get<Components::Microwave>(microwave).status == Components::Microwave::MicrowaveStatus::DONE)
            {
                typewriter::Sprite sprite1 = (*microwave_done_light)[8];
                sprite1.setColor({255,255,255,day});
                typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
                typewriter::Sprite sprite2 = (*microwave_done_dark)[8];
                sprite2.setColor({255,255,255,night});
                typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            }
            else
            {
                typewriter::Sprite sprite1 = (*microwave_close_light)[0];
                sprite1.setColor({255,255,255,day});
                typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
                typewriter::Sprite sprite2 = (*microwave_close_dark)[0];
                sprite2.setColor({255,255,255,night});
                typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            }
        }
    }
    
    // door
    
    if (door != entt::null)
    {
        const auto& ts = registry.get<typewriter::Transform2D>(door);
        
        // If animation is playing
        if (!door_dark_anim->isDone() || !door_light_anim->isDone())
        {
            typewriter::Sprite sp_l = *door_light_anim.get(); 
            sp_l.setColor({255,255,255,day});
            typewriter::Sprite sp_d = *door_dark_anim.get(); 
            sp_d.setColor({255,255,255,night});
            typewriter::Renderer2D::drawSprite(sp_l, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            typewriter::Renderer2D::drawSprite(sp_d, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        }
        else
        {
            typewriter::Sprite sp_l = (*door_light_anim)[0]; 
            sp_l.setColor({255,255,255,day});
            typewriter::Sprite sp_d = (*door_dark_anim)[0]; 
            sp_d.setColor({255,255,255,night});
            typewriter::Renderer2D::drawSprite(sp_l, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
            typewriter::Renderer2D::drawSprite(sp_d, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        }
        
    } 
    
    if (computer != entt::null)
    {
        const auto& ts = registry.get<typewriter::Transform2D>(computer);
        
        typewriter::Sprite sprite1 = *laptop_light_anim.get();
        sprite1.setColor({255,255,255,day});
        typewriter::Renderer2D::drawSprite(sprite1, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
        
        typewriter::Sprite sprite2 = *laptop_dark_anim.get();
        sprite2.setColor({255,255,255,night});
        typewriter::Renderer2D::drawSprite(sprite2, ts.position.x, ts.position.y, ts.size.x, ts.size.y);
    }
    
    // Bed
    auto bed_sprite1 = typewriter::ResourceManager::loadSprite("assets/LightBed.png");
    bed_sprite1.setColor({255,255,255,day});
    typewriter::Renderer2D::drawSprite(bed_sprite1, 400.0f, 250.0f, 150.0f, 75.0f);
    
    auto bed_sprite2 = typewriter::ResourceManager::loadSprite("assets/DarkBed.png");
    bed_sprite2.setColor({255,255,255,night});
    typewriter::Renderer2D::drawSprite(bed_sprite2, 400.0f, 250.0f, 150.0f, 75.0f);
}

void GameLayer::renderStats()
{
    auto& player_component = scene.getRegistry().get<Components::Player>(player);
    auto font = typewriter::ResourceManager::loadFont("assets/Fonts/Jersey15-Regular.ttf", 35);
    
    
    typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Icons.png"), -15.0f, -5.0f, 120.0f, 200.0f);
            
    auto text = typewriter::ResourceManager::loadText(font, std::format("{}%", static_cast<int>(player_component.hunger * 100.0f)));
    typewriter::Renderer2D::drawText(text.get(), 58.0f, 15.0f);
    
    auto text2 = typewriter::ResourceManager::loadText(font, std::format("{}%", static_cast<int>(player_component.temperature * 100.0f)));
    typewriter::Renderer2D::drawText(text2.get(), 58.0f, 73.0f);
    
    auto text3 = typewriter::ResourceManager::loadText(font, std::format("{}%", static_cast<int>(player_component.sanity * 100.0f)));
    typewriter::Renderer2D::drawText(text3.get(), 58.0f, 131.0f);
}

void GameLayer::renderFilter()
{
    float time = game_manager->getTimer();
    typewriter::Color mix;
    bool should_render = true;

    typewriter::Color morning_color = typewriter::Color::Orange;
    typewriter::Color afternoon_color = typewriter::Color::DarkGrey;
    typewriter::Color evening_color = typewriter::Color::Black;

    if (time >= 0.0f && time < DAY_MORNING_DURATION)
    {
        float w = time / DAY_MORNING_DURATION;
        
        mix = (typewriter::Color::Yellow * (1.0f - w)) + (morning_color * w);
    }
    else if (time >= DAY_MORNING_DURATION && time < DAY_AFTERNOON_DURATION)
    {
        float phase_duration = DAY_AFTERNOON_DURATION - DAY_MORNING_DURATION;
        float w = (time - DAY_MORNING_DURATION) / phase_duration;
        
        mix = (morning_color * (1.0f - w)) + (afternoon_color * w);
    }
    else if (time >= DAY_AFTERNOON_DURATION && time <= DAY_EVENING_DURATION)
    {
        float phase_duration = DAY_EVENING_DURATION - DAY_AFTERNOON_DURATION;
        float w = (time - DAY_AFTERNOON_DURATION) / phase_duration;
        
        mix = (afternoon_color * (1.0f - w)) + (evening_color * w);
    }
    else
    {
        mix = evening_color;
    }

    if (should_render)
    {
        mix.channels.a = 20; 
        typewriter::Renderer2D::drawRectangle(0.0f, 0.0f, screen_width, screen_height, mix);
    }
}

void GameLayer::initAssets()
{
    level_sprite = typewriter::ResourceManager::loadSprite("assets/Level.png");
    
    // Sounds
    
    SoundManager::get().loadSound("Walking", "assets/Sounds/Walking.mp3");
    
    SoundManager::get().loadSound("Breathing", "assets/Sounds/Breathing.mp3");
    SoundManager::get().getSound("Breathing").setVolume(0.5f);
    
    SoundManager::get().loadSound("Ambient", "assets/Sounds/Ambient.mp3");
    SoundManager::get().getSound("Ambient").setLooping(true);
    SoundManager::get().getSound("Ambient").setVolume(0.15f);
    
    SoundManager::get().loadSound("Food-buy", "assets/Sounds/Food-buy.mp3");
    SoundManager::get().loadSound("Food-delivery", "assets/Sounds/Food-delivery.mp3");
    SoundManager::get().loadSound("Laptop-app-close", "assets/Sounds/Laptop-app-close.mp3");
    SoundManager::get().loadSound("Laptop-app-opened", "assets/Sounds/Laptop-app-opened.mp3");
    SoundManager::get().loadSound("Laptop-close", "assets/Sounds/Laptop-close.mp3");
    
    SoundManager::get().loadSound("Laptop-message", "assets/Sounds/Laptop-message.mp3");
    SoundManager::get().getSound("Laptop-message").setVolume(0.4f);
    
    SoundManager::get().loadSound("Siren", "assets/Sounds/Siren.mp3");
    SoundManager::get().getSound("Siren").setVolume(0.4f);
    
    SoundManager::get().loadSound("Laptop-open", "assets/Sounds/Laptop-open.mp3");
    SoundManager::get().loadSound("Microwave-cooking", "assets/Sounds/Microwave-cooking.mp3");
    SoundManager::get().loadSound("Microwave-finished", "assets/Sounds/Microwave-finished.mp3");
    SoundManager::get().loadSound("Microwave-started", "assets/Sounds/Microwave-started.mp3");
    SoundManager::get().loadSound("MinigameMusic", "assets/Sounds/MinigameMusic.mp3");
    SoundManager::get().loadSound("MouseClick", "assets/Sounds/MouseClick.mp3");
    SoundManager::get().loadSound("Window-close", "assets/Sounds/Window-close.mp3");
    SoundManager::get().loadSound("Window-open", "assets/Sounds/Window-open.mp3");
    
    SoundManager::get().loadSound("Yawn", "assets/Sounds/Yawn.mp3");
    SoundManager::get().getSound("Yawn").setVolume(0.3f);
     
    SoundManager::get().loadSound("Warning", "assets/Sounds/Warning.mp3");
    SoundManager::get().getSound("Warning").setVolume(0.7f);
    
    SoundManager::get().loadSound("Scream1", "assets/Sounds/Scream2.mp3");
    SoundManager::get().getSound("Scream1").setVolume(0.5f);
    SoundManager::get().loadSound("Scream2", "assets/Sounds/Scream2.mp3");
    SoundManager::get().getSound("Scream2").setVolume(0.5f);
    
    SoundManager::get().loadSound("Shot1", "assets/Sounds/Shot1.mp3");
    SoundManager::get().getSound("Shot1").setVolume(0.5f);
    SoundManager::get().loadSound("Shot2", "assets/Sounds/Shot2.mp3");
    SoundManager::get().getSound("Shot2").setVolume(0.5f);
    
    SoundManager::get().loadSound("MenuMusic", "assets/Sounds/MenuMusic.mp3");
    SoundManager::get().getSound("MenuMusic").setVolume(0.7f);
    
    SoundManager::get().loadSound("MusicDay2", "assets/Sounds/MusicDay2.mp3");
    SoundManager::get().getSound("MusicDay2").setVolume(0.7f);
    SoundManager::get().getSound("MusicDay2").setLooping(true);
    
    SoundManager::get().loadSound("MusicDay3", "assets/Sounds/MusicDay3.mp3");
    SoundManager::get().getSound("MusicDay3").setVolume(0.7f);
    SoundManager::get().getSound("MusicDay3").setLooping(true);
}
