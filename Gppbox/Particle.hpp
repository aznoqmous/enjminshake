#pragma once

#include "SFML/Graphics.hpp"
#include "Lib.hpp"
#include "Dice.hpp"
#include <functional>
#include "C.hpp"
#include <memory>

using namespace sf;
class Particle {
public:

	std::shared_ptr<sf::Texture> texture;
	Sprite	sprite;
	float			x = 0.0f;
	float			y = 0.0f;

	float			dx = 0.0f;
	float			dy = 0.0f;

	float			life = 1.0f;

	float			scaleX = 1.0f;
	float			scaleY = 1.0f;

	bool			destroyed = false;

	std::function<void(Particle * lthis, float dt)> bhv;
	static void s_nope(Particle* lthis, float dt);

	Particle(std::string texturePath);

	void update(float dt) {
		x += dx * dt;
		y += dy * dt;

		sprite.setPosition(x, y);
		sprite.setScale(scaleX * C::PIXEL_SIZE, scaleY * C::PIXEL_SIZE);
		sprite.rotate(dt * 3.0f);

		life -= dt;
		if (life <= 0.f) destroyed = true;
		bhv(this,dt);
	}
};