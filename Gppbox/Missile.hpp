#pragma once
#include "Bullet.hpp"

class Game;
class Missile : public Bullet {
public:
	float explosionRange = 100.f;
	float smokeInterval = 0.5f;
	float smokeTime = 0.f;
	float turnSpeed = 1.f;
	void update(float dt, Game& game) override;
	void draw(RenderWindow& win) override;
	void handleEntityCollision(Foe& foe, Game& game) override;
	void handleWallCollision(Vector2i& wall, Game& game) override;
	void spawnExplosionParticles(Vector2f position, Game& game);
	Missile() : Bullet() {
		texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile("res/missile.png")) {
			std::cerr << "Fail to load texture res/missile.png" << std::endl;
		}
		else {
			sprite.setTexture(*texture);
		}
		sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
		speed = 50.f;
		turnSpeed = 1.f;
		smokeInterval = 0.1f;
	}
};