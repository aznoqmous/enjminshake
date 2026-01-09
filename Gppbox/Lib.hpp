#pragma once

#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace Lib {
	double getTimeStamp();
	constexpr double pi() { return 3.141592653589793238462643383279502884; }
	constexpr double rad2deg() { return 180 / pi(); }
	constexpr double deg2rad() { return pi() / 180.0; }

	sf::Color makeFromHSV(float _h, float _s, float _v);
	sf::Sprite loadSprite(sf::Texture& texture, const string& texturePath);

	float getMagnitude(sf::Vector2f v);

	inline void rotate(sf::Vector2f& v, float angle) {
		float x = v.x;
		float y = v.y;
		v.x = x * cos(angle) - y * sin(angle);
		v.y = x * sin(angle) + y * cos(angle);
	}

	inline float angle(const sf::Vector2f& v) {
		return atan2f(v.y, v.x);
	}

	inline void normalize(sf::Vector2f& v) {
		float magnitude = getMagnitude(v);
		v.x /= magnitude;
		v.y /= magnitude;
	}
};