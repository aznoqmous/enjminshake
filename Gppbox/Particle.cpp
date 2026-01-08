#include "Particle.hpp"

void Particle::s_nope(Particle* lthis, float dt) {
	// no-op behavior
}

Particle::Particle(std::string texturePath) {
	//sprite = RectangleShape(Vector2f(8, 8));
	sprite = Lib::loadSprite(texture, texturePath);
	sprite.setOrigin(4, 4);
	//sprite.setFillColor(sf::Color(0xff0000ff));
	life = Dice::randF() * 6.0f + 2.0f * 6.0f;
	bhv = s_nope;
}

Particle::Particle(const Particle& other) {
	texture = other.texture; // copy texture data
	sprite = other.sprite; // copy sprite
	sprite.setTexture(texture); // rebind sprite to our texture

	x = other.x; y = other.y;
	dx = other.dx; dy = other.dy;
	life = other.life;
	scaleX = other.scaleX; scaleY = other.scaleY;
	destroyed = other.destroyed;
	bhv = other.bhv;
}

Particle& Particle::operator=(const Particle& other) {
	if (this != &other) {
		texture = other.texture;
		sprite = other.sprite;
		sprite.setTexture(texture);

		x = other.x; y = other.y;
		dx = other.dx; dy = other.dy;
		life = other.life;
		scaleX = other.scaleX; scaleY = other.scaleY;
		destroyed = other.destroyed;
		bhv = other.bhv;
	}
	return *this;
}
