//player.h
#pragma once
#include "ecm.h"

class Player : public Entity {
private:
	float _speed;

public:
	void Update(double dt) override;
	Player();
	void Render() const override;
};