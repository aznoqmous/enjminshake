#include "Weapon.hpp"
#include "C.hpp"
#include "Interp.hpp"
#include "Bullet.hpp"
#include "Game.hpp"
#include "Lib.hpp"

void Weapon::draw(Entity& entity, RenderWindow& win) {

	sprite.setOrigin(origin + offset);
	sprite.setScale(entity.flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
	sprite.setPosition(entity.sprite.getPosition());
	win.draw(sprite);

	if (isDrawMuzzleFire || fireRate - fireCooldown < 0.05f)
	{
		muzzleFireSprite.setPosition(entity.sprite.getPosition());
		muzzleFireSprite.setOrigin(muzzleFireOrigin);
		muzzleFireSprite.setScale(entity.flipSprite ? -C::PIXEL_SIZE : C::PIXEL_SIZE, C::PIXEL_SIZE);
		win.draw(muzzleFireSprite);
		isDrawMuzzleFire = false;
	}

	if (type == WeaponLaser && fireRate - fireCooldown < 0.05f) {
		firePosition = entity.position + muzzleFireOrigin - Vector2f(entity.flipSprite ? 32.f : -32.f, 12.f);
		laserVertices[0].position = firePosition;
		laserVertices[0].color = sf::Color::Yellow;
		float vcount = laserVertices.getVertexCount();
		laserVertices[vcount-1].position = fireTarget;
		for (int i = 1; i < vcount - 1; i++) {
			laserVertices[i].position.x = Interp::lerp(firePosition.x, fireTarget.x, (float) i / vcount);
			laserVertices[i].position.y = Interp::lerp(firePosition.y, fireTarget.y, (float)i / vcount) + sin(Dice::randF() * Lib::pi() * 2.f) * 10.f;
			laserVertices[i].color = Color::Yellow;
		}
		win.draw(laserVertices);
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

	if (type == WeaponBullet) {
		Bullet* bullet = new Bullet();
		bullet->position = entity.position + muzzleFireOrigin - Vector2f(entity.flipSprite ? 32.f : -32.f, 12.f);
		bullet->velocity.x = entity.flipSprite ? -1 : 1;
		Lib::rotate(bullet->velocity, Dice::randF() * Dice::randSign() * spray * Lib::pi() / 2.0f);
		game.bullets.push_back(bullet);
		isDrawMuzzleFire = true;
	}
	if (type == WeaponLaser) {
		firePosition = entity.position + muzzleFireOrigin - Vector2f(entity.flipSprite ? 32.f : -32.f, 12.f);
		float nearest = 10000.0f;
		
		Vector2i* hittedWall = nullptr;
		int x = floor(firePosition.x / (float) C::GRID_SIZE);
		int y = floor(firePosition.y / (float)C::GRID_SIZE);
		for (Vector2i wall : game.walls) {
			if (wall.y != y) continue;
			float distX = x  - wall.x;
			if (
				nearest > abs(distX)
				&& (entity.flipSprite ? wall.x < firePosition.x / C::GRID_SIZE : wall.x > firePosition.x / C::GRID_SIZE)
			) {
				nearest = abs(distX);
				hittedWall = &wall;
			}
		}

		Foe* hittedFoe = nullptr;
		for (Foe* foe : game.foes) {
			int fx = foe->position.x / C::GRID_SIZE;
			int fy = foe->position.y / C::GRID_SIZE;
			if (abs(fy - y) > 1.f) continue;
			float distX = x - fx;
			if (
				nearest > abs(distX)
				&& (entity.flipSprite ? fx < firePosition.x / C::GRID_SIZE : fx > firePosition.x / C::GRID_SIZE)
				) {
				nearest = abs(distX);
				hittedFoe = foe;
			}
		}
		
		fireTarget = firePosition + Vector2f(nearest * (entity.flipSprite ? -1 : 1), 0) * (float) C::GRID_SIZE;

		if (hittedFoe) {
			hittedFoe->takeDamage(1.f);
			fireTarget = hittedFoe->position;
		}
		if (hittedFoe || hittedWall) {
			Particle p = Particle("res/bullet-hit.png", fireTarget.x, fireTarget.y);
			p.life = 0.3f;
			p.bhv = [](Particle* p, float dt) {
				p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.3f) * 4) * 16, 0, 16, 16));
				};
			game.afterParts.add(p);
		}

	}
	
}