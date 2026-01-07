#pragma once
#include <iostream>
#include "Entity.hpp"
class Game;

class Weapon {
public:
	int weaponIndex = 0;

	Texture texture;
	Sprite sprite;
	int spriteWidth = 32;
	int spriteHeight = 16;
	Vector2f origin { 3, 3 };
	Vector2f offset;

	// firee
	float fireRate = 0.1f;
	float fireCooldown = 0.f;
	float recoil = 3.f;
	float spray = 0.05f;
	Texture muzzleFireTexture;
	Sprite muzzleFireSprite;
	Vector2f muzzleFireOrigin { -3, 4 };

	Weapon() {
		sprite = Lib::loadSprite(texture, "res/weapons.png");
		muzzleFireSprite = Lib::loadSprite(muzzleFireTexture, "res/muzzle-fire.png");
		setWeaponIndex(weaponIndex);
	}

	bool canFire();
	void fire(Entity& entity, Game& game);
	void update(Entity& entity, double dt, Game& game);
	void draw(Entity& entity, RenderWindow& win);

	void setWeaponIndex(int index);

};