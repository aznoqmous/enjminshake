#include "Particle.hpp"

void Particle::s_nope(Particle* lthis, float dt) {
	// no-op behavior
}

Particle::Particle(std::string texturePath) {
	// allocate a shared texture so copies share ownership safely
	texture = std::make_shared<sf::Texture>();
	if (!texture->loadFromFile(texturePath)) {
		std::cerr << "Failed to load particle texture: " << texturePath << std::endl;
	}
	sprite.setTexture(*texture);
	sprite.setOrigin(4, 4);
	life = Dice::randF() * 6.0f + 2.0f * 6.0f;
	bhv = s_nope;
}

