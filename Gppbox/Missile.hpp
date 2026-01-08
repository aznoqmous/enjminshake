#pragma once
#include "Bullet.hpp"

class Game;
class Missile : public Bullet {
public:
	float smokeInterval = 0.5f;
	float smokeTime = 0.f;
	float turnSpeed = 1.f;
	void update(float dt, Game& game) override;
	void draw(RenderWindow& win) override;
	void handleEntityCollision(Foe& foe, Game& game) override;
	void handleWallCollision(Vector2i& wall, Game& game) override;
	
	Missile() : Bullet() {
		sprite = Lib::loadSprite(texture, "res/missile.png");
		speed = 10.f;
		turnSpeed = 1.f;
		smokeInterval = 0.1f;
	}
};