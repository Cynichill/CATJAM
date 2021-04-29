#include "scene_level1.h"
#include "../components/cmp_cat.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> cat;

void Level1Scene::Load() {

    /*
  // Create Cat
  {
    cat = makeEntity();
    
    auto c = cat->addComponent<CatComponent>("Tabby", "Fluffy", "Female", "Canned Cat Food", 4);

    auto d = c->getAge();

    std::cout << d << std::endl;
  }
  */


  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
 //cat.reset();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {


  Scene::Update(dt);
}

void Level1Scene::Render() {

  Scene::Render();
}
