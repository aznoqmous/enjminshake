#include "Entity.h"
#include <iostream>
#include <imgui.h>
#include "C.hpp"
#include "Game.hpp"

Entity::Entity(const std::string& texturePath) {
	initializeSprite(texturePath);
}

Entity::Entity(const std::string& texturePath, int cx, int cy) {
	setPositionCell(cx, cy);
	initializeSprite(texturePath);
}

void Entity::initializeSprite(const std::string& texturePath) {
	if (!texture.loadFromFile(texturePath)) {
		std::cerr << "Fail to load texture !" << std::endl;
	}
	else {
		std::cout << "Texture loaded !" << std::endl;
		sprite.setTexture(texture);
	}
}

void Entity::update(double dt, Game& game) {
	std::cout << animationFrame << std::endl;
	while (rx > 1.0f) {
		rx--;
		cx++;
	}
	while (rx < 0.0f) {
		rx++;
		cx--;
	}
	while (ry > 1.0f) {
		ry--;
		cy++;
	}
	while (ry < 0.0f) {
		ry++;
		cy--;
	}


	isFloored = game.isWall(cx, cy + 1);
	if (!isFloored) {
		dy += dt * C::GRAVITY;
	}
	else if (dy >= 0.0f) {
		setPositionCell(cx + rx, cy);
		dy = 0.0f;
	}

	if (!game.isWall(cx + dx * dt, cy)) {
		rx += dx * dt * speed * C::PIXEL_SIZE;
	}
	else {
		dx = 0.0f;
	}

	if (!game.isWall(cx, cy + dy * dt)) {
		ry += dy * dt * speed * C::PIXEL_SIZE;
	}


	if (dx > 0) flipSprite = false;
	if (dx < 0) flipSprite = true;
	// dont flip if dx = 0

	// always sync because *.smoothnessss.*
	syncPixel();

}

void Entity::draw(RenderWindow& win) {
	// fix weird initialization error
	if (animationStartTime < 0) animationStartTime = Lib::getTimeStamp();
	if (abs(dx) > 0) setAnimation(1);
	else setAnimation(0);

	if (animationLoop[animationIndex]) {
		animationFrame = (int((Lib::getTimeStamp() - animationStartTime) / animationSpeed[animationIndex])) % animationLength[animationIndex];
	}
	else {
		animationFrame = (Lib::getTimeStamp() - animationStartTime) / animationLength[animationIndex];
	}

	sprite.setTextureRect(
		sf::IntRect(
			animationFrame * spriteWidth,
			animationIndex * spriteHeight,
			spriteWidth,
			spriteHeight
		)
	);

	sprite.setOrigin(spriteWidth / 2.f, 0);
	sprite.setScale(flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
	win.draw(sprite);
}

void Entity::setPositionPixel(float px, float py) {
	sprite.setPosition(px, py);
	cx = (int)px / C::GRID_SIZE;
	rx = ((1.0f * px / C::GRID_SIZE) - cx);

	cy = (int)py / C::GRID_SIZE;
	ry = ((1.0f * py / C::GRID_SIZE) - cy);
}

void Entity::setPositionCell(float cx, float cy) {
	int pixx = cx * C::GRID_SIZE;
	int pixy = cy * C::GRID_SIZE;
	this->cx = (int)cx;
	this->cy = (int)cy;

	rx = cx - this->cx;
	ry = cy - this->cy;

	sprite.setPosition(pixx, pixy);
}

void Entity::syncPixel() {
	float pixx = (cx + rx) * C::GRID_SIZE;
	float pixy = (cy + ry) * C::GRID_SIZE;
	//pixy -= texture.getSize().y / 2.0f;
	sprite.setPosition(pixx, pixy);
}

void Entity::move(float x, float y) {

}

void Entity::jump() {
	if (!isFloored) return;
	dy -= jumpPower;
}

void Entity::moveLeft(double dt) {

}

void Entity::moveRight(double dt) {

}

void Entity::setAnimation(int index) {
	if (animationIndex == index) return;

	animationStartTime = Lib::getTimeStamp();
	animationIndex = index;
}

void Entity::im()
{
	using namespace ImGui;
	if (TreeNode("player")) {
		Value("cx", cx);
		Value("cy", cy);
		Value("rx", rx);
		Value("ry", ry);
		Value("animationIndex", animationIndex);
		Value("animationFrame", animationFrame);
		TreePop();
	}
}