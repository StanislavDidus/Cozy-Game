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
#include "glm/gtc/random.hpp"
#include "typewriter/Typewriter.hpp"

static std::array<std::string, 5> hints = 
    {
    "Press on microwave while cooking to speed up the process.",
        "Try to think in the future on what you are going to need.",
        "You cannot have more then three food packages by your door. EAT them.",
        "Always check your mailbox for new story details.",
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
        day.hunger_up = 0.002f;
        day.sanity_up = 0.0f;
        day.heat_up = 0.0f;
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(false);
    
            food_spawner->spawnFood(delivery_zone);
            food_spawner->spawnFood(delivery_zone);
            can_order_food = false;
                }, 0.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("We were going to hang out with Marks today. We are to meet in the park in 3 a.m.");
        }, 3.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("I need to eat something.");
        }, 6.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("Ooh the food is already delivered.");
            game_manager->stopProgress(true);
        }, 7.0f});
        events.push_back(EventPoint{[this]
        {
            showDialogue("Its time for me to go.");
        }, 30.0f});
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
                showDialogue("I can't even leave my house now. I need to contact my family as soon as possible..");
            };
            messages.push_back(message);
                }, 35.0f});
        events.push_back(EventPoint{[this]
        {
            food_spawner->spawnFood(delivery_zone);
            showDialogue("What is this box in my doorway. Is this food?");
            EmailMessage message{"To all residents of this complex", 
                "Our dear leader has given us a gracious supply of food and water.\n"
                " Enough to divide among each resident."
                " With it they have shared with us a set of rules to get us through these trying times: First of all, you must not consume any other good except for the one delivered at your doorstep by the government."
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
        day.hunger_up = 0.0035f;
        day.sanity_up = 0.005f;
        day.heat_up = 0.0003f;
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
            messages.push_back(message);
        }, 85.0f});
        events.push_back(EventPoint{[this]
        {
        }, 110.0f});
        events.push_back(EventPoint{[this]
        {
            // Play gunshot sound
        }, 115.0f});
        days.push_back(day);
    }
    // Day three
    {
        Day day;
        
        day.hunger_up = 0.005f;
        day.sanity_up = 0.006f;
        day.heat_up = 0.00075f;
        auto& events = day.events;
        
        events.push_back(EventPoint{[this]
        {
            game_manager->setCanDroneAttack(true);
        }, 0.0f});
        events.push_back(EventPoint{[this]
        {
            // Play scream sound
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
            messages.push_back(message);
        }, 20.0f});
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
            messages.push_back(message);
        }, 75.0f});
        
        events.push_back(EventPoint{ [this]
        {
           showDialogue("I am so worried about tomorrow day. Father, Marks, protest. They have to give answers");
        }, 110.0f});
        days.push_back(day);
    } 
    // Day four
    {
        Day day;
        
        day.hunger_up = 0.010f;
        day.sanity_up = 0.008f;
        day.heat_up = 0.0009f;
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
            };
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
    {
        std::vector<int> frames = {0,1,2,3,4,5,6,7};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/Player.png", 32, 48);
        down_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 5.0f, frames);
    }
    {
        
        std::vector<int> frames = {8,9,10,11,12,13,14,15};
        auto sprite_sheet = typewriter::ResourceManager::loadSpriteSheet("assets/Player.png", 32, 48);
        top_movement = std::make_unique<typewriter::SpriteAnimation>(sprite_sheet, 5.0f, frames);
    }
}

void GameLayer::playPlayerSounds()
{
    auto& registry = scene.getRegistry();
    auto& player_component = registry.get<Components::Player>(player);
    
    if (glm::length(player_component.velocity) >= 10.0f)
    {
        /*
        Audio::Sound sound{"assets/Sounds/Walking.wav", Audio::Sound::Type::Sound};
        sound.play();
    */
    }
}

void GameLayer::updatePlayerAnimations(float deltaTime)
{
    auto& registry = scene.getRegistry();
    auto& player_component = registry.get<Components::Player>(player);
    auto& render_component = registry.get<Components::Sprite2D>(player);
    
    if (player_component.velocity.y > 0.0f)
    {
        down_movement->update(deltaTime);
        render_component.sprite = *down_movement.get();
    }
    else if (player_component.velocity.y < 0.0f)
    {
        top_movement->update(deltaTime);
        render_component.sprite = *top_movement.get();
    }
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
    // Init Player
    player = scene.createEntity();
    game_manager->setPlayer(player);
    typewriter::Registry& registry = scene.getRegistry();
    registry.emplace<Components::Player>(player, glm::vec2{150.0f, 150.0f}, glm::vec2{50.0f, 50.0f}, glm::vec2{0.0f}, 250.0f, 300.0f);

    registry.emplace<typewriter::Transform2D>(player, glm::vec2{200.0f, 200.0f}, glm::vec2{50.0f,83.0f});
    registry.emplace<Components::Sprite2D>(player, typewriter::ResourceManager::loadSprite("assets/Player.png", typewriter::RectI{0,0,32,48}));
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
    
    delivery_zone = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(delivery_zone, glm::vec2{170.0f, 430.0f}, glm::vec2{80.0f, 80.0f});
    registry.emplace<Components::Sprite2D>(delivery_zone, typewriter::ResourceManager::loadSprite("assets/Carpet.png"));
    registry.emplace<Components::DeliveryZone>(delivery_zone);
    
    // Window

    window = scene.createEntity();
    game_manager->setWindow(window);
    registry.emplace<typewriter::Transform2D>(window, glm::vec2{400.0f, 50.0f}, glm::vec2{150.0f, 80.0f});

    registry.emplace<Components::SpriteAnimation>(window, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Window.png", 32, 22)});
    registry.emplace<Components::Window>(window, false);
    registry.emplace<Components::InteractableObject>(window, [&registry](typewriter::Entity player, typewriter::Entity object)
    {
           registry.get<Components::Window>(object).opened = !registry.get<Components::Window>(object).opened;

           registry.get<Components::SpriteAnimation>(object).frame = registry.get<Components::Window>(object).opened;
    });
    
    // Bed
    typewriter::Entity bed = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(bed, glm::vec2{740.0f, 150.0f}, glm::vec2{100.0f, 120.0f});
    registry.emplace<Components::SpriteAnimation>(bed, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Bed.png", 24, 24)});
    registry.emplace<typewriter::Collision2D>(bed, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    
    // Computer
    typewriter::Entity computer = scene.createEntity();
    registry.emplace<typewriter::Transform2D>(computer, glm::vec2{750.0f, 430.0f}, glm::vec2{90.0f, 90.0f});
    registry.emplace<Components::SpriteAnimation>(computer, typewriter::SpriteAnimation{typewriter::ResourceManager::loadSpriteSheet("assets/Computer.png", 16, 16)});
    registry.emplace<typewriter::Collision2D>(computer, typewriter::AABB{{}, {}}, typewriter::CollisionType::STATIC);
    registry.emplace<Components::InteractableObject>(computer, [&registry, this](typewriter::Entity player, typewriter::Entity object)
    {
        setState(GameState::G_COMPUTER);
    });
    
    initPlayerAnimations();
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
            scene.destroyEntity(start_button);
            scene.destroyEntity(exit_menu_button);
        }
        break;
    case GameState::G_GAME:
        break;
    case GameState::G_COMPUTER:
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
            auto& registry = scene.getRegistry();
            start_button = registry.create();
            registry.emplace<typewriter::Transform2D>(start_button, glm::vec2{200.0f, 200.0f}, glm::vec2{100.0f, 60.0f});
            registry.emplace<Components::Sprite2D>(start_button, typewriter::ResourceManager::loadSprite("assets/UI.png", typewriter::RectI(0,32,48,16)), 1, true);
            registry.emplace<typewriter::Clickable>(start_button, typewriter::AABB{glm::vec2{200.0f, 200.0f}, glm::vec2{300.0f, 260.0f}}, [this]
            {
                game_manager = std::make_unique<GameManager>(scene, player, window);
                
                initGameStory();
                
                init();
                setState(GameState::G_GAME);
                return true;
            });
            
            exit_menu_button = registry.create();
            registry.emplace<typewriter::Transform2D>(exit_menu_button, glm::vec2{200.0f, 300.0f}, glm::vec2{100.0f, 60.0f});
            registry.emplace<Components::Sprite2D>(exit_menu_button, typewriter::ResourceManager::loadSprite("assets/UI.png", typewriter::RectI(0,48,48,16)), 1, true);
            registry.emplace<typewriter::Clickable>(exit_menu_button, typewriter::AABB{glm::vec2{200.0f, 300.0f}, glm::vec2{300.0f, 360.0f}}, [this]
            {
                std::exit(0);
                return true;
            });
        }
        break;
    case GameState::G_GAME:
        break;
    case GameState::G_COMPUTER:
        enterComputerState(current_computer_state);
        break;
    case GameState::G_DAY_END:
        {
            displayed_hint = glm::linearRand(0,4);
        }
        break;
    case GameState::G_GAME_OVER:
        {
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
        button_system->update(deltaTime, mouse_position, mouse_down, mouse_up);
        break;
    case GameState::G_GAME:
        {
            input_system->update(deltaTime);
            
            updatePlayerAnimations(deltaTime);
            playPlayerSounds();
            
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
        object_manager->update(deltaTime);
        
        food_order_timer += deltaTime;
        
        game_manager->update(deltaTime);
        checkLoseCondition();
        
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
            renderSystem(false);
        break;
    case GameState::G_GAME:
        {
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
        
            renderSystem(false);
            game_manager->render();
            
            interaction_system->render();
        }
        break;
    case GameState::G_COMPUTER:
        {
            // Render main level and game objects
            // PC UI is rendered in renderUIState
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
            
            renderSystem(false);
            game_manager->render();
        }
        break;
    case GameState::G_DAY_END:
        {
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
            renderSystem(false);
            game_manager->render();
        }
        break;
    case GameState::G_GAME_OVER:
        {
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
            renderSystem(false);
            game_manager->render();
            break;
        }
    case GameState::G_DIALOGUE:
        {
            typewriter::Renderer2D::drawSprite(level_sprite, 0, 0, screen_width, screen_height);
            renderSystem(false);
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
            renderStats(); 
            renderDayInfo();
            renderFilter();
            renderDialogues();
        }
        break;
    case GameState::G_COMPUTER:
        {
            typewriter::Renderer2D::drawSprite(typewriter::ResourceManager::loadSprite("assets/Computer_Screen.png"), 50.0f, 25.0f, screen_width - 100.0f, screen_height - 50.0f);
            
            renderStats();
            renderDayInfo();
            
            renderSystem(true);
            renderFilter();
        
            renderComputerState(current_computer_state);
            
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
}
