#pragma once
#include "Entity.hpp"

class Foe : public Entity {
public:
	Foe() : Entity("res/foe.png") {}
	Foe(int cx, int cy) : Entity("res/foe.png", cx, cy) {}
	void update(double dt, Game& game) override;
};