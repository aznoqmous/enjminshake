#include "Bullet.hpp"
#include "C.hpp"
#include "Game.hpp"

Bullet::Bullet() {
	sprite = Lib::loadSprite(texture, "res/bullet-fire.png");
}

void Bullet::update(float dt, Game& game){
	position += dt * velocity * speed * float(C::PIXEL_SIZE);
	for (Entity* e : game.entities) {
		if (Lib::getMagnitude(e->position - position) < size) {
			handleEntityCollision(*e);
			// mark bullet as not live; actual deletion handled in Game::update
			break;
		}
	}
}

void Bullet::draw(RenderWindow& win) {
	sprite.setPosition(position);
	sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
	sprite.setRotation(atan2(velocity.y, velocity.x) * Lib::rad2deg());
	sprite.setOrigin({8, 8});
	win.draw(sprite);
}

void Bullet::handleEntityCollision(Entity& entity) {
	isLive = false;
}
void Bullet::handleWallCollision() {
	isLive = false;
}