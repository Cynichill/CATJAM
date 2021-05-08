#pragma once

#include "engine.h"
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_OF_ITEMS 4

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void MouseCheck();
  void Highlight();
  void MenuChange();
  void MenuControls();
  void MoveUp();
  void MoveDown();
  void MenuSelect();
  void setSize();
  void DeleteSaveData();
  bool controllerType();


private:
	int selected;
	int changeMenu;
	int maxDraw;
	bool keyPressed;
	float widthStore;
	float heightStore;
	bool fullscreen;
	bool controller;
};
