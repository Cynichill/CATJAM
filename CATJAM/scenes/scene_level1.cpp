#include "scene_level1.h"
#include "../components/cmp_cat.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> cat;
std::shared_ptr<CatComponent> c;

void Level1Scene::Load() {

  // Create Cat
  {
    cat = makeEntity();

    c = cat->addComponent<CatComponent>("Tabby", "Fluffy", "Female", "CannedCatFood", 4);
    
    cat->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));

   // std::cout << d << std::endl;

    /*
    auto sh = cat->addComponent<ShapeComponent>();
    sh->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    sh->getShape().setFillColor(Color::Blue);
    sh->getShape().setOrigin(10.f, 15.f);
    */

    std::shared_ptr<sf::Texture> spritesheet = std::make_shared<sf::Texture>();;
    

    if (!spritesheet->loadFromFile("res/sprites/tabbyCat.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    auto sp = cat->addComponent<SpriteComponent>();

    sp->setTexture(spritesheet);

    Level1Scene::LoadGame();
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  Level1Scene::SaveGame();
  cat.reset();
  c.reset();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    
  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&menu);
  }

  Scene::Update(dt);
}

void Level1Scene::Render() {

  Scene::Render();
}

void Level1Scene::SaveGame() {

    std::ofstream saveFile("saveFile.txt", std::ofstream::out);

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

void Level1Scene::LoadGame() {

    ifstream myfile("saveFile.txt");
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
