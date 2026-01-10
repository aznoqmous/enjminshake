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
#include "Foe.hpp"
#include "LevelEditor.hpp"
#include "Drone.hpp"
#include "ParallaxLayer.hpp"

using namespace sf;

enum GameMode {
	PlayMode,
	EditMode,
};

class HotReloadShader;
class Game {
public:
	sf::RenderWindow* win = nullptr;

	sf::RectangleShape bg;
	HotReloadShader* bgShader = nullptr;

	sf::Texture	tex;
	sf::Font font;

	sf::View mainCamera;
	sf::Vector2f cameraPosition;
	float cameraZoom = 1.f;
	sf::Vector2f screenShakeOffset;
	sf::Vector2f screenShakeTarget;
	float screenShakeDuration = 0.3f;
	float screenShakeTime = 0.f;
	float screenShakePower = 3.f;
	float timeSpeed = 1.f;
	float timeFreezeSpeed = 50.f;

	bool closing = false;

	std::vector<sf::Vector2i>		walls;
	std::vector<sf::Sprite> wallSprites;
	sf::Texture foliageTexture;
	std::vector<sf::Sprite> foliageSprites;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Player player = Player(14, 0);
	Drone drone = Drone(14, 0);
	std::vector<Foe*> foes;
	std::vector<Foe*> deadFoes;
	std::vector<Bullet*> bullets;
	std::vector<ParallaxLayer* > parallaxLayers;
	LevelEditor levelEditor;

	GameMode mode = GameMode::PlayMode;

	sf::Text gameOverText;
	sf::Text playerHealthText;

	Game(sf::RenderWindow* win);

	~Game();

	void cacheWalls();

	float jumpTime = 0.f;
	float jumpDuration = 0.5f;
	bool wasPressed = false;
	bool twasPressed = false;
	bool tabWasPressed = false;
	bool isFiring = false;
	void processInput(sf::Event ev);
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();

	void screenShake(sf::Vector2f shake);
	void loadLevel();
	void resetLevel();
	void gameOver();
	void updatePlayerHealth();
};

