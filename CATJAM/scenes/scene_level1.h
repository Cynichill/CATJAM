#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void SaveGame();

  void LoadGame();

  void Highlight();

  void Controls();

  void MenuDrop();

  void MouseUpdate();

  void KeyboardUpdate();

  void LoadMenu();

  void UnLoadMenu();

private:
	bool hideMouse;
	bool keyPressed;
	bool menuDropped;
	bool shopOpen;
	bool selected;
	float cursorSpeed;
	bool controller;

};
