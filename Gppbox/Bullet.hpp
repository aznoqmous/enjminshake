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
	float size = 32.f;

	bool isLive = true;

	Bullet();
	virtual ~Bullet() = default;

	void virtual update(float dt, Game& game);
	void virtual draw(RenderWindow& win);

	void virtual handleEntityCollision(Foe& foe, Game& game);
	void virtual handleWallCollision(Vector2i& wall, Game& game);
};