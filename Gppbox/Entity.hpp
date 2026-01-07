#pragma once

#include <SFML/Graphics.hpp>
#include "Lib.hpp"

using namespace sf;

class Game;

class Entity {
public:
	Texture texture;
	Sprite sprite;

	// Base coordinates
	int cx;
	int cy;
	float rx;
	float ry;

	// Resulting coordinates
	Vector2f position;

	// Movements
	float dx;
	float dy;

	bool isFloored = false;

	float speed = 3.0f;
	float jumpPower = 2.0f;

	bool collideLeft = false;
	bool collideRight = false;

	// Sprite Sheets
	bool flipSprite = false;

	int animationIndex = 0;
	int animationFrame = 0;
	float spriteWidth = 32;
	float spriteHeight = 32;
	double animationStartTime = Lib::getTimeStamp();

	// idle, walking, damage, dead
	float animationSpeed[4] = { .2f, .1f, .1f, .1f };
	bool animationLoop[4] = {true, true, true, false};
	int animationLength[4] = { 4, 6, 3, 6};
	bool isAnimationEnded() {
		return (int((Lib::getTimeStamp() - animationStartTime) / animationSpeed[animationIndex])) >= animationLength[animationIndex];
	}

	float scaleX = 1.f;
	float scaleY = 1.f;

	float maxHp = 3.f;
	float currentHp = 3.f;
	bool isAlive() {
		return currentHp > 0.f;
	}

	Entity(int cx, int cy);
	Entity(const std::string& texturePath);
	Entity(const std::string& texturePath, int cx, int cy);

	virtual void update(double dt, Game& game);
	virtual void draw(RenderWindow& win);
	virtual void im();
	
	void setPositionPixel(float px, float py);
	void setPositionCell(float cx, float cy);
	void syncPixel();

	void move(float x, float y);
	void jump();

	void moveLeft(double dt);
	void moveRight(double dt);
	
	void setAnimation(int index);

	void takeDamage(float value);
	

};

enum Animation {
	Idle, Moving, Hit, Dead
};