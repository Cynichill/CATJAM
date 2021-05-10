#pragma once

#include "engine.h"

class Level2Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void SaveGame();

  void LoadGame();

  void MouseUpdate();

  void Retry();

  void SpawnBox();

  void KeyboardUpdate();

  void Highlight();

  void Controls();

private:
	float cursorSpeed;
	bool keyPressed;
	bool controller;
	bool change;
	bool hideMouse;
};