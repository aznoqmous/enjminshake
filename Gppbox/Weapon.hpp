#pragma once
#include <iostream>
#include "Entity.hpp"

using namespace std;

class Game;

enum WeaponType {
	WeaponBullet,
	WeaponLaser
};

class Weapon {
public:
	WeaponType type;
	string weaponName = "Gun";

	int weaponIndex = 0;
	Texture texture;
	Sprite sprite;
	int spriteWidth = 32;
	int spriteHeight = 16;
	Vector2f origin { 3, 8 };
	Vector2f offset;

	// laser weapons
	Vector2f firePosition;
	Vector2f fireTarget;
	sf::VertexArray laserVertices;

	// firee
	float fireRate = 0.1f;
	float fireCooldown = 0.f;
	int bulletCount = 1;
	float bulletAngle = 10.f;
	float recoil = 0.5f;
	float spray = 0.05f;
	Texture muzzleFireTexture;
	Sprite muzzleFireSprite;
	Vector2f muzzleFireOrigin { -6, 9 };
	bool isDrawMuzzleFire = false;
	bool isAutomatic = false;

	Weapon(WeaponType type, int weaponIndex, string weaponName) {
		this->type = type;
		this->weaponName = weaponName;
		sprite = Lib::loadSprite(texture, "res/weapons.png");
		muzzleFireSprite = Lib::loadSprite(muzzleFireTexture, "res/muzzle-fire.png");
		setWeaponIndex(weaponIndex);
		laserVertices = VertexArray(LinesStrip, 10);
	}

	bool canFire();
	void fire(Entity& entity, Game& game);
	void update(Entity& entity, double dt, Game& game);
	void draw(Entity& entity, RenderWindow& win);

	void setWeaponIndex(int index);

};

