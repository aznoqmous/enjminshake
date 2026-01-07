#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

using namespace sf;

class HotReloadShader;
class Game {
public:
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	sf::View mainCamera;
	sf::Vector2f cameraPosition;
	sf::Vector2f screenShakeOffset;
	sf::Vector2f screenShakeTarget;
	float screenShakeDuration = 1.f;
	float screenShakeTime = 0.f;

	bool							closing = false;
	
	std::vector<sf::Vector2i>		walls;
	std::vector<sf::RectangleShape> wallSprites;

	ParticleMan beforeParts;
	ParticleMan afterParts;


	Player player = Player(14, 0);
	std::vector<Entity*> entities;
	std::vector<Bullet*> bullets;


	Game(sf::RenderWindow * win);

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();

	void screenShake(sf::Vector2f shake);
};