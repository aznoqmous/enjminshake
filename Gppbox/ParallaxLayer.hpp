#pragma once
#include <SFML/Graphics.hpp>
#include "Lib.hpp"
using namespace sf;

class Game;
class ParallaxLayer {
public:
	Sprite sprite;
	Texture texture;
	float speedRatio;
	Vector2f cameraPos;

	ParallaxLayer(std::string texturePath, float speedRatio);
	void update(double dt, Game& game);
	void draw(RenderWindow& win);
};