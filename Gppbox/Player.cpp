#include <imgui.h>
#include "Player.hpp"
#include "Game.hpp"
#include "Interp.hpp"
#include "Dice.hpp"

void Player::draw(RenderWindow& win) {
	Entity::draw(win);
	if (isAlive() && activeWeapon) activeWeapon->draw(*this, win);
}

void Player::update(double dt, Game& game) {
	Entity::update(dt, game);
	if (!isAlive()) return;

	lastDamageTaken += dt;
	if (activeWeapon) activeWeapon->update(*this, dt, game);

	if (lastDamageTaken > invulnerabilityTime) {
		for (Foe* f : game.foes) {
			if (Lib::getMagnitude(f->position - position) < 50.f) {
				takeDamage(1.f);
				lastDamageTaken = 0.f;
				game.updatePlayerHealth();
				Vector2f shake = f->position - position;
				Lib::normalize(shake);
				game.screenShake(shake);
			}
		}
	}
}

void Player::fire(Game& game) {
	if(activeWeapon && activeWeapon->canFire())
	{
		activeWeapon->fire(*this, game);
		dx = (flipSprite ? 1 : -1) * activeWeapon->recoil;
		sf::Vector2f shake;
		shake.x = (flipSprite ? 1 : -1) * activeWeapon->recoil *  4.f * C::PIXEL_SIZE;
		game.screenShake(shake);

		Particle bulletParticle = Particle("res/shell.png", position.x, position.y);
		bulletParticle.dy = -200.f;
		bulletParticle.dx = (flipSprite ? 1 : -1) * (300.f + Dice::randF() * 100.f);
		bulletParticle.sprite.setOrigin(1.5, 2);
		bulletParticle.sprite.rotate(Dice::randF() * 360.f);
		bulletParticle.angularSpeed = Dice::randF() * 1080.f;
		bulletParticle.collideWithWalls = true;
		bulletParticle.life = 3.f;
		bulletParticle.bhv = [](Particle* p, float dt) {
			p->dy += dt * C::GRAVITY * 100.f;
		};
		game.afterParts.add(bulletParticle);
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