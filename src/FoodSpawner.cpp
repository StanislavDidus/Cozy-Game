#include "FoodSpawner.hpp"

#include "Components.hpp"
#include <typewriter/Typewriter.hpp>

#include "SoundManager.hpp"

using namespace typewriter;

FoodSpawner::FoodSpawner(Scene& scene)
    : scene(scene)
{
    
}

constexpr int MAX_FOOD = 3;
constexpr float FOOD_WIDTH = 70.0f;
constexpr float FOOD_HEIGHT = 50.0f;

void FoodSpawner::update(float deltaTime)
{
    if (spawn)
    {
        spawn_timer += deltaTime;
        
        if (spawn_timer >= spawn_time)
        {
            spawn = false;
            spawn_timer = 0.0f;
            
            spawnFoodObject();
            
            delivery_zone = entt::null;
        }
    }
}

void FoodSpawner::spawnFood(Entity delivery_zone)
{
    spawn = true;
    this->delivery_zone = delivery_zone;
}

void FoodSpawner::render(uint8_t day, uint8_t night)
{
    auto& registry = scene.getRegistry();
    
    auto view = registry.view<::typewriter::Transform2D, Components::FoodPackage>();
    for (auto [entity, transform, food_package] : view.each())
    {
        if (food_package.food_count == 1)
        {
            auto sprite = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageLight.png", ::typewriter::RectI{0,0, 34, 22});
            auto sprite1 = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageDark.png", ::typewriter::RectI{0,0, 34, 22});
            sprite.setColor({255, 255, 255, day});
            sprite1.setColor({255,255,255, night});
            ::typewriter::Renderer2D::drawSprite(sprite, transform.position.x, transform.position.y, transform.size.x, transform.size.y);
            ::typewriter::Renderer2D::drawSprite(sprite1, transform.position.x, transform.position.y, transform.size.x, transform.size.y);
        }
        else if (food_package.food_count == 2)
        {
            auto sprite = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageLight.png", ::typewriter::RectI{35,0, 34, 38});
            auto sprite1 = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageDark.png", ::typewriter::RectI{35,0, 34, 38});
            sprite.setColor({255, 255, 255, day});
            sprite1.setColor({255,255,255, night});
            ::typewriter::Renderer2D::drawSprite(sprite, transform.position.x, transform.position.y - 20.0f, transform.size.x, transform.size.y + 20.0f);
            ::typewriter::Renderer2D::drawSprite(sprite1, transform.position.x, transform.position.y - 20.0f, transform.size.x, transform.size.y + 20.0f);
        }
        else if (food_package.food_count == 3)
        {
            auto sprite = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageLight.png", ::typewriter::RectI{70,0, 34, 54});
            auto sprite1 = ::typewriter::ResourceManager::loadSprite("assets/FoodPackageDark.png", ::typewriter::RectI{70,0, 34, 54});
            sprite.setColor({255, 255, 255, day});
            sprite1.setColor({255,255,255, night});
            ::typewriter::Renderer2D::drawSprite(sprite, transform.position.x, transform.position.y - 20.0f, transform.size.x, transform.size.y + 40.0f);
            ::typewriter::Renderer2D::drawSprite(sprite1, transform.position.x, transform.position.y - 20.0f, transform.size.x, transform.size.y + 40.0f);
        }
    }
}

void FoodSpawner::spawnFoodObject()
{
    auto& registry = scene.getRegistry();
    
    auto& delivery_zone_component  = registry.get<Components::DeliveryZone>(delivery_zone);
    
    SoundManager::get().getSound("Food-delivery").replay();
    
    // If there is no food on the delivery zone
    if (delivery_zone_component.food == entt::null)
    {
       // Create food object 
        Entity food = registry.create(); 
        glm::vec2 delivery_position = registry.get<Transform2D>(delivery_zone).position;
        registry.emplace<Transform2D>(food, glm::vec2{delivery_position.x, delivery_position.y}, glm::vec2{FOOD_WIDTH, FOOD_HEIGHT});
        //registry.emplace<Components::Sprite2D>(food, ResourceManager::loadSprite("assets/FoodPackage.png", math::RectI(0,0,16,16)), ITEM_LAYER);
        registry.emplace<Components::FoodPackage>(food);
        
        // Assign food to delivery zone
        delivery_zone_component.food = food;
        
        // On Interact
        registry.emplace<Components::InteractableObject>(delivery_zone, [&delivery_zone_component, &registry](Entity player, Entity object)
        {
            auto& player_component = registry.get<Components::Player>(player);
            if (delivery_zone_component.food != entt::null &&
                player_component.inv_food < player_component.max_inv_food)
            {
                player_component.inv_food += registry.get<Components::FoodPackage>(delivery_zone_component.food).food_count;
                if (player_component.inv_food > player_component.max_inv_food)
                    player_component.inv_food = player_component.max_inv_food;
                registry.destroy(delivery_zone_component.food);
                delivery_zone_component.food = entt::null;
                registry.remove<Components::InteractableObject>(object);
            }
        });
    }
    // If there is already food
    else
    {
       // Increase food count 
        int& food_count = registry.get<Components::FoodPackage>(delivery_zone_component.food).food_count;
        
        // If reaches limit
        if (++food_count > MAX_FOOD)
        {
            //TODO: for now just type a warning 
            //LOG_WARN("Food limit reached");
            food_count = MAX_FOOD;
        }
    }
    
}
