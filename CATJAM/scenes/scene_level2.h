#pragma once

#include "engine.h"

class Level2Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void Highlight();

  void Controls();

  void MenuDrop();

  void MouseUpdate();

  void KeyboardUpdate();

private:
	bool hideMouse;
	bool keyPressed;
	bool menuDropped;
	bool shopOpen;
	bool selected;
	float cursorSpeed;
	bool controller;
};