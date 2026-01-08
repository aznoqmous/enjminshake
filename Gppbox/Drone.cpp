#include <imgui.h>
#include "Drone.hpp"
#include "Interp.hpp"
#include "Lib.hpp"
#include "Game.hpp"
#include "C.hpp"
#include "Missile.hpp"

void Drone::draw(RenderWindow& win) {
	//Entity::draw(win);
	sprite.setPosition(position);
	sprite.setOrigin(spriteWidth / 2.f + 2, 20);
	sprite.setScale(flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
	win.draw(sprite);
}
void Drone::update(double dt, Game& game){
	Entity::update(dt, game);
	
	Vector2f tPosition = targetPosition;
	tPosition.x = game.player.flipSprite ? tPosition.x : -tPosition.x;
	tPosition.y += sin(Lib::getTimeStamp() * Lib::pi()) * 12.f;
	tPosition.x += cos(Lib::getTimeStamp() * Lib::pi()) * 32.f;
	dx = Interp::lerp(dx, copysign(1.f, (game.player.position.x + tPosition.x) - position.x), dt * 5.f);
	dy = Interp::lerp(dy, copysign(1.f, (game.player.position.y + tPosition.y) - position.y), dt * 5.f);
	
	flipSprite = game.player.flipSprite;
	
	fireCooldown -= dt;
	if (fireCooldown <= 0) {
		fireCooldown = fireRate;
		Missile* miss = new Missile();
		miss->position = position;
		miss->velocity.x = flipSprite ? -1 : 1;
		Lib::rotate(miss->velocity, Dice::randF() * Dice::randSign() * 0.3f * Lib::pi() / 2.0f);
		game.bullets.push_back(miss);
	}
}

void Drone::im() {
	using namespace ImGui;
	if (TreeNode("Drone")) {
		Value("x", position.x);
		Value("y", position.y);
		TreePop();
	}
}