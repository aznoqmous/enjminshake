#include <imgui.h>
#include "Drone.hpp"
#include "Interp.hpp"
#include "Lib.hpp"
#include "Game.hpp"
#include "C.hpp"
#include "Missile.hpp"

void Drone::draw(RenderWindow& win) {
	sprite.setPosition(position);
	sprite.setOrigin(spriteWidth / 2.f + 2, 20);
	sprite.setScale((flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE) * scale, C::PIXEL_SIZE * scale);	
	win.draw(sprite);
}
void Drone::update(double dt, Game& game){
	
	Vector2f tPosition = targetPosition;
	tPosition.x = game.player.flipSprite ? tPosition.x : -tPosition.x;
	tPosition.y += sin(Lib::getTimeStamp() * Lib::pi()) * 12.f;
	tPosition.x += cos(Lib::getTimeStamp() * Lib::pi()) * 32.f;

	Vector2f direction = game.player.position + tPosition - position;
	Lib::normalize(direction);

	dx = direction.x;
	dy = direction.y;

	//dx = Interp::lerp(dx, copysign(1.f, (game.player.position.x + tPosition.x) - position.x), dt * 5.f);
	//dy = Interp::lerp(dy, copysign(1.f, (game.player.position.y + tPosition.y) - position.y), dt * 5.f);

	Entity::update(dt, game);
	
	flipSprite = game.player.flipSprite;
	
	fireCooldown -= dt;

	// teleport if too far from player
	if (Lib::getMagnitude(position - game.player.position) > 500.f) {
		scale = 0.f;
		position = game.player.position + tPosition;
		setPositionCell(position.x / C::GRID_SIZE, position.y / C::GRID_SIZE);
	}
	
}

void Drone::fire(Game& game) {
	if (fireCooldown <= 0) {
		ImVec2 mousePos = ImGui::GetMousePos();
		Vector2f mouse = Vector2f(mousePos.x, mousePos.y) + (Vector2f)(game.mainCamera.getCenter() - game.mainCamera.getSize() / 2.f);
		Vector2f target = (Vector2f)mouse - position;
		Lib::normalize(target);

		fireCooldown = fireRate;
		Missile* miss = new Missile();
		miss->position = position;
		//miss->velocity.x = flipSprite ? -1 : 1;
		miss->velocity = target;
		//Lib::rotate(miss->velocity, Dice::randF() * Dice::randSign() * 0.3f * Lib::pi() / 2.0f);
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