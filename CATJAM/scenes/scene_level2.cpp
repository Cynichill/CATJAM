#include "scene_level2.h"
#include "../components/cmp_cat.h"
#include "../components/cmp_player.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_clock.h"
#include "../components/cmp_cat_AI.h"
#include "../components/cmp_item.h";
#include "../components/cmp_text.h"
#include "../components/cmp_enemy.h"
#include "../components/cmp_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <thread>
#include <ctime>
#include <chrono>

using namespace std;
using namespace sf;

/*
UI STUFF
*/

shared_ptr<Entity> txt;
shared_ptr<Entity> txt2;
shared_ptr<TextComponent> score;
shared_ptr<TextComponent> hiScore;


/*
UI STUFF
*/

static shared_ptr<Entity> cat2;
std::shared_ptr<CatComponent> c2;
std::shared_ptr<CatAI> a2;
std::shared_ptr<SpriteComponent> sp2;
std::shared_ptr<sf::Texture> spritesheet3 = std::make_shared<sf::Texture>();

static shared_ptr<Entity> player2;
std::shared_ptr<PlayerComponent> p2;

shared_ptr<SpriteComponent> cursorEntity2;
std::shared_ptr<Entity> cursorSprite2;

bool _startTime = false;
std::chrono::time_point<std::chrono::system_clock> _start;

bool lose = false;

long long store;

int i = 0;
int highScore = 0;
int randomIndex = 0;
int numberOfEnemies = 0;
sf::Vector2f enemySpawnLocation;

std::vector<shared_ptr<Entity>> enemyEnts;
static shared_ptr<Entity> enem;
std::vector<std::shared_ptr<Enemy>> it2;
std::vector<std::shared_ptr<SpriteComponent>>s2;
std::vector<Vector2f> enemyLocations;

bool reroll = false;

void Level2Scene::Load()
{

    //Creates a new seed for randomization
    srand(time(NULL));

    //Loads in the 'level', used as the boundary
    std::cout << " Scene 2 Load" << std::endl;
    ls::loadLevelFile("res/levels/minigame.txt", 40.0f);

    auto h = Engine::getWindowSize().x - (ls::getWidth() * 40.f);
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(h, ho));

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
        cat2 = makeEntity();

        //Set cat to start position
        cat2->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

        //Initialize cat with variables
        c2 = cat2->addComponent<CatComponent>("Tabby", "Fluffy", "Female", "CannedCatFood", 4);

        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();

        if (!spritesheet->loadFromFile("res/sprites/tabbyCat.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        if (!spritesheet3->loadFromFile("res/sprites/deadTabbyCat.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }

        //Create cat's sprite
        sp2 = cat2->addComponent<SpriteComponent>();
        sp2->setTexture(spritesheet);
        sp2->getSprite().setOrigin(50.f, 50.f);

        //Add cat AI (pathfinding and state machine)
        a2 = cat2->addComponent<CatAI>(Vector2f(60.f, 40.f), true, Vector2f(200.f, 0.0f), "res/levels/minigame.txt");
        a2->SetDead(false);

    }

    //Create player
    {
        player2 = makeEntity();

        p2 = player2->addComponent<PlayerComponent>("PlayerName", 500.00);
        //Fill inventory
        p2->baseInventory();
    }

    //Create cursor
    {
        std::shared_ptr<Texture> cursor = std::make_shared<sf::Texture>();;
        cursorSprite2 = makeEntity();

        if (!cursor->loadFromFile("res/sprites/laserPointer.png"))
        {
            //Error
        }
        cursorEntity2 = cursorSprite2->addComponent<SpriteComponent>();
        cursorEntity2->setTexture(cursor);
        cursorEntity2->getSprite().setPosition(0, 0);
        cursorSpeed = 0.15f;
    }

    //UI STUFF
    txt = makeEntity();
    txt2 = makeEntity();

    score = txt->addComponent<TextComponent>();
    score->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 5 - 100, 80));
    score->getText().setScale(1, 1);
    score->getText().setColor(sf::Color::Black);

    score->SetText("Score: " + std::to_string(i));

    hiScore = txt2->addComponent<TextComponent>();
    hiScore->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x - 300, 80));
    hiScore->getText().setScale(1, 1);
    hiScore->getText().setColor(sf::Color::Black);

    Level2Scene::LoadGame();

    hiScore->SetText("High Score: " + std::to_string(highScore));

    cout << " Scene 2 Load Done" << endl;
    setLoaded(true);
}

void Level2Scene::UnLoad()
{
    cat2.reset();
    c2.reset();
    a2.reset();
    sp2.reset();
    player2.reset();
    p2.reset();
    sp2.reset();
    cursorEntity2.reset();
    cursorSprite2.reset();

    enemyEnts.clear();
    it2.clear();
    s2.clear();
    enemyLocations.clear();
    enem.reset();

    txt.reset();
    txt2.reset();
    score.reset();
    hiScore.reset();

    cout << "Scene 2 UnLoad" << endl;
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt)
{
    if (!_startTime)
    {
        _start = std::chrono::system_clock::now();
        _startTime = true;
    }

    if (_startTime)
    {
        //Get current system time every frame
        std::chrono::time_point<std::chrono::system_clock> dur = std::chrono::system_clock::now();

        //Find difference between dur and _start
        long long seconds = std::chrono::duration_cast<std::chrono::seconds>(dur - _start).count();

        if (seconds != store)
        {
            i++;

            if (lose)
            {
                i = 0;
            }
            score->SetText("Score: " + std::to_string(i));
            if (!lose)
            {
                cout << i << endl;

                if ((i % 5) == 0)
                {

                    auto enemies = ls::findTiles(ls::ENEMY);

                    numberOfEnemies = rand() % 3 + 4;

                    for (int k = 0; k < numberOfEnemies; k++)
                    {
                        //Randomize enemy spawn location on top row

                        reroll = true;

                        while (reroll)
                        {
                            reroll = false;

                            randomIndex = rand() % enemies.size();

                            enemySpawnLocation = ls::getTilePosition(enemies[randomIndex]) + Vector2f(20.f, 20.f);

                            for (int l = 0; l < enemyLocations.size(); l++)
                            {
                                if (enemySpawnLocation == enemyLocations[l])
                                {
                                    reroll = true;
                                }
                            }
                        }

                        enem = makeEntity();

                        //Choose random spot to place enemy on screen
                        enem->setPosition(enemySpawnLocation);

                        //Create enemy
                        auto j2 = enem->addComponent<Enemy>(Vector2f(40.f, 20.f), Vector2f(0.f, 100.0f));

                        //Create sprite for enemy
                        auto is2 = enem->addComponent<SpriteComponent>();


                        std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();

                        if (!spritesheet->loadFromFile("res/sprites/rock.png"))
                        {
                            cerr << "Failed to load spritesheet!" << std::endl;
                        }

                        is2->setTexture(spritesheet);
                        is2->getSprite().setScale(0.4f, 0.4f);
                        is2->getSprite().setOrigin(50.f, 50.f);

                        //Add item to list of items
                        it2.push_back(j2);
                        s2.push_back(is2);
                        enemyEnts.push_back(enem);
                        enemyLocations.push_back(enemySpawnLocation);
                    }
                }
            }
        }
        enemyLocations.clear();

        store = std::chrono::duration_cast<std::chrono::seconds>(dur - _start).count();

    }

    int rangeX = 60;
    int rangeY = 40;
    //For each enemy..
    if (!enemyEnts.empty())
    {
        for (int k = 0; k < enemyEnts.size(); k++)
        {

            if (!lose && (cat2->getPosition().y > enemyEnts[k]->getPosition().y - rangeY && cat2->getPosition().y < enemyEnts[k]->getPosition().y + rangeY) && (cat2->getPosition().x > enemyEnts[k]->getPosition().x - rangeX && cat2->getPosition().x < enemyEnts[k]->getPosition().x + rangeX))
            {
                cout << "game over" << endl;
                lose = true;
                a2->SetDead(true);

                sp2->setTexture(spritesheet3);
                sp2->getSprite().setOrigin(50.f, 50.f);

                it2[k]->SetDelete(true);
            }
            //Check if enemy should be deleted
            if (it2[k]->GetDelete())
            {

                //Delete enemy
                enemyEnts[k]->setForDelete();
                enemyEnts.erase(enemyEnts.begin() + k);
                it2.erase(it2.begin() + k);
                s2.erase(s2.begin() + k);
            }
        }
    }

    if (lose)
    {
        p2->SetCurrency(p2->getCurrency() + (i * 50));
        if (i > highScore)
        {
            highScore = i;
            hiScore->SetText("High Score: " + std::to_string(highScore));
        }

        i = 0;
    }

    a2->PickTarget("MOUSE", sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()).x - 7.0f, 0));
    MouseUpdate();
    Scene::Update(dt);
}

void Level2Scene::Render()
{
    ls::render(Engine::GetWindow());
    Scene::Render();
}

void Level2Scene::SaveGame() {

    {
        //Save Player Data
        std::ofstream saveFile("playerFile.txt", std::ofstream::out);

        saveFile << p2->getName() << " ";
        saveFile << p2->getCurrency() << " ";

        saveFile.close();
    }

    {
        //Save high score
        std::ofstream saveFile("highScoreFile.txt", std::ofstream::out);

        saveFile << highScore;

        saveFile.close();
    }

}

void Level2Scene::LoadGame()
{

    {
        //Load Cat Data
        ifstream myfile("catFile.txt");
        if (!myfile)
        {
            std::cout << "Failed to find file" << std::endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            myfile.seekg(0, ios::end);
            size_t size = myfile.tellg();
            if (size == 0)
            {
                std::cout << "File is empty\n";
            }
            else
            {
                myfile.seekg(0, ios::beg);
                float hp = 0, mood = 0, hng = 0, cln = 0, agl = 0, pwr = 0, stm = 0, bond = 0;
                string typ = "", name = "", sex = "", faveFood = "";
                int age = 0;
                if (myfile >> hp >> mood >> hng >> cln >> agl >> pwr >> stm >> bond >> typ >> name >> sex >> faveFood >> age)
                {
                    c2->SetHealth(hp);
                    c2->SetMood(mood);
                    c2->SetHunger(hng);
                    c2->SetCleanliness(cln);
                    c2->SetAgility(agl);
                    c2->SetPower(pwr);
                    c2->SetStamina(stm);
                    c2->SetBond(bond);
                    c2->SetType(typ);
                    c2->SetName(name);
                    c2->SetSex(sex);
                    c2->SetFaveFood(faveFood);
                    c2->SetAge(age);
                }
                else
                {
                    std::cout << "Failed to load from file" << std::endl;
                }
            }
        }
    }

    {
        //Load Player Data
        ifstream myfile("playerFile.txt");
        if (!myfile)
        {
            std::cout << "Failed to find file" << std::endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            myfile.seekg(0, ios::end);
            size_t size = myfile.tellg();
            if (size == 0)
            {
                std::cout << "File is empty\n";
            }
            else
            {
                myfile.seekg(0, ios::beg);
                string name = "";
                double dbl = 0.0;

                int age = 0;
                if (myfile >> name >> dbl)
                {
                    p2->SetName(name);
                    p2->SetCurrency(dbl);
                }
                else
                {
                    std::cout << "Failed to load from file" << std::endl;
                }
            }
        }
    }

    {
        //Load Player Data
        ifstream myfile("highScoreFile.txt");
        if (!myfile)
        {
            std::cout << "Failed to find file" << std::endl;
        }
        else
        {
            //CHECK FILE NOT EMPTY
            myfile.seekg(0, ios::end);
            size_t size = myfile.tellg();
            if (size == 0)
            {
                std::cout << "File is empty\n";
            }
            else
            {
                myfile.seekg(0, ios::beg);

                int high = 0;
                if (myfile >> high)
                {
                    highScore = high;
                }
                else
                {
                    std::cout << "Failed to load from file" << std::endl;
                }
            }
        }
    }
}

void Level2Scene::MouseUpdate()
{
    cursorSprite2->setPosition(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()).x - 7.0f, sf::Mouse::getPosition(Engine::GetWindow()).y));
}

void Level2Scene::Retry()
{
    Level2Scene::SaveGame();
    Level2Scene::UnLoad();
    Level2Scene::Load();

}