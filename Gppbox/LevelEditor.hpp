#pragma once
#include <SFML/Graphics.hpp>
#include "Lib.hpp"

class Game;

enum TileType {
	Empty, // empty tiles must be destroyed
	Wall,
	Enemy,
	PlayerSpawn
};


class Tile {
public:
	TileType type;
	sf::Texture texture;
	sf::Sprite sprite;
	int spriteWidth = 32;
	int spriteHeight = 32;

	Tile() : Tile(Empty){}
	Tile(TileType t) {
		sprite = Lib::loadSprite(texture, "res/leveleditor-tiles.png");
		setType(t);
	}

	void setType(TileType t) {
		type = t;
		sprite.setTextureRect(sf::IntRect(
			static_cast<int>(t) * spriteWidth,
			0,
			spriteWidth, spriteHeight
		));
	}
};


class LevelEditor {
public:
	std::string saveFile = "res/save.dat";

	std::vector<Tile> tileTypes;
	TileType selectedType;

	struct Vec2iLess {
		bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
			if (a.x != b.x) return a.x < b.x;
			return a.y < b.y;
		}
	};

	std::map<sf::Vector2i, TileType, Vec2iLess> tiles;
	sf::Vector2i gridMousePos;
	sf::Vector2i spawnPosition;
	float cameraZoom = 1.f;

	LevelEditor();

	void update(double dt, Game& game);
	void draw(sf::RenderWindow& win);

	void handleInputs(double dt, Game& game);
	void im(Game& game);
	void setTile(sf::Vector2i position, TileType type, Game& game);
	void drawTile(TileType t, sf::Vector2f position, sf::RenderWindow& win);

	void saveLevel();
	void loadLevel(Game& game);
};