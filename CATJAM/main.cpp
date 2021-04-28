#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

int width = 800;
int height = 600;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
  Engine::Start(width, height, "Platformer",&menu);
}