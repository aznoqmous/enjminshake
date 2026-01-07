
#include <imgui.h>
#include <array>
#include <vector>
#include <algorithm>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include <iostream>
#include "Interp.hpp"

static int cols = C::RES_X / C::GRID_SIZE;
static int lastLine = C::RES_Y / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {

	this->win = win;
	mainCamera = win->getDefaultView();

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
	

	foes.push_back(new Foe(7, 0));
	foes.push_back(new Foe(10, 0));

	cameraPosition = player.position;
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
		if (!twasPressed) {
			foes.push_back(new Foe(7, 0));
			twasPressed = true;
		}
	}
	else {
		twasPressed = false;
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

	for (Foe* e : foes)
		e->update(dt, *this);
	
	for (auto it = foes.begin(); it != foes.end(); ) {
		if (!(*it)->isAlive()) {
			deadFoes.push_back((*it));
			it = foes.erase(it);
		}
		else {
			++it;
		}
	}

	for (Foe* f : deadFoes)
		f->update(dt, *this);

	for (Bullet* b : bullets)
		b->update(dt, *this);

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		if (!(*it)->isLive) {
			delete *it;
			it = bullets.erase(it);
		} else {
			++it;
		}
	}



	player.update(dt, *this);
	
	screenShakeOffset.x = 0;
	screenShakeOffset.y = 0;

	
	

	cameraPosition = Interp::lerp(cameraPosition, player.position + Vector2f(0, -200.f), dt * 10.f);
	screenShakeOffset = Interp::lerp(screenShakeTarget, screenShakeOffset, dt * 10.f);
	screenShakeTime -= dt;
	screenShakeTime = clamp(screenShakeTime, 0.f, screenShakeDuration);
	mainCamera.setCenter(
		cameraPosition 
		+ (screenShakeOffset
			+ Vector2f(
				sin(Lib::getTimeStamp() * Lib::pi() * screenShakeOffset.y),
				cos((Lib::getTimeStamp() + 0.5f) * Lib::pi() * screenShakeOffset.x)
			) * screenShakePower
			)
			* Interp::lerp(0.0f, 1.0f, screenShakeTime / screenShakeDuration)
	);

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
	
	for (Foe* e : deadFoes)
		e->draw(win);

	for (Foe* e : foes)
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
	using namespace ImGui;
	if (TreeNode("game")) {
		InputFloat("screenShakeDuration", &screenShakeDuration);
		InputFloat("screenShakePower", &screenShakePower);
		Value("foes", static_cast<int>(foes.size()));
		Value("deadFoes", static_cast<int>(deadFoes.size()));
		TreePop();
	}
	player.im();
	for (Foe* f : foes) f->im();
}

void Game::screenShake(sf::Vector2f shake){
	screenShakeTime = screenShakeDuration;
	screenShakeTarget = shake;
}