#include "Foe.hpp"
#include "Game.hpp"
#include <cmath>

void Foe::update(double dt, Game& game) {
	speed = 1.f;

	Entity::update(dt, game);

	if (isAlive()) {
		if (flipSprite) {
			if (!collideLeft) moveLeft(dt);
			else moveRight(dt);
		}
		else {
			if (!collideRight) moveRight(dt);
			else moveLeft(dt);
		}
	}

}