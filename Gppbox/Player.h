#pragma once
#include "Entity.h"
#include "C.hpp"
#include <iostream>

class Player : public Entity {
public:
	int animationLength[4] = { 4, 6, 2, 3};

	Texture weaponTexture;
	Sprite weaponSprite;
	int weaponSpriteWidth = 32;
	int weaponSpriteHeight = 16;
	int weaponOriginX = 4;
	int weaponOriginY = -10;

	Player() : Entity("res/peer.png") {
		if (!weaponTexture.loadFromFile("res/weapons.png")) {
			std::cerr << "Fail to load weaponTexture !" << std::endl;
		}
		else {
			std::cout << "weaponTexture loaded !" << std::endl;
			weaponSprite.setTexture(weaponTexture);
			setWeaponIndex(1);
		}
	
	}
	Player(int cx, int cy) : Entity("res/peer.png", cx, cy) {}

	void draw(RenderWindow& win) override;
	void setWeaponIndex(int index);
};