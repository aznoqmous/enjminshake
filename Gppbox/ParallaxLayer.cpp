#include "ParallaxLayer.hpp"
#include "Game.hpp"
#include "C.hpp"
#include <imgui.h>

ParallaxLayer::ParallaxLayer(std::string texturePath, float speedRatio) {
	this->speedRatio = speedRatio;
	sprite = Lib::loadSprite(texture, texturePath);
}

void ParallaxLayer::update(double dt, Game& game) {
	cameraPos = game.cameraPosition;
}

void ParallaxLayer::draw(RenderWindow& win){
	sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
	
	float texWidth = texture.getSize().x * C::PIXEL_SIZE;
	float texHeight = texture.getSize().y * C::PIXEL_SIZE;
	
	float parallaxX = cameraPos.x * speedRatio;
	float parallaxY = cameraPos.y - win.getSize().y / 2.f;
	
	float posX = parallaxX - fmod(parallaxX, texWidth);
	for (float x = posX - texWidth; x < posX + win.getSize().x + texWidth; x += texWidth) {
		sprite.setPosition(x, parallaxY);
		win.draw(sprite);
	}
}

