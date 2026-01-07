#pragma once
#include <SFML/Graphics.hpp>
#include "Lib.hpp"
#include "Foe.hpp"
#include "C.hpp"
using namespace sf;

class Game;

class Bullet {
public:
	Texture texture;
	Sprite sprite;
	
	Vector2f position;
	Vector2f velocity;
	float speed = 1000.f;
	float size = C::PIXEL_SIZE * 16.f;

	bool isLive = true;

	Bullet();

	void update(float dt, Game& game);
	void draw(RenderWindow& win);

	void handleEntityCollision(Foe& foe);
	void handleWallCollision(Vector2i& wall);
};