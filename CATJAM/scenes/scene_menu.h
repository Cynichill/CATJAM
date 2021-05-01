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

  void Render() override;

};
