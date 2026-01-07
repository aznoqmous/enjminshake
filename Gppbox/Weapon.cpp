#include "Weapon.hpp"
#include "C.hpp"
#include "Interp.hpp"
#include "Bullet.hpp"
#include "Game.hpp"

void Weapon::draw(Entity& entity, RenderWindow& win) {

	sprite.setOrigin(origin + offset);
	sprite.setScale(entity.flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
	sprite.setPosition(entity.sprite.getPosition());
	win.draw(sprite);

	if (fireRate - fireCooldown < 0.1f)
	{
		muzzleFireSprite.setPosition(entity.sprite.getPosition());
		muzzleFireSprite.setOrigin(muzzleFireOrigin);
		muzzleFireSprite.setScale(entity.flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
		win.draw(muzzleFireSprite);
	}
}

void Weapon::setWeaponIndex(int index) {
	sprite.setTextureRect(
		sf::IntRect(
			0,
			index * spriteHeight,
			spriteWidth,
			spriteHeight
		)
	);
}

void Weapon::update(Entity& entity, double dt, Game& game) {
	offset = Interp::lerp(offset, Vector2f(0, entity.dy * 3.f), dt * 50.f);
	fireCooldown -= dt;
	fireCooldown = clamp(fireCooldown, 0.f, fireRate);
}

bool Weapon::canFire() {
	return fireCooldown <= 0.f;
}

void Weapon::fire(Entity& entity, Game& game) {
	fireCooldown = fireRate;
	Bullet* bullet = new Bullet();
	bullet->position = entity.position + muzzleFireOrigin;
	bullet->velocity.x = entity.flipSprite ? -1 : 1;
	game.bullets.push_back(bullet);
}