
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
	bg.setOrigin(sf::Vector2f(C::RES_X, C::RES_Y) / 2.f);
	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");


	foliageTexture.loadFromFile("res/foliages.png");

	parallaxLayers.push_back(new ParallaxLayer("res/parallax-2.png", 0.5f));
	parallaxLayers.push_back(new ParallaxLayer("res/parallax-1.png", 0.25f));
	
	// Texts
	for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	if (!font.loadFromFile("res/cc.yal.7w7.block.ttf")) {
		cout << "ERROR NO FONT" << endl;
		return;
	}
	gameOverText.setString("Press R to restart");
	gameOverText.setFont(font);
	gameOverText.setOrigin(Vector2f(gameOverText.getLocalBounds().width / 2.f, gameOverText.getLocalBounds().height / 2.f));
	playerHealthText.setFont(font);

	vignetteSprite = Lib::loadSprite(*(new Texture()), "res/vignette.png");

	resetLevel();
}

void Game::cacheWalls()
{
	foliageSprites.clear();

	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::Sprite sprite;
		int i = levelEditor.tiles[w];
		sprite.setTexture(levelEditor.tileTypes[levelEditor.tiles[w]].texture);
		sprite.setTextureRect(sf::IntRect(i * levelEditor.tileTypes[i].spriteWidth, 0, levelEditor.tileTypes[i].spriteWidth, levelEditor.tileTypes[i].spriteHeight));
		//sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		sprite.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		sprite.setOrigin({ 8, 8 });
		sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
		wallSprites.push_back(sprite);

		if (levelEditor.tiles[w + Vector2i(0, -1)] != Wall && levelEditor.tiles[w + Vector2i(0, -1)] != SolidWall) {
			sf::Sprite fSprite;
			fSprite.setTexture(foliageTexture);
			fSprite.setTextureRect(sf::IntRect((int) Dice::randF() * (64 - 16), 0, 16, 16));
			fSprite.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE - 8.f * C::PIXEL_SIZE);
			fSprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
			foliageSprites.push_back(fSprite);
		}
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
	
	if (mode == EditMode && ev.type == sf::Event::MouseWheelMoved)
	{
		if (ImGui::IsAnyItemHovered()) return;
		float target = cameraZoom * (1.f - ev.mouseWheel.delta / 10.f);
		if (target > 0.4f && target < 2.f) {
			cameraZoom *= 1.f - ev.mouseWheel.delta / 10.f;
			mainCamera.zoom(1.f - ev.mouseWheel.delta / 10.f);
		}
		//std::cout << ev.mouseWheel.delta << '\n';
	}

	if (mode == PlayMode && ev.type == sf::Event::MouseWheelMoved)
	{
		player.activeWeaponIndex = (player.activeWeaponIndex + (int) copysign(1, ev.mouseWheel.delta)) % player.weapons.size();
		player.activeWeapon = player.weapons[player.activeWeaponIndex];
		//std::cout << ev.mouseWheel.delta << '\n';
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		if (!tabWasPressed) {
			mode = mode == EditMode ? PlayMode : EditMode;
			tabWasPressed = true;

			if (mode == PlayMode) {
				loadLevel();
			}
		}
	}
	else {
		tabWasPressed = false;
	}

	if (mode == EditMode) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
			cameraPosition += Vector2f(-dt * 1000.f, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			cameraPosition += Vector2f(dt * 1000.f, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
			cameraPosition += Vector2f(0, -dt * 1000.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			cameraPosition += Vector2f(0, dt * 1000.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
			
			player.setPositionCell(levelEditor.gridMousePos.x, levelEditor.gridMousePos.y);
			player.syncPixel();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {

			cameraPosition = player.position;
		}
	}

	if (mode == PlayMode && player.isAlive()) {
		isFiring = false;
		float horizontalControllerInput = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float deadzone = 15.f;
		ImGui::Value("horizontal", horizontalControllerInput);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Joystick::isButtonPressed(0, 5)) {
			player.fire(*this);
			isFiring = true;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) || sf::Joystick::isButtonPressed(0, 4)) {
			drone.fire(*this);
		}
		bool isFlipped = player.flipSprite;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) || (horizontalControllerInput < 0 && fabs(horizontalControllerInput) > deadzone)) {
			player.moveLeft(dt);
			if(isFiring){
				player.flipSprite = isFlipped;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || (horizontalControllerInput > 0 && fabs(horizontalControllerInput)> deadzone)) {
			player.moveRight(dt);
			if (isFiring) {
				player.flipSprite = isFlipped;
			}
		}

		jumpTime += dt;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Joystick::isButtonPressed(0, 0)) {
			if (jumpTime < jumpDuration) {
				player.dy -= dt * 5.f;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
			if (!twasPressed) {
				debugDisplay = !debugDisplay;
				twasPressed = true;
			}
		}
		else {
			twasPressed = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Joystick::isButtonPressed(0, 0)) {
			if (!wasPressed) {
				onSpacePressed();
				wasPressed = true;
			}
		}
		else {
			wasPressed = false;
		}

	}

	if (mode == PlayMode && !player.isAlive() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
		resetLevel();
		
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

	for (ParallaxLayer* pl : parallaxLayers)
		pl->update(dt, *this);

	if (mode == PlayMode) {

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
		drone.update(dt, *this);

		cameraPosition = Interp::lerp(cameraPosition, player.position + Vector2f(0, -200.f), dt * 10.f);
	}
	screenShakeOffset.x = 0;
	screenShakeOffset.y = 0;
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

	if (mode == PlayMode) {
		mainCamera.zoom(1.f / cameraZoom);
		cameraZoom = 1.f;
	}
	//mainCamera.zoom(cameraZoom);

	if (mode == EditMode) {
		levelEditor.update(dt, *this);
	}

	bg.setPosition(cameraPosition);
	bg.setOrigin(sf::Vector2f(C::RES_X, C::RES_Y) / 2.f);

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
	
	for (ParallaxLayer* pl : parallaxLayers)
		pl->draw(win);

	beforeParts.draw(win);

	if (mode == PlayMode) {
		Vector2f size = mainCamera.getSize();

		for (sf::Sprite& r : wallSprites) {
			Vector2f pos = r.getPosition();
			if (
				cameraPosition.x < pos.x + size.x/2.f  + 16.f * C::PIXEL_SIZE && cameraPosition.x + size.x / 2.f > pos.x
				&& cameraPosition.y < pos.y + size.y/2.f  + 16.f * C::PIXEL_SIZE && cameraPosition.y + size.y / 2.f > pos.y
				) {
				win.draw(r);
			}
		}

		/*for (Vector2i& wall : walls) {
			win.draw(levelEditor.tileTypes[levelEditor.tiles[wall]].sprite);
			levelEditor.drawTile(levelEditor.tiles[wall], (Vector2f) wall * (float) C::GRID_SIZE, win
			);
		}*/
		for (sf::Sprite& r : foliageSprites)
			win.draw(r);
	}

	for (sf::RectangleShape& r : rects) 
		win.draw(r);
	
	for (Foe* e : deadFoes)
		e->draw(win);

	for (Foe* e : foes)
		e->draw(win);
	
	player.draw(win);
	drone.draw(win);

	afterParts.draw(win);

	for (Bullet* b : bullets)
		b->draw(win);

	if(mode == EditMode) levelEditor.draw(win);

	if (mode == PlayMode) {
		vignetteSprite.setPosition(cameraPosition - mainCamera.getSize() / 2.f);
		Vector2f camSize = mainCamera.getSize();
		Vector2f vignetteSize = (Vector2f) vignetteSprite.getTexture()->getSize();
		vignetteSprite.setScale(camSize.x / vignetteSize.x, camSize.y / vignetteSize.y);
		
		win.draw(vignetteSprite);
		
		if (player.isAlive()) {
			playerHealthText.setPosition(cameraPosition - mainCamera.getSize() / 2.f + Vector2f(20.f, 0.f));
			win.draw(playerHealthText);
		}
		else {
			gameOver();
			gameOverText.setPosition(cameraPosition + Vector2f(0, 10.f * sin(Lib::getTimeStamp())));
			win.draw(gameOverText);
		}
	}
}

void Game::onSpacePressed() {
	if (player.isCoyoteFloored())
	{
		player.jump();
		jumpTime = 0.f;
	}
}


bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

bool Game::isBreakableWall(Vector2i pos){
	return levelEditor.tiles[pos] == Wall;
}


void Game::im()
{
	using namespace ImGui;

	if (mode == EditMode && Button("PlayMode")) {
		mode = PlayMode;
	}
	if (mode == PlayMode && Button("EditMode")) {
		mode = EditMode;
	}

	if (TreeNode("game")) {
		InputFloat("timeFreezeSpeed", &timeFreezeSpeed);
		InputFloat("screenShakeDuration", &screenShakeDuration);
		InputFloat("screenShakePower", &screenShakePower);
		Value("foes", static_cast<int>(foes.size()));
		Value("deadFoes", static_cast<int>(deadFoes.size()));
		TreePop();
	}
	if (mode == EditMode) {
		levelEditor.im(*this);
	}
	if (mode == PlayMode) {
		player.im();
		drone.im();
		for (Foe* f : foes) f->im();
	}
	
}

void Game::screenShake(sf::Vector2f shake){
	screenShakeTime = screenShakeDuration;
	screenShakeTarget = shake;
}

void Game::loadLevel() {
	walls.clear();
	// delete previous foes to avoid leaks
	for (Foe* f : foes) delete f;
	foes.clear();
	for (Foe* f : deadFoes) delete f;
	deadFoes.clear();
	for (auto& kv : levelEditor.tiles) {
		const sf::Vector2i& pos = kv.first;
		TileType type = kv.second;
		switch (type) {
		case Wall:
		case SolidWall:
			walls.push_back(pos);
			break;
		case Enemy:
			foes.push_back(new Foe(pos.x, pos.y));
			break;
		}
	}
	cacheWalls();
}
void Game::resetLevel() {
	levelEditor.loadLevel(*this);
	loadLevel();
	cameraPosition = player.position;
	player.currentHp = player.maxHp;
	updatePlayerHealth();
}
void Game::gameOver() {
	
}

void Game::updatePlayerHealth() {
	playerHealthText.setString("HP:" + to_string((int)player.currentHp) + "/" + to_string((int)player.maxHp));
	playerHealthText.setCharacterSize(60);
}

Game::~Game() {
	if (bgShader) {
		delete bgShader;
		bgShader = nullptr;
	}
	for (Foe* f : foes) delete f;
	for (Foe* f : deadFoes) delete f;
	for (Bullet* b : bullets) delete b;
}