#include "Player.h"


void Player::draw(RenderWindow& win) {
	Entity::draw(win);

	win.draw(weaponSprite);

	weaponSprite.setOrigin(
		weaponOriginX, 
		weaponOriginY 
		+ floor(sin(Lib::getTimeStamp() * Lib::pi() * 2.f) / 2) * 2
	);
	weaponSprite.setScale(flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
	weaponSprite.setPosition(sprite.getPosition());
}
void Player::setWeaponIndex(int index) {
	weaponSprite.setTextureRect(
		sf::IntRect(
			0,
			index * weaponSpriteHeight,
			weaponSpriteWidth,
			weaponSpriteHeight
		)
	);
}