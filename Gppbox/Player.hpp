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

		Weapon* gun = new Weapon(WeaponBullet, 0, "Gun");
		gun->fireRate = 0.f;
		gun->spray = 0.02f;
		gun->muzzleFireOrigin = { -6, 9 };
		weapons.push_back(gun);

		Weapon* machineGun = new Weapon(WeaponBullet, 1, "Rifle");
		machineGun->fireRate = 0.1f;
		machineGun->spray = 0.05f;
		machineGun->muzzleFireOrigin = { -14, 10 };
		machineGun->isAutomatic = true;
		weapons.push_back(machineGun);

		Weapon* tripleMachineGun = new Weapon(WeaponBullet, 2, "Triple Machinegun");
		tripleMachineGun->fireRate = 0.1f;
		tripleMachineGun->bulletCount = 3;
		tripleMachineGun->bulletAngle = 0.1f;
		tripleMachineGun->spray = 0.01f;
		tripleMachineGun->muzzleFireOrigin = { -16, 10 };
		tripleMachineGun->isAutomatic = true;
		weapons.push_back(tripleMachineGun);

		Weapon* laserRifle = new Weapon(WeaponLaser, 3, "Laser Rifle");
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