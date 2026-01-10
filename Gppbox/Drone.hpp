#pragma once
#include "Entity.hpp"
class Game;
class Drone : public Entity {
public:
	sf::Vector2f targetPosition = Vector2f(32, -64);
	float fireRate = 0.5f;
	float fireCooldown = 0.5f;

	Drone() : Entity("res/drone.png") {
		isFlying = true;
		speed = 2.f;
		scale = 0.f;
	}
	Drone(float cx, float cy) : Entity("res/drone.png", cx, cy) {
		isFlying = true;
		speed = 2.f;
		scale = 0.f;
	}
	void draw(RenderWindow& win) override;
	void update(double dt, Game& game) override;
	void fire(Game& game);
	void im() override;
};