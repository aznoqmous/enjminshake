# aznoqmous/enjminshake

Enjmin CPP gameplay programming project using a modern c++ sandbox.
![ingame gif](/ingame.gif)

## Summary
- [aznoqmous/enjminshake](#aznoqmousenjminshake)
  - [Summary](#summary)
  - [Gameplay](#gameplay)
    - [Ingame Controls](#ingame-controls)
  - [Level editor](#level-editor)
    - [Level Editor Controls](#level-editor-controls)
      - [Available tiles](#available-tiles)
- [Required features :](#required-features-)
  - [Main character - Movement and 2 to 4 cell jump](#main-character---movement-and-2-to-4-cell-jump)
  - [Ingame editor - Load/Save to file](#ingame-editor---loadsave-to-file)
  - [Simple enemy AI](#simple-enemy-ai)
  - [Deathray / Laser](#deathray--laser)
  - [Controller support](#controller-support)
  - [Homing missiles (using atan2)](#homing-missiles-using-atan2)
  - [Pet drone](#pet-drone)
  - [Screenshake](#screenshake)
  - [Muzzle fire](#muzzle-fire)
  - [Recoil](#recoil)
- [Additional features :](#additional-features-)
- [Entity](#entity)
  - [Sprite animation (idle, walking, hurt, dead)](#sprite-animation-idle-walking-hurt-dead)
  - [Player](#player)
    - [Coyote jump](#coyote-jump)
    - [Weapon](#weapon)
      - [Backward fire](#backward-fire)
      - [Recoil](#recoil-1)
      - [Shell particles](#shell-particles)
    - [Health UI](#health-ui)
    - [Active weapon UI](#active-weapon-ui)
  - [Foes](#foes)
    - [Display dead foes corpse](#display-dead-foes-corpse)
- [Bullets](#bullets)
  - [Entity collisions](#entity-collisions)
  - [Wall collisions](#wall-collisions)
    - [Time freeze](#time-freeze)
  - [Particle emission](#particle-emission)
- [Camera](#camera)
  - [Follow player (smooth)](#follow-player-smooth)
  - [Move camera according to mouse position](#move-camera-according-to-mouse-position)
  - [Parallax background](#parallax-background)
  - [Vignette](#vignette)
- [Walls](#walls)
  - [Random foliage on top walls](#random-foliage-on-top-walls)
  - [Breakable/Unbreakable walls](#breakableunbreakable-walls)
- [Level editor](#level-editor-1)
  - [Quick `Tab` switch Edit/Player mode](#quick-tab-switch-editplayer-mode)
  - [Grid display](#grid-display)
  - [Mousewheel Zoom](#mousewheel-zoom)
  - [Controls UI text](#controls-ui-text)
- [Credits](#credits)


## Gameplay
The game features 4 weapons (Gun / Rifle / Triple Machinegun / Laser Rifle) and a friendly bot that fires rockets.

### Ingame Controls
- `Q` `D` : Move Left / Right
- `Space` : Jump (maintain pressed to jump higher)
- `Left Click` : Fire equipped weapon
- `Right Click` : Fire bot rocket
- `Mousewheel` : Select weapon
- `T` : Toggle Imgui / Fps counter

## Level editor
![editor gif](/editor.gif)
The game feature an editor that allows you to edit the level as you go !  
Level can be saved and load from/to file 

### Level Editor Controls
- `Tab` : Switch between Game / Editor
- `Z` `Q` `S` `D` : Move camera
- `F` : Focus player
- `C` : Move player to mouse position
- `Mousewheel` : Zoom/Unzoom camera
- `Left Click` : Place selected tile
- `Right Click` : Erase hovered tile 
  
#### Available tiles
- Spawn Position
- Enemy
- Wall (breakable)
- SolidWall (unbreakable)

# Required features :
- [x] Main character - Movement and 2 to 4 cell jump
- [x] Ingame editor - Load/Save to file
- [x] Simple enemy AI
- [x] Deathray / Laser
- [x] Controller support 
- [x] Homing missiles (using atan2)
- [x] Pet drone
- [x] Screenshake
- [x] Muzzle fire
- [x] Recoil
- [x] Readme (its me)

## Main character - Movement and 2 to 4 cell jump  
[Game.cpp:207](/Gppbox/Game.cpp?plain=1#L207)    
`Player` jump is implemented in 2 manners. We first apply a single force on the frame `<Space>` is pressed, then we apply a delta time modified force each frame `<Space>` is pressed, up to 0.5s after the initial force as been applied.  
It allow the player to do short and large jump.

```cpp
// Initial <Space> press
if (player.isCoyoteFloored())
{
  player.jump();
  jumpTime = 0.f;
}

// ...

// Keeping <Space> pressed
jumpTime += dt;
if (jumpTime < jumpDuration) {
  player.dy -= dt * 5.f;
}
```
## Ingame editor - Load/Save to file
[LevelEditor.cpp](/Gppbox/LevelEditor.cpp)

In order to save the level, we use a simple tuple structure `Vector2i position, int tileType`.
`LevelEditor` will save all data inside a `map<Vector2i, int> tiles`, so we only save tiles containing objects.
It also theorically allows us to make "infinite" levels.   
The `Tile` class will be used to hold additional data related to a specific type, and will be set inside `LevelEditor` constructor, in the `tileTypes` array.
An enum `TileType` is used to ease development, and avoid us the pain of working with numbers for our types.

This way, all save/load logic rely on `LevelEditor::tiles`, and we can use `LevelEditor::tileTypes[TileType::Wall]` to get additional tile data (eg: to recreate our `Game::wallSprites` inside `Game::cacheWalls`)

## Simple enemy AI
[Foe.cpp](/Gppbox/Foe.cpp)

The enemy AI is implemented in the `Foe : Entity` subclass.  
Using  `Entity` `collideRight` and `collideLeft` boolean, we simply toggle moving direction when needed in the `Foe::update()` method.
```cpp
if (flipSprite) {
  // moving toward left
  if (!collideLeft) moveLeft(dt);
  else moveRight(dt);
}
else {
  // moving toward right
  if (!collideRight) moveRight(dt);
  else moveLeft(dt);
}
```

## Deathray / Laser
[Weapon.cpp:79](/Gppbox/Weapon.cpp?plain=1#L79)  
`WeaponLaser` must raycast against walls and enemies (`Game::foes`), we initiate a single loop against walls and enemies, returning the nearest one.
```cpp
float nearest = 100.0f; // laser max range

// A isWall method tuned for laser
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

// Collision with foes
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

// Particle or Wall collision ? Spawn particle !
if (hittedFoe || hittedWall) {
  Particle p = Particle("res/bullet-hit.png", fireTarget.x, fireTarget.y);
  p.life = 0.3f;
  p.bhv = [](Particle* p, float dt) {
    p->sprite.setTextureRect(IntRect(floor((1.f - p->life / 0.3f) * 4) * 16, 0, 16, 16));
  };
  game.afterParts.add(p);
}

// If no foe was nearer than a wall, hittedFoe will be empty, else hit the foe !
if (hittedFoe) {
  hittedFoe->takeDamage(1.f);
  fireTarget = hittedFoe->position;
}

```
## Controller support 
[Game.cpp:173](/Gppbox/Game.cpp?plain=1#L173)  
Nothing fancy here, `sf::Joystick` class will do the job for most cases.
A `deadZone` check as been added to handle left joystick usage.

## Homing missiles (using atan2)
[Missile.cpp](/Gppbox/Drone.cpp)  
Homing missiles can be fired by the pet drone.  
I added a `Interp::lerpAngle` method to handle missile panning, to avoid the 0-360 issue. This method uses a shortest angle check before computing the resulting lerp.  

## Pet drone
[Drone.cpp](/Gppbox/Drone.cpp)  
To implement the pet drone (`Drone : Entity` subclass), I added a `isFlying` boolean to `Entity` to prevent applying gravity.  
The drone will move toward a moving point close to the player, to mimic a flying animation.  
If too far from the player, the drone will teleport on said point, with a scale animation, fancy.  
The drone can also fire homing missiles !

## Screenshake
[Game.cpp:308](/Gppbox/Game.cpp?plain=1#L308)  
Screenshake is implemented in a non-stacking manner and rely on 2 variables :  
`Vector2f screenShakeOffset` represents the current screenshake position  
`float screenShakeTime` represents the time when last screenshake was emitted  
`float screenShakePower` represents the current screenshake power, decreases each frame until reaching 0  
Each frame, we set the game view center to `cameraPosition` combined with a simple `sin(screenShakeOffset)` and `cos(screenShakeOffset)` on each axis, multiplied by `screenShakePower`.
```cpp
// some dark magic here
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
```
## Muzzle fire
[Weapon.cpp:15](/Gppbox/Weapon.cpp?plain=1#L15)  
Muzzle fire is a simple sprite, drawed on the frame player fires, and for 1/10sc after that.

## Recoil
[Weapon.cpp:64](/Gppbox/Weapon.cpp?plain=1#L64)  
Recoil is simulated by :
Applying a horizontal force to `Player::dx`  
Applying a horizontal offset to `Weapon::offset`  

# Additional features :

# Entity

## Sprite animation (idle, walking, hurt, dead)
Sprite animations are made using `sf::Sprite.setTextureRect()`  
Directly integrated inside `Entity` base class, it rely on the `animationLength` to infer the animation frames count.

## Player
The `Player` class allows to handle specific calls needed for the player. A distinct reference is made inside `Game` between `Player` and `Foe` for simplicity. 

### Coyote jump
Coyote jump rely on setting `coyoteTime` when entity `isFloored` state change.

### Weapon
The `Weapon` class holds different parameters to facilitate further weapon creation. Weapon visuals rely on a single spritesheet.

#### Backward fire
To allow player to move to the opposite way when firing, we set a `Game::isFiring` boolean. The boolean will be checked when flipping player sprite/facing direction. 

#### Recoil

#### Shell particles 
To implement shell particles, I added a `collideWithWalls` boolean which is used in the `Particle::update()` method

### Health UI

### Active weapon UI  

## Foes
The `Foe` class allows to manage enemies behavior. They are activated when player is close enough, reducing compute time and allows to place enemy strategically in level design. 

### Display dead foes corpse 
Dead foes are pushed to `Game::deadFoes` array to draw them beside living foes.

# Bullets
The `Bullet` class is a simplified version of the `Entity` class.
Bullets instance are placed inside `Game::bullets` array, used for rendering and collision detection. 

## Entity collisions
Bullets collisions with enemies is a simple distance test, priorizing compute speed.

## Wall collisions 
Bullets wall collisions relly on the `Game::isWall` method.

### Time freeze
Every `draw` methods are bound to `deltaTime`.
To implement time freeze, `app` deltaTime is multiplied by `Game::timeSpeed`.  
As we only need to add time freeze spikes, `timeSpeed` is lerp toward 0 each frame using a non-modified `dt`.  
This way, setting `Game::timeSpeed` to 0 emulates a time spike.

## Particle emission 
To avoid bloating the project with classes, particles behavior will be implemented in ``Particle`` `bhv` callback, per instance.

# Camera

## Follow player (smooth)
`Game::cameraPosition` is lerp toward player position each frame.

## Move camera according to mouse position 
`Game::cameraPosition` is lerp toward a point between player position and the current mouse position.

## Parallax background 
To add depth to our scene, a 2 layer parallax background is implemented (see `ParallaxLayer` class).

## Vignette 
A simple vignette sprite is drawn on top of the scene to add flavour to the game.

# Walls

## Random foliage on top walls
In the `Game::cacheWalls()` method, we generate a grass sprite each time a wall doesnt have a top neighbouring wall.
Those sprites are then drawn to the screen directly after drawing walls.

## Breakable/Unbreakable walls 
To keep `Game` from having 2 `walls` arrays, the breakable/unbreakable wall info is directly checked against `LevelEditor::tiles`.

# Level editor

## Quick `Tab` switch Edit/Player mode 
To easily swap between those modes, and split our code accordingly, a `GameMode` enum is created to indicate the current game state (`EditMode`/`PlayMode`).

## Grid display
To allow players to visualize the game grid, we draw `RectangleShape` on the screen. Using `RectangleShape` allows us to draw the hovered tile eaasily, by changing `OutlineColor` and `FillColor`.  

## Mousewheel Zoom
To facilitate level editing, the mouse wheel will be bound to zooming in `EditMode`. Unforunately, `SMFL::View` doesnt provide a `setZoom` method, and we can only modifiy current zoom based on its value.  
We then can track of the current zoom value inside `Game::cameraZoom`, each time we call `zoom()`. It allows us to zoom back to 1:1 when entering `PlayMode`, and to clamp the min and max zoom value.
```cpp
// Setting the zoom value :
float zoomValue = 1.f - ev.mouseWheel.delta / 10.f
cameraZoom *= zoomValue;
mainCamera.zoom(zoomValue);

// Zooming back to 1:1
mainCamera.zoom(1.f / cameraZoom);
cameraZoom = 1.f;
```
## Controls UI text

# Credits
**Nuclear Throne** sprites from [Spriters Resources](https://www.spriters-resource.com/pc_computer/nuclearthrone/)

**Nuclear Throne Font** from [yellowafterlife](https://yellowafterlife.itch.io/cc-yal-7w7-block) 

![dead](/dead.gif)
____
&copy; LICENCE MIT
