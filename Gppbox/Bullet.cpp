#include "Bullet.hpp"
#include "C.hpp"
#include "Game.hpp"

Bullet::Bullet() {
	sprite = Lib::loadSprite(texture, "res/bullet-fire.png");
}

void Bullet::update(float dt, Game& game){
	position += dt * velocity * speed * float(C::PIXEL_SIZE);
	for (Foe* e : game.foes) {
		if (Lib::getMagnitude(e->position - position) < size) {
			handleEntityCollision(*e);
			game.timeSpeed = 0.f;
		}
	}
	for (Vector2i& w : game.walls) {
		if (Lib::getMagnitude(Vector2f(w.x, w.y) - position) < size) {
			handleWallCollision(w);			
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

void Bullet::handleEntityCollision(Foe& foe) {
	isLive = false;
	foe.takeDamage(1.f);
	foe.dx = copysignf(1.f, foe.position.x - position.x);
	foe.dy = -1.f;
}
void Bullet::handleWallCollision(Vector2i& wall) {
	isLive = false;
}