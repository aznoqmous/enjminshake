#pragma once
#include "Entity.hpp"
#include "C.hpp"
#include <iostream>
#include <array>
#include "Weapon.hpp"

class Player : public Entity {
public:
	float lastDamageTaken = 0.f;
	float invulnerabilityTime = 0.5f;

	RectangleShape debugRectangle = RectangleShape(Vector2f(C::GRID_SIZE, C::GRID_SIZE));

	std::vector<Weapon*> weapons;
	int activeWeaponIndex = 0;
	Weapon* activeWeapon;

	Player() : Entity("res/peer.png") {
		animationLength = { 4, 6, 2, 3 };
		debugRectangle.setFillColor(sf::Color::Blue);

		Weapon* gun = new Weapon(WeaponBullet, 0);
		weapons.push_back(gun);

		Weapon* laserRifle = new Weapon(WeaponLaser, 3);
		weapons.push_back(laserRifle);

		activeWeapon = gun;
	}

	Player(float cx, float cy) : Entity("res/peer.png", cx, cy) {
		animationLength = { 4, 6, 2, 3 };

		Weapon* gun = new Weapon(WeaponBullet, 0);
		weapons.push_back(gun);

		Weapon* laserRifle = new Weapon(WeaponLaser, 3);
		weapons.push_back(laserRifle);

		activeWeapon = gun;
	}

	~Player() {
		for (auto w : weapons) delete w;
	}

	void draw(RenderWindow& win) override;
	void update(double dt, Game& game) override;
	void im() override;

	void setWeaponIndex(int index);
	void fire(Game& game);
};