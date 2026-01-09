#include "Foe.hpp"
#include "Game.hpp"
#include <cmath>

void Foe::update(double dt, Game& game) {

	speed = 1.f;

	Entity::update(dt, game);
	if (!isActive) {
		if (Lib::getMagnitude(game.player.position - position) < 500.f) {
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