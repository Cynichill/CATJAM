#include "scene_level2.h"
#include "../components/cmp_physics.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;


void Level2Scene::Load() 
{
	
	cout << " Scene 2 Load Done" << endl;
	setLoaded(true);
}

void Level2Scene::UnLoad() 
{

  cout << "Scene 2 UnLoad" << endl;
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) 
{
	
	Scene::Update(dt);
}

void Level2Scene::Render() 
{
	Scene::Render();
}

