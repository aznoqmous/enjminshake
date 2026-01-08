#pragma once
#include "Entity.hpp"
class Game;
class Drone : public Entity {
public:
	sf::Vector2f targetPosition = Vector2f(128, -64);
	float fireRate = 1.f;
	float fireCooldown = 1.f;

	Drone() : Entity("res/drone.png") {
		isFlying = true;
	}
	Drone(float cx, float cy) : Entity("res/drone.png", cx, cy) {
		isFlying = true;
	}
	void draw(RenderWindow& win) override;
	void update(double dt, Game& game) override;
	void im() override;
};