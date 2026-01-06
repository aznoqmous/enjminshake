#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Entity {
public:
	Texture texture;
	Sprite sprite;

	// Base coordinates
	int cx;
	int cy;
	float xr;
	float yr;

	// Resulting coordinates
	float xx;
	float yy;

	// Movements
	float dx;
	float dy;



	Entity();

	void update(double dt, Game& game);
	void setPositionPixel(float px, float py);
	void setPositionCell(float cx, float cy);
	void syncPixel();

	void move(float x, float y);

	void moveLeft(double dt);
	void moveRight(double dt);

	void im();
};