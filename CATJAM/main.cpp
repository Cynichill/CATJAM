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

	ifstream file("resolutionFile.txt");
	if (!file)
	{
		cout << "Failed to find file" << endl;
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
			cout << "File is empty\n";
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
				cout << "Failed to load file" << endl;
			}
		}
	}
  Engine::Start(width, height, "Platformer",&menu);
}