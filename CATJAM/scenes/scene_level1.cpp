#include "scene_level1.h"
#include "../components/cmp_cat.h"
#include "../components/cmp_player.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_clock.h"
#include "../components/cmp_cat_AI.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> cat;
static shared_ptr<Entity> player;
static shared_ptr<Entity> clockTime;
std::shared_ptr<CatComponent> c;
std::shared_ptr<PlayerComponent> p;
std::shared_ptr<ClockComponent> cl;

void Level1Scene::Load() {

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
        cat->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        c = cat->addComponent<CatComponent>("Tabby", "Fluffy", "Female", "CannedCatFood", 4);
       
        auto sh = cat->addComponent<ShapeComponent>();
        sh->setShape<sf::RectangleShape>(Vector2f(60.f, 40.f));
        sh->getShape().setFillColor(Color::Blue); 
        sh->getShape().setOrigin(30.f, 20.f);
   
        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();;

        if (!spritesheet->loadFromFile("res/sprites/tabbyCat.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        auto sp = cat->addComponent<SpriteComponent>();
        sp->setTexture(spritesheet);
        sp->getSprite().setOrigin(50.f, 50.f);
        
        auto a = cat->addComponent<CatAI>(Vector2f(60.f, 40.f));
        a->PickWanderLocation();
    }

    {
        player = makeEntity();

        p = player->addComponent<PlayerComponent>("PlayerName", 2000.00);
        p->baseInventory();
    }

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
    p.reset();
    cl.reset();
    Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {


    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        Engine::ChangeScene(&menu);
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
