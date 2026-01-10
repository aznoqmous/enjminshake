#pragma once
#include "Entity.hpp"
#include "C.hpp"
#include <iostream>
#include "Weapon.hpp"

class Player : public Entity {
public:
	int animationLength[4] = { 4, 6, 2, 3};
	float lastDamageTaken = 0.f;
	float invulnerabilityTime = 0.5f;

	RectangleShape debugRectangle = RectangleShape(Vector2f(C::GRID_SIZE, C::GRID_SIZE));

	Weapon* activeWeapon = new Weapon();

	Player() : Entity("res/peer.png") {
		debugRectangle.setFillColor(sf::Color::Blue);
	}
	Player(float cx, float cy) : Entity("res/peer.png", cx, cy) {}
	~Player() { delete activeWeapon; }

	void draw(RenderWindow& win) override;
	void update(double dt, Game& game) override;
	void im() override;

	void setWeaponIndex(int index);
	void fire(Game& game);
};