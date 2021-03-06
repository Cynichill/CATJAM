#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include <fstream>

using namespace std;

int width;
int height;


MenuScene menu;
Level1Scene level1;
Level2Scene level2;

int main() {

	//Load resolution options from file
	ifstream file("resolutionFile.txt");
	if (!file)
	{
		width = 800;
		height = 600;

		std::ofstream saveFile("resolutionFile.txt", std::ofstream::out);
		saveFile << width << " ";
		saveFile << height << " ";
		saveFile.close();
	}
	else
	{
		//CHECK FILE NOT EMPTY
		file.seekg(0, ios::end);
		size_t size = file.tellg();
		if (size == 0)
		{
			//Error
		}
		else
		{
			file.seekg(0, ios::beg);
			float w = 0, h = 0;
			if (file >> w >> h)
			{
				width = w;
				height = h;
			}
			else
			{
				//Error
			}
		}
	}
  Engine::Start(width, height, "Cat-Atonic",&menu);
}