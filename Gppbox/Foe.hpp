#pragma once
#include "Entity.hpp"

class Foe : public Entity {
public:
	bool isActive = false;
	Foe() : Entity("res/foe.png") {
		speed = 1.f;
	}
	Foe(int cx, int cy) : Entity("res/foe.png", cx, cy) {
		speed = 1.f;
	}
	void update(double dt, Game& game) override;
};