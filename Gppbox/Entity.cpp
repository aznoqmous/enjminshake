#include <iostream>
#include <imgui.h>
#include "Interp.hpp"
#include "Entity.hpp"
#include "C.hpp"
#include "Game.hpp"

Entity::Entity(const std::string& texturePath) {
	sprite = Lib::loadSprite(texture, texturePath);
}

Entity::Entity(const std::string& texturePath, int cx, int cy) {
	setPositionCell(cx, cy);
	sprite = Lib::loadSprite(texture, texturePath);
}

void Entity::update(double dt, Game& game) {


	/*
	isFloored = game.isWall(cx, cy + 1);
	if (!isFloored) {
		dy += dt * C::GRAVITY;
	}
	else if (dy >= 0.0f) {
		setPositionCell(cx + rx, cy);
		dy = 0.0f;
	}
	*/
	/*
	if (!game.isWall(cx + dx * dt, cy)) {
		rx += dx * dt * speed * C::PIXEL_SIZE;
	}
	else {
		dx = 0.0f;
	}

	if (!game.isWall(cx, cy + dy * dt)) {
		ry += dy * dt * speed * C::PIXEL_SIZE;
	}
	*/

	rx += dx * dt * speed * C::PIXEL_SIZE;
	dx = Interp::lerp(dx, 0.f, dt);
	if (game.isWall(cx + 1, cy) && rx >= 0.7) {
		rx = 0.7;
		dx = 0;
	}
	if (game.isWall(cx - 1, cy) && rx <= 0.3) {
		rx = 0.3;
		dx = 0;
	}

	ry += dy * dt * speed * C::PIXEL_SIZE;
	dy += dt * C::GRAVITY;
	//dy = Interp::lerp(dx, 0.f, dt);
	if (game.isWall(cx, cy + 1) && ry >= 0.7) {
		ry = 0.7;
		dy = 0;
	}
	if (game.isWall(cx, cy - 1) && ry <= 0.3) {
		ry = 0.3;
		dy = 0;
	}

	isFloored = game.isWall(cx, cy + 1);

	while (rx > 1.0f) { rx--; cx++; }
	while (rx < 0.0f) { rx++; cx--; }
	while (ry > 1.0f) { ry--; cy++; }
	while (ry < 0.0f) { ry++; cy--; }

	//if (dx > 0) flipSprite = false;
	//if (dx < 0) flipSprite = true;
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

	sprite.setOrigin(spriteWidth / 2.f, spriteHeight / 2.f);
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
	position.x = (cx + rx) * C::GRID_SIZE;
	position.y = (cy + ry) * C::GRID_SIZE;
	sprite.setPosition(position);
}

void Entity::move(float x, float y) {

}

void Entity::jump() {
	if (!isFloored) return;
	dy -= jumpPower;
}

void Entity::moveLeft(double dt) {
	flipSprite = true;
	dx = Interp::lerp(dx, -1.0f, dt * 100.f);
}

void Entity::moveRight(double dt) {
	flipSprite = false;
	dx = Interp::lerp(dx, 1.0f, dt * 100.f);
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