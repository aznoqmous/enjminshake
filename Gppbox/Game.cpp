
#include <imgui.h>
#include <array>
#include <vector>
#include <algorithm>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include <iostream>

static int cols = C::RES_X / C::GRID_SIZE;
static int lastLine = C::RES_Y / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {


	this->win = win;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg-clouds.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	walls.push_back(Vector2i(5.f, lastLine - 1));
	walls.push_back(Vector2i(15.f, lastLine - 1));

	/*
	walls.push_back(Vector2i(0, lastLine-1));
	walls.push_back(Vector2i(0, lastLine-2));
	walls.push_back(Vector2i(0, lastLine-3));

	walls.push_back(Vector2i(cols - 1, lastLine - 1));
	walls.push_back(Vector2i(cols-1, lastLine - 2));
	walls.push_back(Vector2i(cols-1, lastLine - 3));

	walls.push_back(Vector2i(cols >>2, lastLine - 2));
	walls.push_back(Vector2i(cols >>2, lastLine - 3));
	walls.push_back(Vector2i(cols >>2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	*/

	cacheWalls();
	

	entities.push_back(new Entity("res/foe.png", 5, 0));
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(C::GRID_SIZE, C::GRID_SIZE));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == Keyboard::K) {
			walls.clear();
			cacheWalls();
		}
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		player.moveLeft(dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.moveRight(dt);
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.dx = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		player.fire(*this);
	}

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	for (Entity* e : entities)
		e->update(dt, *this);
	
	for (Bullet* b : bullets)
		b->update(dt, *this);

	// remove dead bullets safely after updating them
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		if (!(*it)->isLive) {
			delete *it;
			it = bullets.erase(it);
		} else {
			++it;
		}
	}

	player.update(dt, *this);
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::RectangleShape & r : wallSprites)
		win.draw(r);

	for (sf::RectangleShape& r : rects) 
		win.draw(r);
	
	for (Entity* e : entities)
		e->draw(win);
	
	for (Bullet* b : bullets)
		b->draw(win);

	player.draw(win);


	afterParts.draw(win);
}

void Game::onSpacePressed() {
	player.jump();
}


bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}


void Game::im()
{
	for (Entity* e : entities) {
		e->im();
	}
}