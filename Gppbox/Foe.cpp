#include "Foe.hpp"
#include "Game.hpp"
#include <cmath>

void Foe::update(double dt, Game& game) {
	Entity::update(dt, game);

	// activate enemies based on distance to player
	if (!isActive) {
		if (Lib::getMagnitude(game.player.position - position) < 800.f) {
			isActive = true;
		}
		else return;
	}

	if (isAlive() && isFloored) {
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