#include <fstream>
#include <imgui.h>
#include "LevelEditor.hpp"
#include "Game.hpp"
#include <algorithm>
#include "C.hpp"
#include "Lib.hpp"
#include <imgui-SFML.h>

using namespace ImGui;
using namespace sf;

LevelEditor::LevelEditor() {
    selectedType = TileType::Empty;
    tileTypes.clear();
    tileTypes.emplace_back(Empty);
    tileTypes.emplace_back(Wall);
    tileTypes.emplace_back(SolidWall);
    tileTypes.emplace_back(Enemy);
    tileTypes.emplace_back(PlayerSpawn);

    for (int i = 0; i < (int)tileTypes.size(); ++i) {
        tileTypes[i].sprite.setTexture(tileTypes[i].texture);
        tileTypes[i].sprite.setTextureRect(sf::IntRect(i * tileTypes[i].spriteWidth, 0, tileTypes[i].spriteWidth, tileTypes[i].spriteHeight));
    }

    controlsText.setString("ZQSD : Move camera / F : Focus player / C : Call player / Mousewheel : Zoom");
}

void LevelEditor::update(double dt, Game& game) {
    cameraZoom = game.cameraZoom;
	handleInputs(dt, game);
    controlsText.setPosition(
        game.cameraPosition + Vector2f(
            0.f,
            game.mainCamera.getSize().y / 2.f - 48.f
        )
    );
    controlsText.setOrigin(Vector2f(controlsText.getLocalBounds().width / 2.f, 0.f));
}

void LevelEditor::draw(RenderWindow& win){
    ImGui::Value("cameraZoom", cameraZoom);
    View view = win.getView();
    FloatRect viewport = view.getViewport();

    // DRAW GRID
    RectangleShape cell(Vector2f(C::GRID_SIZE, C::GRID_SIZE));
    cell.setFillColor(Color::Transparent);
    cell.setOutlineColor(Color(100, 100, 100, 255));
    cell.setOutlineThickness(1.f);
    
    float minx = floor(viewport.left);

    Vector2f min = view.getCenter() - view.getSize() / 2.f;
    min = Vector2f(floor(min.x / C::GRID_SIZE), floor(min.y / C::GRID_SIZE));

    for (int y = 0; y < win.getSize().y / C::GRID_SIZE * cameraZoom + 1; ++y)
    {
        for (int x = 0; x < win.getSize().x / C::GRID_SIZE * cameraZoom + 1; ++x)
        {
            cell.setPosition((x + min.x) * C::GRID_SIZE, (y + min.y)*C::GRID_SIZE);
            win.draw(cell);
        }
    }

    
    // DRAW PLACED TILES
    for (auto& kv : tiles) {
        const sf::Vector2i& pos = kv.first;
        TileType type = kv.second;
        if (type == Empty) continue;
        drawTile(type, Vector2f(pos.x * C::GRID_SIZE, pos.y * C::GRID_SIZE), win);
    }


    if (ImGui::IsAnyItemHovered()) {
        return;
    }

    // DRAW CURSOR
    ImVec2 mousePos = GetMousePos();
    gridMousePos = Vector2i(mousePos.x * cameraZoom, mousePos.y * cameraZoom) + (Vector2i)(view.getCenter() - view.getSize() / 2.f);
    ImGui::Value("mousex", gridMousePos.x);
    ImGui::Value("mousey", gridMousePos.y);
    gridMousePos.x = floor((float) gridMousePos.x / (float) C::GRID_SIZE);
    gridMousePos.y = floor((float) gridMousePos.y / (float) C::GRID_SIZE);

    RectangleShape selectedCell(Vector2f(C::GRID_SIZE, C::GRID_SIZE));
    selectedCell.setFillColor(Color::Transparent);
    selectedCell.setOutlineColor(Color::White);
    selectedCell.setOutlineThickness(1.f);
    selectedCell.setFillColor(Color(255, 255, 255, 120));
    selectedCell.setPosition(gridMousePos.x * C::GRID_SIZE, gridMousePos.y * C::GRID_SIZE);
    win.draw(selectedCell);
    
    drawTile(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? Empty : selectedType, Vector2f(gridMousePos.x * C::GRID_SIZE, gridMousePos.y * C::GRID_SIZE), win);

    // DRAW CONTROLS
    win.draw(controlsText);
}



void LevelEditor::handleInputs(double dt, Game& game) {
    if (ImGui::IsAnyItemHovered()){
        return;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        setTile(gridMousePos, selectedType, game);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        setTile(gridMousePos, Empty, game);
    }
} 

void LevelEditor::setTile(Vector2i position, TileType type, Game& game) {
    
    if (type == PlayerSpawn) {
        tiles.erase(spawnPosition);
        spawnPosition = position;
    }

    if (tiles.find(position) == tiles.end()) {
    }
    else {
        if (tiles[position] == type) return;
    }
    tiles[position] = type;

    switch (type) {
    case Empty:
        tiles.erase(position);
        break;
    case Wall:
        //game.walls.push_back(position);
        //game.cacheWalls();
        break;
    case Enemy:
        //game.foes.push_back(new Foe(position.x, position.y));
        break;
    }
    
}

void LevelEditor::drawTile(TileType t, sf::Vector2f position, sf::RenderWindow& win) {
    tileTypes[static_cast<int>(t)].sprite.setPosition(position);
    tileTypes[static_cast<int>(t)].sprite.setOrigin({8, 8});
    tileTypes[static_cast<int>(t)].sprite.setScale(C::PIXEL_SIZE, C::PIXEL_SIZE);
    win.draw(tileTypes[static_cast<int>(t)].sprite);
}
    
void LevelEditor::im(Game& game) {
    if (CollapsingHeader("LevelEditor")) {
        Value("tiles", static_cast<int>(tiles.size()));
        Value("gridx", gridMousePos.x);
        Value("gridy", gridMousePos.y);
        std::string names[5] = { "Selected : Empty", "Selected : Wall", "Selected : SolidWall", "Selected : Enemy", "Selected : PlayerSpawn" };
        ImGui::Text(names[static_cast<int>(selectedType)].c_str());
        if (Button("Empty")) {
            selectedType = TileType::Empty;
        }
        if (Button("Wall")) {
            selectedType = TileType::Wall;
        }
        if (Button("SolidWall")) {
            selectedType = TileType::SolidWall;
        }
        if (Button("Enemy")) {
            selectedType = TileType::Enemy;
        }
        if (Button("PlayerSpawn")) {
            selectedType = TileType::PlayerSpawn;
        }
        if (Button("Save")) {
            saveLevel();
        }
        if (Button("Load")) {
            loadLevel(game);
        }
        if (Button("Clear")) {
            game.player.setPositionCell(0, 0);
            game.player.syncPixel();
            game.cameraPosition = game.player.position;
            
            tiles.clear();
            game.loadLevel();
        }
    }
    
}
void LevelEditor::saveLevel() {
    std::ofstream file(saveFile);
    if (!file.is_open()) {
        std::cerr << "Failed to access save file !" << std::endl;
        return;
    }

    for (const auto& [pos, type] : tiles)
    {
        file << pos.x << ' ' << pos.y << ' ' << type << '\n';
    }
    std::cout << "Level saved !" << std::endl;

}

void LevelEditor::loadLevel(Game& game) {
    std::ifstream file(saveFile);
    if (!file.is_open())
        return;

    tiles.clear();

    int x, y, type;
    while (file >> x >> y >> type)
    {
        TileType t = static_cast<TileType>(type);
        tiles[{x, y}] = t;
        if (t == PlayerSpawn) {
            spawnPosition = Vector2i(x, y);
            game.player.position = Vector2f(spawnPosition);
            game.player.setPositionCell(game.player.position.x, game.player.position.y);
        }
    }
    game.loadLevel();
}