#include "scene_level1.h"
#include "../components/cmp_cat.h"
#include "../components/cmp_player.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_clock.h"
#include "../components/cmp_cat_AI.h"
#include "../components/cmp_item.h";
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>

using namespace std;
using namespace sf;

static shared_ptr<Entity> cat;
std::shared_ptr<CatComponent> c;
std::shared_ptr<CatAI> a;

static shared_ptr<Entity> player;
std::shared_ptr<PlayerComponent> p;

static shared_ptr<Entity> clockTime;
std::shared_ptr<ClockComponent> cl;

std::vector<shared_ptr<Entity>> items;
static shared_ptr<Entity> item;
std::vector<Vector2f> itemLocations;
sf::Vector2f previousTarget;
std::string storeItem;

std::vector<std::shared_ptr<ItemComponent>> i;
std::vector<std::shared_ptr<SpriteComponent>>s;


bool getTime = false;
std::string storeTime;
bool itemClicked = true;


void Level1Scene::Load() {
    
    //Creates a new seed for randomization
    srand(time(NULL));

    //Loads in the 'level', used as the boundary
    cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/levels/gameScene.txt", 40.0f);

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
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
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

        //Create cat's sprite
        auto sp = cat->addComponent<SpriteComponent>();
        sp->setTexture(spritesheet);
        sp->getSprite().setOrigin(50.f, 50.f);

        //Add cat AI (pathfinding and state machine)
        a = cat->addComponent<CatAI>(Vector2f(60.f, 40.f));
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

    Level1Scene::LoadGame();
    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::UnLoad() {
    cout << "Scene 1 Unload" << endl;
    Level1Scene::SaveGame();
    cat.reset();
    player.reset();
    clockTime.reset();
    c.reset();
    a.reset();
    p.reset();
    cl.reset();

    for (int k = 0; k < i.size(); k++)
    {
        items[k].reset();
        i[k].reset();
        s[k].reset();
    }
    itemLocations.clear();
    Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    //If inventory item selected, get key from inventory item
    if (itemClicked)
    {
        itemClicked = false;
        storeItem = "CannedCatFood";
    }

    //If inventory item selected, create item
    if (sf::Keyboard::isKeyPressed(Keyboard::Space))
    {

        item = makeEntity();

        //Create item using key from inventory
        auto j = item->addComponent<ItemComponent>(storeItem);

        j->baseItems();

        //Create sprite for item
        auto is = item->addComponent<SpriteComponent>();

        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();

        if (!spritesheet->loadFromFile(j->GetSpriteLocation(storeItem))) {
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
        i.push_back(j);
        s.push_back(is);
        items.push_back(item);
        itemLocations.push_back(itemLocation);
    }


    //Cat's detection range for items
    int range = 40;

    //For each item..
    for (int k = 0; k < items.size(); k++)
    {
        //Calculate if the cat is nearby each item (HEAVILY INEFFICIENT, NEEDS REWORK)
        if ((cat->getPosition().y > items[k]->getPosition().y - range && cat->getPosition().y < items[k]->getPosition().y + range) && (cat->getPosition().x > items[k]->getPosition().x - range && cat->getPosition().x < items[k]->getPosition().x + range))
        {
            //If cat gets close to item, it eats the item and gains stats depending on what it ate
            c->gainStats(i[k]->GetStats(i[k]->GetKey()));

            //Delete item and remove from lists
            items[k]->setForDelete();
            items.erase(items.begin() + k);
            i.erase(i.begin() + k);
            s.erase(s.begin() + k);
            itemLocations.erase(itemLocations.begin() + k);
        }
    }

    //c->SetHunger(40.0f);

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
        c->SetHunger(c->getHunger() - 5);

        c->SetMood(c->getMood() - 1);

       // c->SetCleanliness(c->getCleanliness() - 5);

        //Cats health will worsen if not cleaned or fed
        if (c->getHunger() < 30 || c->getCleanliness() < 30)
        {
            c->SetHealth(c->getHealth() - 5);
        }

        getTime = false;
    }

    //If the cat is waiting for a new position to go to
    if (a->GetPause() == true)
    {
        //If the cat is hungry, and an item is on screen, and it is not the current target square
        if (c->getHunger() < 30.0f && !itemLocations.empty() && a->GetTarget() != previousTarget)
        {
            //Walk to a random item on screen
            int itemIndex = rand() % itemLocations.size();
            a->PickTarget("HUNGRY", itemLocations[itemIndex]);
            previousTarget = itemLocations[itemIndex];
        }
        //Otherwise, wander to a random square
        else
        {
            a->PickTarget("WANDER");
        }

        a->SetChosen(true);
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

    {
        //Load Player Data
        ifstream myfile("playerFile.txt");
        if (!myfile)
        {
            cout << "Failed to find file" << endl;
        }

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
  
        //Load inventory data
        ifstream invfile("invFile.txt");
        if (!invfile)
        {
            cout << "Failed to find file" << endl;
        }

        std::map<std::string, int> params;
        std::string key;
        int value;
        while (invfile >> key >> value)
        {
            params[key] = value;
        }

        p->SetInventory(params);
    }

    {
        //Load clock data
        ifstream clockfile("clockFile.txt");
        if (!clockfile)
        {
            cout << "Failed to find file" << endl;
        }

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
