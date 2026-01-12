#include "Particle.hpp"
#include "Game.hpp"
#include "Interp.hpp"

void Particle::s_nope(Particle* lthis, float dt) {
	// no-op behavior
}

Particle::Particle(std::string texturePath, float x, float y) {
	texture = std::make_shared<sf::Texture>();
	if (!texture->loadFromFile(texturePath)) {
		std::cerr << "Failed to load particle texture: " << texturePath << std::endl;
	}
	this->x = x;
	this->y = y;
	sprite.setPosition(x, y);
	sprite.setTexture(*texture);
	sprite.setOrigin(4, 4);
	life = Dice::randF() * 6.0f + 2.0f * 6.0f;
	bhv = s_nope;
}

void Particle::update(float dt, Game& game) {
	if (collideWithWalls) {
		int cx = floor(x / (float) C::GRID_SIZE);
		int cy = floor(y / (float) C::GRID_SIZE);
		float rx = x / C::GRID_SIZE - cx;
		float ry = y / C::GRID_SIZE - cy;
		if (game.isWall(cx + 1, cy) && rx >= 0.9) {
			rx = 0.9;
			//x = cx + rx;
			dx = 0;
		}
		if (game.isWall(cx - 1, cy) && rx <= 0.1) {
			rx = 0.1;
			//x = cx + rx;
			dx = 0;
		}
		if (game.isWall(cx, cy + 1) && ry >= 0.9) {
			dx = Interp::lerp(dx, 0.f, dt * 50.f); // apply frixxxion
			angularSpeed = 0.f;
			ry = 0.9;
			//y = cy + ry;
			dy = 0;
		}
		if (game.isWall(cx, cy - 1) && ry <= 0.1) {
			ry = 0.1;
			//y = cy + ry;
			dy = 0;
		}
	}
	
	x += dx * dt;
	y += dy * dt;

	sprite.setPosition(x, y);
	sprite.setScale(scaleX * C::PIXEL_SIZE, scaleY * C::PIXEL_SIZE);
	sprite.rotate(dt * angularSpeed);

	life -= dt;
	if (life <= 0.f) destroyed = true;
	bhv(this, dt);
}

