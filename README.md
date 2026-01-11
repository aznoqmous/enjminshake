# aznoqmous/enjminshake

Enjmin CPP gameplay programming project using a modern c++ sandbox.

## Features
![ingame gif](/ingame.gif)
The game features 2 weapons (Gun / Laser Rifle) and a friendly bot that fires rockets.

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
  
### Available tiles : 
- Spawn Position
- Enemy
- Wall

## Required features :
- [x] Main character - Movement and 2 to 4 cell jump
- [x] Ingame editor
- [x] Ingame editor - Load/Save to file
- [x] Simple enemy AI
- [ ] Bullets - Bresenham algorithm
- [x] Controller support 
- [x] Homing missiles (using atan2)
- [x] Pet drone
- [x] Screenshake
- [x] Muzzle fire
- [x] Recoil
- [ ] Readme

## Additional features :
- [x] **Entity**
  - [x] Sprite animation (idle, walking, hurt, dead)
  - [x] **Player**
    - [x] Coyote jump
    - [x] Weapons
      - [x] Backward fire
      - [x] Recoil
  - [x] **Foes**
- [x] **Bullets**
  - [x] Entity collisions
    - [x] Time freeze
    - [x] Particle emission 
  - [x] Wall collisions 
- [x] **Camera**
  - [X] Follow player (smooth)
  - [x] Screenshake
  - [x] Parallax background 
  - [x] Vignette 
- [x] **Walls**
  - [x] New skin
  - [x] Random foliage on top walls
  - [x] Destructable walls 
  - [ ] Noise based wall sprites
- [x] **Level editor**
  - [x] Grid display
  - [x] Quick `Tab` switch Edit/Player mode 
  - [x] Wall placement
  - [x] Enemy placement
  - [x] Player placement
  - [x] Mousewheel Zoom
  - [ ] Controls UI

## Credits
**Nuclear Throne** sprites from [Spriters Resources](https://www.spriters-resource.com/pc_computer/nuclearthrone/)

**Nuclear Throne Font** from [yellowafterlife](https://yellowafterlife.itch.io/cc-yal-7w7-block) 

![dead](/dead.gif)
____
&copy; LICENCE MIT
