#include "Missile.hpp"
#include "Game.hpp"
#include "Interp.hpp"
#include "SFML/Graphics.hpp"


void Missile::update(float dt, Game& game) {
	Bullet::update(dt, game);
	float nearest = 100000.0f;
	float minDistance = 200.f;
	Foe* nearestFoe = nullptr;
	for (Foe* f : game.foes) {
		float distance = Lib::getMagnitude(position - f->position);
		if (distance < nearest) {
			nearest = distance;
			nearestFoe = f;
		}
	}

	speed += dt * 100.f;
	if (nearest < minDistance && nearestFoe) {
		float targetAngle = Lib::angle(nearestFoe->position - position);
		float vangle = Lib::angle(velocity);
		float angle = Interp::lerpAngle(vangle, targetAngle, turnSpeed * dt);
		Lib::rotate(velocity, angle - vangle);
	}
	
	smokeTime -= dt;
	if (smokeTime <= 0.f) {
		smokeTime = smokeInterval;
		Particle p = Particle("res/smoke.png", position.x, position.y);
		p.dx = Dice::randF() * Dice::randSign() * 50.f;
		p.dy = Dice::randF() * Dice::randSign() * 50.f;
		p.life = 1.0f;
		p.sprite.setOrigin(25.f / 2.f, 25.f / 2.f);
		p.sprite.rotate(Dice::randF() * 360.f);
		p.bhv = [](Particle* p, float dt) {
			p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 1.0f) * 4) * 25, 0, 25, 25));
			};
		game.beforeParts.add(p);
	}

	if(Lib::getMagnitude(position - game.player.position) > 2000.f) isLive = false;
}

void Missile::draw(RenderWindow& win) {
	sprite.setPosition(position);
	sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
	sprite.setRotation(atan2(velocity.y, velocity.x) * Lib::rad2deg());
	sprite.setOrigin({16, 16});
	win.draw(sprite);
}


void Missile::handleEntityCollision(Foe& foe, Game& game) {
	isLive = false;
	game.timeSpeed = 0.f;
	Vector2f shake(1.f, 0.0f);
	Lib::rotate(shake, Dice::randF());
	game.screenShake(shake);

	for (Foe* f : game.foes) {
		float distance = Lib::getMagnitude(position - f->position);
		if (distance < 100.f) {
			f->takeDamage(1.f);
			f->dx = copysignf(1.f, f->position.x - position.x);
			f->dy = -1.f;
		}
	}

	Particle p = Particle("res/explosion.png", position.x, position.y);
	p.life = 0.2f;
	p.sprite.setOrigin(32.f / 2.f, 32.f / 2.f);
	p.bhv = [](Particle* p, float dt) {
		p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.2f) * 3) * 32, 0, 32, 32));
		};
	game.afterParts.add(p);
}

void Missile::handleWallCollision(Vector2i& wall, Game& game) {
	isLive = false;
	game.timeSpeed = 0.f;
	Vector2f shake(1.f, 0.0f);
	Lib::rotate(shake, Dice::randF());
	game.screenShake(shake);

	for (Foe* f : game.foes) {
		float distance = Lib::getMagnitude(position - f->position);
		if (distance < 100.f) {
			f->takeDamage(1.f);
			f->dx = copysignf(1.f, f->position.x - position.x);
			f->dy = -1.f;
		}
	}

	Particle p = Particle("res/explosion.png", position.x, position.y);
	p.life = 0.2f;
	p.sprite.setOrigin(32.f / 2.f, 32.f / 2.f);
	p.bhv = [](Particle* p, float dt) {
		p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.2f) * 3) * 32, 0, 32, 32));
		};
	game.afterParts.add(p);
}