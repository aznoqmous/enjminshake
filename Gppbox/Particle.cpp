#include "Particle.hpp"

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

void Particle::update(float dt) {
	x += dx * dt;
	y += dy * dt;

	sprite.setPosition(x, y);
	sprite.setScale(scaleX * C::PIXEL_SIZE, scaleY * C::PIXEL_SIZE);
	sprite.rotate(dt * 3.0f);

	life -= dt;
	if (life <= 0.f) destroyed = true;
	bhv(this, dt);
}

