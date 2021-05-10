#include "scene_level1.h"
#include "../components/cmp_cat.h"
#include "../components/cmp_player.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_clock.h"
#include "../components/cmp_cat_AI.h"
#include "../components/cmp_item.h";
#include "../components/cmp_text.h"
#include "../components/cmp_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <thread>
#include <ctime>
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

//
//Ui Entities
//
shared_ptr<TextComponent> stillUiText[4]; //Texts that stay in place/don't highlight (ie currency, time)
shared_ptr<TextComponent> catStats[14]; //Texts that display the cat stats when opening that menu
shared_ptr<TextComponent> uiText[5]; //Texts that move or highlight
shared_ptr<ShapeComponent> stillUi[4]; //Boxes that stay in place/don't highlight (ie currency, time)
shared_ptr<ShapeComponent> ui[6]; //Boxes that move or highlight
shared_ptr<ShapeComponent> slots[6]; //Shop/Inventory Boxes that display the item
shared_ptr<ShapeComponent> box; //Background box for Shop/Inventory and cat stats
shared_ptr<ShapeComponent> minigame; // Minigame box
shared_ptr<Entity> minigameEntity;
shared_ptr<Entity> catBox;
shared_ptr<Entity> toys;
shared_ptr<Entity> food;
shared_ptr<Entity> boxEntity;
shared_ptr<Entity> date;
shared_ptr<Entity> currency;
shared_ptr<Entity> options;
shared_ptr<Entity> tutorial;
shared_ptr<Entity> shop;
shared_ptr<Entity> inventory;
shared_ptr<Entity> slot1;
shared_ptr<Entity> slot2;
shared_ptr<Entity> slot3;
shared_ptr<Entity> slot4;
shared_ptr<Entity> slot5;
shared_ptr<Entity> slot6;
shared_ptr<SpriteComponent> cursorEntity;
std::shared_ptr<Entity> cursorSprite;
//
//Ui Entities
//

//
//Sound Effects
//

sf::Sound paying;

//
//Sound Effects
//

static shared_ptr<Entity> cat;
std::shared_ptr<CatComponent> c;
std::shared_ptr<CatAI> a;
std::shared_ptr<SpriteComponent> sp;
std::shared_ptr<sf::Texture> spritesheet2 = std::make_shared<sf::Texture>();

int bondTracker = 0;

static shared_ptr<Entity> player;
std::shared_ptr<PlayerComponent> p;

static shared_ptr<Entity> clockTime;
std::shared_ptr<ClockComponent> cl;

std::vector<shared_ptr<Entity>> items;
static shared_ptr<Entity> item;
std::vector<Vector2f> itemLocations;
std::vector<std::string> itemType;
sf::Vector2f previousTarget;
std::string storeItem;
bool foodFound;
bool bathFound;

std::vector<std::shared_ptr<ItemComponent>> it;
std::vector<std::shared_ptr<SpriteComponent>>s;

bool getTime = false;
std::string storeTime;
bool itemClicked = true;
bool bondReached = false;

void Level1Scene::Load() {

    //Creates a new seed for randomization
    srand(time(NULL));

    //Loads in the 'level', used as the boundary
    cout << " Scene 1 Load" << endl;
    if (Engine::getWindowSize().x == 800)
    {
        ls::loadLevelFile("res/levels/gameScene.txt", 40.0f);
    }

    if (Engine::getWindowSize().x == 1600)
    {
        ls::loadLevelFile("res/levels/gameScene2.txt", 40.0f);
    }

    if (Engine::getWindowSize().x == 1920)
    {
        ls::loadLevelFile("res/levels/gameScene3.txt", 40.0f);
    }

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(80.f, 80.f));
        }
    }

    // Create Cat
    {
        cat = makeEntity();

        //Set cat to start position
        cat->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

        //Initialize cat with variables
        c = cat->addComponent<CatComponent>("Tabby", "Fluffy", "Female", "CannedCatFood", 4);

        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();

        if (!spritesheet->loadFromFile("res/sprites/tabbyCat.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        if (!spritesheet2->loadFromFile("res/sprites/altTabbyCat.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        //Create cat's sprite
        sp = cat->addComponent<SpriteComponent>();
        sp->setTexture(spritesheet);
        sp->getSprite().setOrigin(50.f, 50.f);

        //Add cat AI (pathfinding and state machine)
        a = cat->addComponent<CatAI>(Vector2f(40.f, 40.f), false, Vector2f(40.f, 40.f), "res/levels/gameScene.txt");
        a->PickTarget("WANDER");
        a->SetChosen(true);

    }

    //Create player
    {
        player = makeEntity();

        p = player->addComponent<PlayerComponent>("PlayerName", 2000.00);
        //Fill inventory
        p->baseInventory();
    }

    //Create in game clock
    {

        clockTime = makeEntity();
        cl = clockTime->addComponent<ClockComponent>();

    }


    //
    //Loading Ui Stuff
    //

    //Sets the shape for the shop box
    sf::ConvexShape shopShape;
    shopShape.setPointCount(5);
    shopShape.setPoint(0, sf::Vector2f(0, 0));
    shopShape.setPoint(1, sf::Vector2f(60, 0));
    shopShape.setPoint(2, sf::Vector2f(50, 30));
    shopShape.setPoint(3, sf::Vector2f(-50, 30));
    shopShape.setPoint(4, sf::Vector2f(-60, 0));

    //Sets the shape for the inventory box
    sf::ConvexShape inventoryShape;
    inventoryShape.setPointCount(5);
    inventoryShape.setPoint(0, sf::Vector2f(0, 0));
    inventoryShape.setPoint(1, sf::Vector2f(60, 0));
    inventoryShape.setPoint(2, sf::Vector2f(50, 30));
    inventoryShape.setPoint(3, sf::Vector2f(-50, 30));
    inventoryShape.setPoint(4, sf::Vector2f(-60, 0));

    //Background for inventory & shop
    boxEntity = makeEntity();
    box = boxEntity->addComponent<ShapeComponent>();
    box->setShape<sf::RectangleShape>(sf::Vector2f(400, 499));
    box->getShape().setFillColor(sf::Color::White); 
    box->getShape().setOutlineColor(sf::Color::Black);

    boxEntity->setVisible(false);

    //
    //Still UI things
    // 
    //Date Box
    date = makeEntity();
    stillUi[0] = date->addComponent<ShapeComponent>();
    date->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 2)); //Sets position of the Box based on resolution
    stillUi[0]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
    stillUi[0]->getShape().setFillColor(sf::Color::White); //Sets Colour of the Box
    stillUi[0]->getShape().setOutlineColor(sf::Color::Black);


    //Currency Box
    currency = makeEntity();
    stillUi[1] = currency->addComponent<ShapeComponent>();
    currency->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 44)); //Sets position of the Box based on resolution
    stillUi[1]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
    stillUi[1]->getShape().setFillColor(sf::Color::White); //Sets Colour of the Box
    stillUi[1]->getShape().setOutlineColor(sf::Color::Black);


    //Food Box
    food = makeEntity();
    stillUi[2] = food->addComponent<ShapeComponent>();
    food->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, 5)); //Sets position of the Box based on resolution
    stillUi[2]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
    stillUi[2]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the Box
    stillUi[2]->getShape().setOutlineColor(sf::Color::Black);
    stillUi[2]->getShape().setScale(0, 0);

    //Toys Box
    toys = makeEntity();
    stillUi[3] = toys->addComponent<ShapeComponent>();
    toys->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, 5)); //Sets position of the Box based on resolution
    stillUi[3]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
    stillUi[3]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the Box
    stillUi[3]->getShape().setOutlineColor(sf::Color::Black);
    stillUi[3]->getShape().setScale(0, 0);


    //
    //Normal UI
    // 
    //Tutorial Box
    options = makeEntity();
    ui[0] = options->addComponent<ShapeComponent>();
    options->setPosition(sf::Vector2f(Engine::getWindowSize().x - 175, 88)); //Sets position of the Box based on resolution
    ui[0]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));
    ui[0]->getShape().setOutlineColor(sf::Color::Black); //Sets Colour of the Box


    //Options Box
    tutorial = makeEntity();
    ui[1] = tutorial->addComponent<ShapeComponent>();
    tutorial->setPosition(sf::Vector2f(Engine::getWindowSize().x - 60, 88)); //Sets position of the Box based on resolution
    ui[1]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));
    ui[1]->getShape().setOutlineColor(sf::Color::Black); //Sets Colour of the Box


    //Shop
    shop = makeEntity();
    ui[2] = shop->addComponent<ShapeComponent>();
    shop->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 100, 0)); //Sets position of the Box based on resolution
    ui[2]->setShape<sf::ConvexShape>(shopShape);
    ui[2]->getShape().setOutlineColor(sf::Color::Black);


    //Inventory
    inventory = makeEntity();
    ui[3] = inventory->addComponent<ShapeComponent>();
    inventory->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 + 100, 0)); //Sets position of the Box based on resolution
    ui[3]->setShape<sf::ConvexShape>(inventoryShape);
    ui[3]->getShape().setOutlineColor(sf::Color::Black);


    //Cat Stats
    catBox = makeEntity();
    ui[4] = catBox->addComponent<ShapeComponent>();
    catBox->setPosition(sf::Vector2f(0, 88)); //Sets position of the Box based on resolution
    ui[4]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));
    ui[4]->getShape().setOutlineColor(sf::Color::White);


    //Slots for Inventory + Shop
    slot1 = makeEntity();
    slot2 = makeEntity();
    slot3 = makeEntity();
    slot4 = makeEntity();
    slot5 = makeEntity();
    slot6 = makeEntity();

    boxEntity->setPosition(sf::Vector2f(shop->getPosition().x - 100, 0)); //Sets position of the Box based on resolution
    food->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, 5)); //Sets position of the Box based on resolution
    toys->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, 5)); //Sets position of the Box based on resolution

    slot1->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 175)); //Sets position of the Box based on resolution
    slot2->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 275)); //Sets position of the Box based on resolution
    slot3->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 375)); //Sets position of the Box based on resolution
    slot4->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 175)); //Sets position of the Box based on resolution
    slot5->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 275)); //Sets position of the Box based on resolution
    slot6->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 375)); //Sets position of the Box based on resolution

    slots[0] = slot1->addComponent<ShapeComponent>();
    slots[1] = slot2->addComponent<ShapeComponent>();
    slots[2] = slot3->addComponent<ShapeComponent>();
    slots[3] = slot4->addComponent<ShapeComponent>();
    slots[4] = slot5->addComponent<ShapeComponent>();
    slots[5] = slot6->addComponent<ShapeComponent>();

    minigameEntity = makeEntity();
    minigame = minigameEntity->addComponent<ShapeComponent>();
    minigameEntity->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 75, Engine::getWindowSize().y - 50)); //Sets position of the Box based on resolution
    minigame->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
    minigame->getShape().setFillColor(sf::Color::White);

    for (int i = 0; i < 6; i++)
    {
        slots[i]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
        slots[i]->getShape().setFillColor(sf::Color::Black);
        slots[i]->getShape().setOutlineColor(sf::Color::Black);
        slots[i]->getShape().setScale(0, 0);
    }

    //
    //Text UI
    //

    //Sets the text
    string text[5];
    text[0] = "S";
    text[1] = "S&Q";
    text[2] = "Shop";
    text[3] = "Inventory";
    text[4] = "Minigame";

    //Sets the text for time and currancy
    string stillText[4];
    stillText[0] = cl->getTime();
    stillText[1] = p->getCurrency();
    stillText[2] = "";
    stillText[3] = "";

    shared_ptr<Entity> txt = makeEntity();

    //Sets the text for each UI Item
    for (int i = 0; i < 5; ++i) {

        uiText[i] = txt->addComponent<TextComponent>(
            text[i]);
        //Sets the text
        //We have an array of text which contain each item in the menu

        uiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
    }

    //Sets the text for each Still UI Item
    for (int i = 0; i < 4; ++i) {

        stillUiText[i] = txt->addComponent<TextComponent>(stillText[i]);
        stillUiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
    }

    stillUiText[0]->getText().setPosition(date->getPosition()); //Sets position of Date text
    stillUiText[1]->getText().setPosition(currency->getPosition()); //Sets position Currency text

    stillUiText[2]->getText().setPosition(food->getPosition()); //Sets position of Date text
    stillUiText[3]->getText().setPosition(toys->getPosition()); //Sets position Currency text
    stillUiText[2]->getText().setColor(sf::Color::White); //Sets colour of text
    stillUiText[3]->getText().setColor(sf::Color::White); //Sets colour of text

    uiText[0]->getText().setPosition(options->getPosition()); //Sets position of Options text
    uiText[1]->getText().setPosition(tutorial->getPosition()); //Sets position of Tutorial text

    uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x - 20, shop->getPosition().y)); //Sets position of Shop text
    uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x - 40, inventory->getPosition().y)); //Sets position of Inventory Text

    uiText[2]->getText().setScale(0.5, 0.5); //Scales the text so it fits in box
    uiText[3]->getText().setScale(0.5, 0.5); //Scales the text so it fits in box

    uiText[4]->getText().setPosition(sf::Vector2f(minigameEntity->getPosition().x, minigameEntity->getPosition().y));

    //Sets the scale for each stat text
    for (int i = 0; i < 14; ++i) {

        catStats[i] = txt->addComponent<TextComponent>();
        catStats[i]->getText().setColor(sf::Color::Black); //Sets colour of text
        catStats[i]->getText().setScale(0.5, 0.5);
    }

    //Makes mouse sprite
    std::shared_ptr<Texture> cursor = std::make_shared<sf::Texture>();;
    cursorSprite = makeEntity();

    if (!cursor->loadFromFile("res/sprites/mouse.png"))
    {
        std::cout << "Failed to Load Sprite" << std::endl;
        //Error
    }
    cursorEntity = cursorSprite->addComponent<SpriteComponent>();
    cursorEntity->setTexture(cursor);
    cursorEntity->getSprite().setPosition(0, 0);
    cursorSpeed = 0.15f;


    //Setting Variables
    keyPressed = true; //Checks if a key is pressed
    menuDropped = false; //Checks if the Shop/Inventory is dropped
    shopOpen = false; //Checks what is open, Shop or Inventory
    catOpen = false; //Checks if the cat stat menu is open
    selected = false; //Used to help with logic for the shopOpen
    hideMouse = false; //Checks if the Mouse should be hidden or not
    controller = false; //Checks if the controller is in use or the mouse
    change = false; //Checks if the scene should be changed
    sceneChange = 0; //Checks which scene should change to
    //
    //Loading Ui Stuff
    //

    //
    //Sound Stuff
    //
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("res/sounds/Dropmetalthing.ogg"));
    {
        std::cout << "Failed to Load Sound" << std::endl;
        //Error
    }

    paying.setBuffer(buffer);
    

    //
    //Sound Stuff
    //

    Level1Scene::LoadGame();
    //Simulate long loading times
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::UnLoad() {
    cout << "Scene 1 Unload" << endl;
    cat.reset();
    player.reset();
    clockTime.reset();
    c.reset();
    sp.reset();
    a.reset();
    p.reset();
    cl.reset();

    items.clear();
    it.clear();
    s.clear();
    itemLocations.clear();
    itemType.clear();
    item.reset();

    //
    //Resets UI Stuff
    //
    for (int i = 0; i < 4; ++i)
    {
        uiText[i].reset();
        stillUi[i].reset();
        stillUiText[i].reset();
    }
    uiText[4].reset();
    date.reset();
    currency.reset();
    options.reset();
    tutorial.reset();
    shop.reset();
    inventory.reset();
    cursorEntity.reset();
    cursorSprite.reset();
    box.reset();
    boxEntity.reset();
    food.reset();
    toys.reset();

    for (int i = 0; i < 6; i++)
    {
        ui[i].reset();
        slots[i].reset();
    }

    slot1.reset();
    slot2.reset();
    slot3.reset();
    slot4.reset();
    slot5.reset();
    slot6.reset();

    for (int i = 0; i < 14; i++)
    {
        catStats[i].reset();
    }

    minigame.reset();
    minigameEntity.reset();
    //
    //Resets Ui Stuff
    //

    //
    //Resets Sound Stuff
    //

    //
    //Resets Sound Stuff
    //

    Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    //
    //Ui Stuff
    //
    //Sets the mouse visibility
    Engine::GetWindow().setMouseCursorVisible(hideMouse);
    //Checks to see if a controller is used or a mouse
    if (!controller)
    {
        MouseUpdate();
    }
    else if (controller)
    {
        KeyboardUpdate();
    }
    //Checks if the cat stats are open or not
    if (catOpen)
    {
        GetStats();
    }
    if (!catOpen)
    {
        HideStats();
    }
    Controls();
    Highlight();
    //Displays currancy to only 2 decimal points
    ostringstream streamObj3;
    streamObj3 << std::fixed;
    streamObj3 << std::setprecision(2);
    streamObj3 << p->getCurrency();
    string money = streamObj3.str();
    stillUiText[0]->SetText(cl->getTime());
    stillUiText[1]->SetText("$ " + money);
    //
    //Ui Stuff
    //
    
    //If inventory item selected, get key from inventory item
    if (itemClicked)
    {
        itemClicked = false;
        item = makeEntityLast();
        item->addTag("last");

        //Create item using key from inventory
        auto j = item->addComponent<ItemComponent>(storeItem);

        j->baseItems();

        //Create sprite for item
        auto is = item->addComponent<SpriteComponent>();

        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();

        if (!spritesheet->loadFromFile(j->GetSpriteLocation())) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        is->setTexture(spritesheet);
        is->getSprite().setOrigin(50.f, 50.f);

        //Choose random spot to place item on screen
        auto empty = ls::findTiles(ls::EMPTY);

        int randomIndex = rand() % empty.size();

        Vector2f itemLocation = ls::getTilePosition(empty[randomIndex]) + Vector2f(20.f, 20.f);

        item->setPosition(itemLocation);

        //Add item to list of items
        it.push_back(j);
        s.push_back(is);
        items.push_back(item);
        itemLocations.push_back(itemLocation);
        itemType.push_back(storeItem);
    }


    //Cat's detection range for items
    int range = 40;

    
    //For each item..
    if (!items.empty() && cat != NULL)
    {
        for (int k = 0; k < items.size(); k++)
        {
            //Calculate if the cat is nearby each item 
            if ((cat->getPosition().y > items[k]->getPosition().y - range && cat->getPosition().y < items[k]->getPosition().y + range) && (cat->getPosition().x > items[k]->getPosition().x - range && cat->getPosition().x < items[k]->getPosition().x + range))
            {
       
                if (!(it[k]->GetStats()).empty())
                {
                    //If cat gets close to item, it eats the item and gains stats depending on what it ate
                    c->gainStats(it[k]->GetStats());
                }
                

                //Delete item and remove from lists
                items[k]->setForDelete();
                items.erase(items.begin() + k);
                it.erase(it.begin() + k);
                s.erase(s.begin() + k);
                itemLocations.erase(itemLocations.begin() + k);
                itemType.erase(itemType.begin() + k);
            }
        }
    }
    
    //Store current time
    if (!getTime)
    {
        storeTime = cl->getTime();
        getTime = true;
    }

    //When time passes
    if (storeTime != cl->getTime())
    {
        //Drain cat stats over time
        c->SetHunger(c->getHunger() - 2);

        c->SetMood(c->getMood() - 1);

        c->SetCleanliness(c->getCleanliness() - 2);

         //Cats health will worsen if not cleaned or fed
        if (c->getHunger() < 30 || c->getCleanliness() < 30)
        {
            c->SetHealth(c->getHealth() - 5);
        }

        if (c->getHealth() < 100)
        {
            bondTracker = 0;
        }
        else
        {
            bondTracker++;
        }

        if (bondTracker >= 3)
        {
            c->SetBond(c->getBond() + 1);
        }

        if (c->getBond() == 100.0f)
        {
            bondReached = true;
            bondTracker = 0;
        }

        if (bondReached)
        {
            sp->setTexture(spritesheet2);
            sp->getSprite().setOrigin(50.f, 50.f);
        }

        getTime = false;
    }

    //If the cat is waiting for a new position to go to
    if (a->GetPause() == true)
    {
        //If the cat is hungry, and an item is on screen, and it is not the current target square
        if (c->getHunger() < 30.0f && !itemLocations.empty() && a->GetTarget() != previousTarget)
        {
            foodFound = false;
            for (int k = 0; k < items.size(); k++)
            {
                if (it[k]->GetType() == "Food")
                {
                    foodFound = true;
                    a->PickTarget("HUNGRY", itemLocations[k]);
                    previousTarget = itemLocations[k];
                    break;
                }
            }

            //Otherwise, wander to a random square
            if (!foodFound)
            {
                a->PickTarget("WANDER");
            }
        }
        else if (c->getCleanliness() < 20.0f && !itemLocations.empty() && a->GetTarget() != previousTarget)
        {
            bathFound = false;
            for (int k = 0; k < items.size(); k++)
            {
                if (it[k]->GetKey() == "CatBath")
                {
                    bathFound = true;
                    a->PickTarget("DIRTY", itemLocations[k]);
                    previousTarget = itemLocations[k];
                    break;
                }
            }

            //Otherwise, wander to a random square
            if (!bathFound)
            {
                a->PickTarget("WANDER");
            }
        }
        else 
        {
            //Otherwise, wander to a random square
            a->PickTarget("WANDER");
        }
      
        a->SetChosen(true);
    }

    //If scene should be changed
    if (change)
    {
        SaveGame();
        SceneChange();
    }

    Scene::Update(dt);
}

void Level1Scene::Render() {

    ls::render(Engine::GetWindow());
    Scene::Render();
}

void Level1Scene::SaveGame() {

    {
        //Save Cat Data
        std::ofstream saveFile("catFile.txt", std::ofstream::out);

        saveFile << c->getHealth() << " ";
        saveFile << c->getMood() << " ";
        saveFile << c->getHunger() << " ";
        saveFile << c->getCleanliness() << " ";
        saveFile << c->getAgility() << " ";
        saveFile << c->getPower() << " ";
        saveFile << c->getStamina() << " ";
        saveFile << c->getBond() << " ";
        saveFile << c->getType() << " ";
        saveFile << c->getName() << " ";
        saveFile << c->getSex() << " ";
        saveFile << c->getFaveFood() << " ";
        saveFile << c->getAge() << " ";

        saveFile.close();
    }

    {
        //Save Player Data
        std::ofstream saveFile("playerFile.txt", std::ofstream::out);

        saveFile << p->getName() << " ";
        saveFile << p->getCurrency() << " ";


        saveFile.close();

        //Save Player Inventory Data
        std::ofstream invFile("invFile.txt", std::ofstream::out);

        auto m = p->getInventory();

        for (auto& t : m)
        {
            invFile << t.first << " ";
            invFile << t.second << "\n";
        }

        invFile.close();
    }

    {
        //Save Clock Data
        std::ofstream clockFile("clockFile.txt", std::ofstream::out);

        clockFile << cl->getTime();

        clockFile.close();
    }
}

void Level1Scene::LoadGame() {

    {
        //Load Cat Data
        ifstream myfile("catFile.txt");
        if (!myfile)
        {
            cout << "Failed to find file" << endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            myfile.seekg(0, ios::end);
            size_t size = myfile.tellg();
            if (size == 0)
            {
                cout << "File is empty\n";
            }
            else
            {
                myfile.seekg(0, ios::beg);
                float hp = 0, mood = 0, hng = 0, cln = 0, agl = 0, pwr = 0, stm = 0, bond = 0;
                string typ = "", name = "", sex = "", faveFood = "";
                int age = 0;
                if (myfile >> hp >> mood >> hng >> cln >> agl >> pwr >> stm >> bond >> typ >> name >> sex >> faveFood >> age)
                {
                    c->SetHealth(hp);
                    c->SetMood(mood);
                    c->SetHunger(hng);
                    c->SetCleanliness(cln);
                    c->SetAgility(agl);
                    c->SetPower(pwr);
                    c->SetStamina(stm);
                    c->SetBond(bond);
                    c->SetType(typ);
                    c->SetName(name);
                    c->SetSex(sex);
                    c->SetFaveFood(faveFood);
                    c->SetAge(age);
                }
                else
                {
                    cout << "Failed to load from file" << endl;
                }
            }
        }
    }

    {
        //Load Player Data
        ifstream myfile("playerFile.txt");
        if (!myfile)
        {
            cout << "Failed to find file" << endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            myfile.seekg(0, ios::end);
            size_t size = myfile.tellg();
            if (size == 0)
            {
                cout << "File is empty\n";
            }
            else
            {
                myfile.seekg(0, ios::beg);
                string name = "";
                double dbl = 0.0;

                int age = 0;
                if (myfile >> name >> dbl)
                {
                    p->SetName(name);
                    p->SetCurrency(dbl);
                }
                else
                {
                    cout << "Failed to load from file" << endl;
                }
            }
        }

        //Load inventory data
        ifstream invfile("invFile.txt");
        if (!invfile)
        {
            cout << "Failed to find file" << endl;
        }
        else
        {
            invfile.seekg(0, ios::end);
            size_t size = invfile.tellg();
            if (size == 0)
            {
                cout << "File is empty\n";
            }
            else
            {
                invfile.seekg(0, ios::beg);
                std::map<std::string, int> params;
                std::string key;
                int value;
                while (invfile >> key >> value)
                {
                    params[key] = value;
                }


                p->SetInventory(params);
            }
        }
    }

    {
        //Load clock data
        ifstream clockfile("clockFile.txt");
        if (!clockfile)
        {
            cout << "Failed to find file" << endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            clockfile.seekg(0, ios::end);
            size_t size = clockfile.tellg();
            if (size == 0)
            {
                cout << "File is empty\n";
            }
            else
            {
                //CHECK FILE NOT EMPTY
                clockfile.seekg(0, ios::beg);
                std::string time;
                std::string holdAMPM;

                if (clockfile >> time >> holdAMPM)
                {
                    time = time + " " + holdAMPM;
                }
                else
                {
                    cout << "Failed to load from file" << endl;
                }

                std::string hr = time.substr(0, 2);
                std::string min = time.substr(3, 1);

                cl->SetTime(time, std::stoi(hr), std::stoi(min), holdAMPM);
            }
        }
    }
}

//
//UI Stuff Below
//

//Highlights the selected box when hovering over it
void Level1Scene::Highlight()
{
    for (int i = 0; i < 5; i++)
    {
        //Normal UI
        if (ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
        {
            ui[i]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
            if (i < 4)
            {
                uiText[i]->getText().setColor(sf::Color::Red); //Sets colour of text
            }
        }
        else
        {
            ui[i]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
            if (i < 4)
            {
                uiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        //Shop Items
        if (slots[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
        {
            slots[i]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
        }
        else
        {
            slots[i]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the hitboxes
        }
    }

    //If shop is open then shop box stays highlighted
    if (shopOpen && menuDropped)
    {
        ui[2]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
        uiText[2]->getText().setColor(sf::Color::Red); //Sets colour of text
    }

    //If inventory is open then inventory box stays highlighted
    else if (!shopOpen && menuDropped)
    {
        ui[3]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
        uiText[3]->getText().setColor(sf::Color::Red); //Sets colour of text
    }

    //If cat stats are open then stays highlighted
    if (catOpen)
    {
        ui[4]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
    }

    //Minigame Box
    if (minigame->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
    {
        minigame->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
        uiText[4]->getText().setColor(sf::Color::Red); //Sets colour of text
    }
    else
    {
        minigame->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
        uiText[4]->getText().setColor(sf::Color::Black); //Sets colour of text
    }
}

//Does the controls for the UI
void Level1Scene::Controls()
{
    //Mouse Controls
    if (!controller)
    {
        for (int i = 0; i < 5; i++)
        {
            //Normal UI
            if (!keyPressed && sf::Mouse::isButtonPressed(Mouse::Left) && ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
            {
                keyPressed = true;
                if (i == 0)
                {
                    SaveGame();
                }
                if (i == 1)
                {
                    sceneChange = 0;
                    change = true;
                }
                else if (i == 2)
                {
                    shopOpen = true;
                    MenuDrop();
                }
                else if (i == 3)
                {
                    shopOpen = false;
                    MenuDrop();
                }
                else if (i == 4)
                {
                    OpenCat();
                }
            }
        }

        for (int i = 0; i < 6; i++)
        {
            //Item/Inventory Boxes
            if (!keyPressed && sf::Mouse::isButtonPressed(Mouse::Left) && slots[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
            {
                //Inventory UI + Using
                keyPressed = true;
                if (i == 0 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("CannedCatFood") > 0)
                    {
                        //Take one away from the inventory and trigger the spawn
                        p->changeItem("CannedCatFood", p->getItem("CannedCatFood") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "CannedCatFood";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 100 && p->getItem("CannedCatFood") < 99)
                    {
                        //Decrease the player currency and increase the item in inventory
                        p->SetCurrency(p->getCurrency() - 100);
                        p->changeItem("CannedCatFood", p->getItem("CannedCatFood") + 1);
                        paying.play();
                    }
                }
                if (i == 1 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("Fish") > 0)
                    {
                        p->changeItem("Fish", p->getItem("Fish") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "Fish";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 60 && p->getItem("Fish") < 99)
                    {
                        p->SetCurrency(p->getCurrency() - 60);
                        p->changeItem("Fish", p->getItem("Fish") + 1);
                    }
                }
                if (i == 2 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("Catnip") > 0)
                    {
                        p->changeItem("Catnip", p->getItem("Catnip") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "Catnip";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 150 && p->getItem("Catnip") < 99)
                    {
                        p->SetCurrency(p->getCurrency() - 150);
                        p->changeItem("Catnip", p->getItem("Catnip") + 1);
                    }
                }
                if (i == 3 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("YarnBall") > 0)
                    {
                        p->changeItem("YarnBall", p->getItem("YarnBall") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "YarnBall";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 50 && p->getItem("YarnBall") < 99)
                    {
                        p->SetCurrency(p->getCurrency() - 50);
                        p->changeItem("YarnBall", p->getItem("YarnBall") + 1);
                    }
                }
                if (i == 4 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("FishPlush") > 0)
                    {
                        p->changeItem("FishPlush", p->getItem("FishPlush") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "FishPlush";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 80 && p->getItem("FishPlush") < 99)
                    {
                        p->SetCurrency(p->getCurrency() - 80);
                        p->changeItem("FishPlush", p->getItem("FishPlush") + 1);
                    }
                }
                if (i == 5 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("CatBath") > 0)
                    {
                        p->changeItem("CatBath", p->getItem("CatBath") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "CatBath";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 200 && p->getItem("CatBath") < 99)
                    {
                        p->SetCurrency(p->getCurrency() - 200);
                        p->changeItem("CatBath", p->getItem("CatBath") + 1);
                    }
                }
            }
        }
        if (!keyPressed && sf::Mouse::isButtonPressed(Mouse::Left) && minigame->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
        {
            keyPressed = true;
            std::cout << "Minigame" << std::endl;
        }
    }

    //Keyboard Controls
    if (controller)
    {
        for (int i = 0; i < 5; i++)
        {
            //Normal UI
            if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Enter) && ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
            {
                keyPressed = true;
                if (i == 0)
                {
                    SaveGame();
                }
                if (i == 1)
                {
                    change = true;
                }
                else if (i == 2)
                {
                    shopOpen = true;
                    MenuDrop();
                }
                else if (i == 3)
                {
                    shopOpen = false;
                    MenuDrop();
                }
                else if (i == 4)
                {
                    OpenCat();
                }
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Enter) && slots[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
            {
                //Inventory UI + Using
                keyPressed = true;
                if (i == 0 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("CannedCatFood") > 0)
                    {
                        p->changeItem("CannedCatFood", p->getItem("CannedCatFood") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "CannedCatFood";
                    }

                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 100 && p->getItem("CannedCatFood") < 99)
                    {

                        p->getCurrency() -= 100;
                        p->changeItem("CannedCatFood", p->getItem("CannedCatFood") + 1);
                    }
                }
                if (i == 1 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("Fish") > 0)
                    {
                        p->changeItem("Fish", p->getItem("Fish") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "Fish";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 60 && p->getItem("Fish") < 99)
                    {
                        p->getCurrency() -= 60;
                        p->changeItem("Fish", p->getItem("Fish") + 1);
                    }
                }
                if (i == 2 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("Catnip") > 0)
                    {
                        p->changeItem("Catnip", p->getItem("Catnip") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "Catnip";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 150 && p->getItem("Catnip") < 99)
                    {
                        p->getCurrency() -= 150;
                        p->changeItem("Catnip", p->getItem("Catnip") + 1);
                    }
                }
                if (i == 3 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("YarnBall") > 0)
                    {
                        p->changeItem("YarnBall", p->getItem("YarnBall") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "YarnBall";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 50 && p->getItem("YarnBall") < 99)
                    {
                        p->getCurrency() -= 50;
                        p->changeItem("YarnBall", p->getItem("YarnBall") + 1);
                    }
                }
                if (i == 4 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("FishPlush") > 0)
                    {
                        p->changeItem("FishPlush", p->getItem("FishPlush") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "FishPlush";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 80 && p->getItem("FishPlush") < 99)
                    {
                        p->getCurrency() -= 80;
                        p->changeItem("FishPlush", p->getItem("FishPlush") + 1);
                    }
                }
                if (i == 5 && menuDropped)
                {
                    //If Inventory is Open
                    if (!shopOpen && p->getItem("CatBath") > 0)
                    {
                        p->changeItem("CatBath", p->getItem("CatBath") - 1);
                        itemClicked = true;
                        menuDropped = false;
                        MoveBack();
                        UnLoadMenu();
                        storeItem = "CatBath";
                    }
                    //If Shop is Open
                    if (shopOpen && p->getCurrency() >= 200 && p->getItem("CatBath") < 99)
                    {
                        p->getCurrency() -= 200;
                        p->changeItem("CatBath", p->getItem("CatBath") + 1);
                    }
                }
            }
        }
        if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Enter) && minigame->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
        {
            keyPressed = true;
            std::cout << "Minigame" << std::endl;
        }
    }


    if (!keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
    {
        keyPressed = true;
        if (controller)
        {
            controller = false;
            hideMouse = !hideMouse;
        }
        if (!controller)
        {
            controller = true;
            cursorSprite->setPosition(sf::Vector2f(Engine::getWindowSize().x/2, Engine::getWindowSize().y/2));
            hideMouse = !hideMouse;
        }
    }

    if (!sf::Mouse::isButtonPressed(Mouse::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F3) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        keyPressed = false;
    }
}

//Drops down the Shops/Inventory Menus
//Also checks what menus are open and what should be closed
void Level1Scene::MenuDrop()
{
    if (menuDropped)
    {
        if (selected == shopOpen)
        {
            menuDropped = false;
            MoveBack();
            UnLoadMenu();
        }
        else
        {
            selected = shopOpen;
            LoadMenu();
        }
    }
    else if (!menuDropped)
    {
        if (catOpen)
        {
            catOpen = false;
            catBox->setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x - 300, ui[4]->getShape().getPosition().y));
        }
        menuDropped = true;
        shop->setPosition(sf::Vector2f(ui[2]->getShape().getPosition().x, ui[2]->getShape().getPosition().y + 500));
        inventory->setPosition(sf::Vector2f(ui[3]->getShape().getPosition().x, ui[3]->getShape().getPosition().y + 500));
        uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x - 20, shop->getPosition().y)); //Sets position of the text based on resolution
        uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x - 40, inventory->getPosition().y)); //Sets position of the text based on resolution
        selected = shopOpen;
        LoadMenu();
    }
}

//Sets the correct position for each items for the correct menu
void Level1Scene::LoadMenu()
{
    if (menuDropped)
    {
        //Background for inventory & shop
        boxEntity->setPosition(sf::Vector2f(shop->getPosition().x - 100, 0)); //Sets position of the hitboxes based on resolution
        box->setShape<sf::RectangleShape>(sf::Vector2f(400, 499));
        boxEntity->setVisible(true);

        //Sets the position for each of the shop/inventory items boxes and text
        //(Reusing catstats text instead of new variables)
        for (int i = 0; i < 6; i++)
        {
            slots[i]->getShape().setScale(1, 1);
            catStats[i]->getText().setPosition(slots[i]->getShape().getPosition());
            catStats[i]->getText().setColor(sf::Color::White);
            if (shopOpen)
            {
                catStats[i + 6]->getText().setPosition(sf::Vector2f(slots[i]->getShape().getPosition().x, slots[i]->getShape().getPosition().y + 20));
                catStats[i + 6]->getText().setColor(sf::Color::White);
            }
            else
            {
                catStats[i + 6]->SetText("");
            }
        }

        stillUi[2]->getShape().setScale(1, 1);
        stillUi[3]->getShape().setScale(1, 1);
        stillUiText[2]->SetText("Food");
        stillUiText[3]->SetText("Toys");
    }

    //Changes box position for the cat stats background
    if (catOpen)
    {
        boxEntity->setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x, ui[4]->getShape().getPosition().y)); //Sets position of the hitboxes based on resolution
        box->setShape<sf::RectangleShape>(sf::Vector2f(300, 300));
        boxEntity->setVisible(true);
    }
}

//Unloads the items that are in menus
void Level1Scene::UnLoadMenu()
{

    boxEntity->setVisible(false);

    for (int i = 0; i < 6; i++)
    {
        slots[i]->getShape().setScale(0, 0);
        catStats[i]->SetText("");
    }
    stillUi[2]->getShape().setScale(0, 0);
    stillUi[3]->getShape().setScale(0, 0);
    stillUiText[2]->SetText("");
    stillUiText[3]->SetText("");
}

//Sets the position for cat stats
void Level1Scene::OpenCat()
{
    if (!catOpen)
    {
        //Checks if a menu is open and closes that menu
        if (menuDropped)
        {
            menuDropped = false;
            MoveBack();
            UnLoadMenu();
        }
        catOpen = true;
        LoadMenu();
        catBox->setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x + 300, ui[4]->getShape().getPosition().y));

        //Sets each text equal distances from each other
        int add = 0;
        for (int i = 0; i < 13; i++)
        {
            if (i == 5)
            {
                add = 0;
            }
            if (i > 4)
            {
                add += 25;
                catStats[i]->getText().setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x + 175, ui[4]->getShape().getPosition().y + add));
            }
            else
            {
                add += 25;
                catStats[i]->getText().setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x, ui[4]->getShape().getPosition().y + add));
            }
            catStats[i]->getText().setColor(sf::Color::Black);
        }

        //Player Character
        catStats[13]->getText().setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x, ui[4]->getShape().getPosition().y + 250));
        catStats[13]->getText().setColor(sf::Color::Black);
    }

    else if (catOpen)
    {
        catOpen = false;
        catBox->setPosition(sf::Vector2f(ui[4]->getShape().getPosition().x - 300, ui[4]->getShape().getPosition().y));
        UnLoadMenu();
    }
}

//Sets the text for each stat
void Level1Scene::GetStats()
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2);
    std::string str;

    catStats[0]->SetText("N:" + c->getName());

    catStats[1]->SetText("T:" + c->getType());

    catStats[2]->SetText("S:" + c->getSex());

    str = std::to_string(c->getAge());
    catStats[3]->SetText("Age:" + str);

    catStats[4]->SetText("Fav:" + c->getFaveFood());

    stream << std::fixed << std::setprecision(0) << c->getHealth();
    str = stream.str();
    catStats[5]->SetText("HP:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getMood();
    str = stream.str();
    catStats[6]->SetText("Mood:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getHunger();
    str = stream.str();
    catStats[7]->SetText("Hunger:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getCleanliness();
    str = stream.str();
    catStats[8]->SetText("Clean:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getAgility();
    str = stream.str();
    catStats[9]->SetText("Speed:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getPower();
    str = stream.str();
    catStats[10]->SetText("Power:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getStamina();
    str = stream.str();
    catStats[11]->SetText("Stamina:" + str + "/100");
    stream.str(std::string());

    stream << std::fixed << std::setprecision(0) << c->getBond();
    str = stream.str();
    catStats[12]->SetText("Bond:" + str + "/100");
    stream.str(std::string());

    str = p->getName();
    catStats[13]->SetText("Owner:" + str);
}

//Hide stats if the cat window is closed
void Level1Scene::HideStats()
{
    //Reusing the catStats for the item/inventory box texts
    if (menuDropped)
    {
        if (!shopOpen)
        {
            catStats[0]->SetText("CannedFood x" + to_string(p->getItem("CannedCatFood")));
            catStats[1]->SetText("Fish x" + to_string(p->getItem("Fish")));
            catStats[2]->SetText("Catnip x" + to_string(p->getItem("Catnip")));
            catStats[3]->SetText("YarnBall x" + to_string(p->getItem("YarnBall")));
            catStats[4]->SetText("FishPlush x" + to_string(p->getItem("FishPlush")));
            catStats[5]->SetText("CatBath x" + to_string(p->getItem("CatBath")));

            for (int i = 6; i < 14; i++)
            {
                catStats[i]->SetText("");
            }
        }

        else if (shopOpen)
        {
            catStats[0]->SetText("CannedFood $100");
            catStats[1]->SetText("Fish $60");
            catStats[2]->SetText("Catnip $150");
            catStats[3]->SetText("YarnBall $50");
            catStats[4]->SetText("FishPlush $80");
            catStats[5]->SetText("CatBath $200");

            catStats[6]->SetText("x" + to_string(p->getItem("CannedCatFood")));
            catStats[7]->SetText("x" + to_string(p->getItem("Fish")));
            catStats[8]->SetText("x" + to_string(p->getItem("Catnip")));
            catStats[9]->SetText("x" + to_string(p->getItem("YarnBall")));
            catStats[10]->SetText("x" + to_string(p->getItem("FishPlush")));
            catStats[11]->SetText("x" + to_string(p->getItem("CatBath")));

            catStats[12]->SetText("");
            catStats[13]->SetText("");
        }
    }

    else
    {
        for (int i = 0; i < 14; i++)
        {
            catStats[i]->SetText("");
        }
    }
}

//Updates the sprite location to the mouse position
void Level1Scene::MouseUpdate()
{
    cursorSprite->setPosition(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()).x - 7.0f, sf::Mouse::getPosition(Engine::GetWindow()).y));
}

//Moves the sprite with Keyboard buttons
void Level1Scene::KeyboardUpdate()
{
    //Move cursor Down
    if (cursorSprite->getPosition().y <= Engine::getWindowSize().y - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x, cursorSprite->getPosition().y + cursorSpeed));
    }

    //Move cursor Up
    if (cursorSprite->getPosition().y >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x, cursorSprite->getPosition().y - cursorSpeed));
    }

    //Move cursor Right
    if (cursorSprite->getPosition().x <= Engine::getWindowSize().x - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x + cursorSpeed, cursorSprite->getPosition().y));
    }

    //Move cursor Left
    if (cursorSprite->getPosition().x >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x - cursorSpeed, cursorSprite->getPosition().y));
    }
}

//Changes the scene
void Level1Scene::SceneChange()
{
    if (sceneChange == 0)
    {
        Engine::GetWindow().setMouseCursorVisible(true);
        Engine::ChangeScene(&menu);
    }
    else
    {
        Engine::GetWindow().setMouseCursorVisible(true);
        Engine::ChangeScene(&menu);
    }
}

//Moves Shop and Inventory Back to original spot
void Level1Scene::MoveBack()
{
    shop->setPosition(sf::Vector2f(ui[2]->getShape().getPosition().x, ui[2]->getShape().getPosition().y - 500));
    inventory->setPosition(sf::Vector2f(ui[3]->getShape().getPosition().x, ui[3]->getShape().getPosition().y - 500));
    uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x - 20, shop->getPosition().y)); //Sets position of the text based on resolution
    uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x - 40, inventory->getPosition().y)); //Sets position of the text based on resolution
}