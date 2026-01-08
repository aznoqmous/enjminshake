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

			Particle p = Particle("res/bullet-hit.png");
			p.x = position.x;
			p.y = position.y;
			p.life = 0.3f;
			p.bhv = [](Particle* p, float dt) {
				p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.3f) * 4) * 16, 0, 16, 16));
				};
			game.afterParts.add(p);
		}
	}
	if (game.isWall(position.x / C::GRID_SIZE, position.y / C::GRID_SIZE)) {
		Vector2i wpos = Vector2i(position.x, position.y);
		handleWallCollision(wpos);

		Particle p = Particle("res/bullet-hit.png");

		p.x = position.x;
		p.y = position.y;
		p.life = 0.3f;
		p.bhv = [](Particle* p, float dt) {
			p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.3f) * 4) * 16, 0, 16, 16));
		};
		game.afterParts.add(p);
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