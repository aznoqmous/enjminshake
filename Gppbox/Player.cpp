#include <imgui.h>
#include "Player.hpp"
#include "Game.hpp"
#include "Interp.hpp"
void Player::draw(RenderWindow& win) {
	Entity::draw(win);
	if (activeWeapon) activeWeapon->draw(*this, win);
	//debugRectangle.setPosition(position);
	//win.draw(debugRectangle);
}

void Player::update(double dt, Game& game) {
	Entity::update(dt, game);
	if (activeWeapon) activeWeapon->update(*this, dt, game);
}

void Player::fire(Game& game) {
	if(activeWeapon && activeWeapon->canFire())
	{
		activeWeapon->fire(*this, game);
		dx = (flipSprite ? 1 : -1) * activeWeapon->recoil;
		activeWeapon->offset.x += (flipSprite ? 1 : -1) * .5f * C::PIXEL_SIZE;
		sf::Vector2f shake;
		shake.x = (flipSprite ? 1 : -1) * activeWeapon->recoil *  4.f * C::PIXEL_SIZE;
		game.screenShake(shake);
	}
}

void Player::im() {
	using namespace ImGui;
	if (TreeNode("player")) {
		Value("position.x", position.x);
		Value("position.y", position.y);
		InputFloat("recoil", &activeWeapon->recoil);
		
		TreePop();
	}
}