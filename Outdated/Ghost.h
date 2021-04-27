//Ghost.h
#pragma once
#include "ecm.h"

class Ghost : public Entity {
private:
	float _speed;

public:
	void Update(double dt) override;
	Ghost();
	void Render() const override;
};