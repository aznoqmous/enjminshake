#include "ParallaxLayer.hpp"
#include "Game.hpp"
#include "C.hpp"
#include <imgui.h>

ParallaxLayer::ParallaxLayer(std::string texturePath, float speedRatio) {
	this->speedRatio = speedRatio;
	sprite = Lib::loadSprite(texture, texturePath);
	sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
}

void ParallaxLayer::update(double dt, Game& game) {
	cameraPos = game.cameraPosition;
}

void ParallaxLayer::draw(RenderWindow& win){
	Vector2f screenSize = win.getView().getSize();
	Vector2f maxv = cameraPos + screenSize / 2.f;
	Vector2f minv = cameraPos - screenSize / 2.f;
	Vector2f spriteSize = ((Vector2f)texture.getSize()) * (float) C::PIXEL_SIZE;
	float neededFrames = ceil(screenSize.x / (spriteSize.x)) + 1;

	for (int frame = 0; frame < neededFrames; frame++) {
		float x = cameraPos.x * speedRatio + frame * spriteSize.x;
		while (x < minv.x) x += neededFrames * spriteSize.x;
		while (x > maxv.x) x -= neededFrames * spriteSize.x;

		float y = cameraPos.y - screenSize.y / 2.f + cameraPos.y * speedRatio / 10.f
			- spriteSize.y / 2.f + screenSize.y / 2.f;
		
		sprite.setPosition(
			x,
			y
		);
		win.draw(sprite);
	}
}

