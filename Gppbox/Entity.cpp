#include "Entity.h"
#include <iostream>
#include <imgui.h>
#include "C.hpp"

Entity::Entity() {
	if (!texture.loadFromFile("C:/Users/paull/cnam/cpp/enjminshake/Gppbox/res/peer-idle.png")) {
		std::cerr << "Fail to load texture !" << std::endl;
	}
	else {
		std::cout << "Texture loaded !" << std::endl;
		sprite.setTexture(texture);
	}
}

void Entity::update(double dt, Game& game) {
	std::cout << xr << std::endl;
	
	bool sync = false;
	while (xr > 1.0f) {
		xr--;
		cx++;
		sync = true;
	}
	while (xr < 0.0f) {
		xr++;
		cx--;
		sync = true;
	}
	while (yr > 1.0f) {
		yr--;
		cy++;
		sync = true;
	}
	while (yr < 0.0f) {
		yr++;
		cy--;
		sync = true;
	}

	if (!game.isWall(cx + dx * dt, cy)) {
		xr += dx * dt;
	}
	if (!game.isWall(cx, cy + dy * dt)) {
		yr += dy * dt;
	}

	if (sync) syncPixel();
	
}


void Entity::setPositionPixel(float px, float py){
	sprite.setPosition(px, py);
	cx = (int) px / C::GRID_SIZE;
	xr = ((1.0f * px / C::GRID_SIZE) - cx);

	cy = (int)py / C::GRID_SIZE;
	yr = ((1.0f * py / C::GRID_SIZE) - cy);
}

void Entity::setPositionCell(float cx, float cy){
	int pixx = cx * C::GRID_SIZE;
	int pixy = cy * C::GRID_SIZE;
	this->cx = (int) cx;
	this->cy = (int) cy;

	xr = cx - this->cx;
	yr = cy - this->cy;

	sprite.setPosition(pixx, pixy);
}

void Entity::syncPixel() {
	int pixx = (cx + xr) * C::GRID_SIZE;
	int pixy = (cy + yr) * C::GRID_SIZE;
	sprite.setPosition(pixx, pixy);
}

void Entity::move(float x, float y) {

}

void Entity::moveLeft(double dt) {

}

void Entity::moveRight(double dt) {

}

void Entity::im()
{
	using namespace ImGui;
	if (TreeNode("player")) {
		Value("xr", xr);
		Value("yr", yr);
		TreePop();
	}
}