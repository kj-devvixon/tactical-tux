# Tactical Tux v2 - Quick Start Guide

## 🆕 What's New?

### 1. Physics & Collision System
- **Gravity**: You now fall and can jump (Space)
- **Collision Detection**: Can't walk through walls anymore!
- **Walking**: No more flying - you're grounded like a real FPS

### 2. Crosshair
- White crosshair always visible in center of screen
- Helps with aiming

### 3. Shooting System (Raycast)
- **Left Click** to shoot
- Raycast detects what you hit
- Console shows hit coordinates
- Fire rate limited to prevent spam

### 4. Map Editor
- **Press E** to toggle editor mode
- **P** - Place object 3 meters in front of camera
- **L** - Delete last placed object
- **S** - Save current map to `map.txt`
- **O** - Load map from `map.txt`

## 🎮 How to Play

1. Build and run:
```bash
./build.sh
./build/TacticalTux
```

2. Move around with WASD
3. Look with mouse
4. Jump with Space
5. Shoot with Left Click

## 🛠️ How to Build Maps

1. Start the game
2. Press **E** to enter Editor Mode (mouse cursor appears)
3. Walk to where you want to place objects
4. Look at the spot and press **P** to place a cube
5. Press **L** to undo last placement
6. Press **S** to save your map
7. Press **E** again to exit editor and test your map!

## 📁 Map File Format

Maps are saved in `map.txt` as plain text:
```
<number_of_objects>
<pos_x> <pos_y> <pos_z> <scale_x> <scale_y> <scale_z> <color_r> <color_g> <color_b> <is_wall>
...
```

You can edit this file manually if you want!

## 🐛 Tips & Tricks

- Objects placed with P are automatically collidable
- Save often! (Press S in editor mode)
- Default map loads if map.txt doesn't exist
- Console shows useful debug info
- Adjust player speed in `Player.h` (moveSpeed variable)
- Adjust gravity in `Physics.h` (GRAVITY constant)

## ⚙️ Customization

### Change Player Movement Speed
Edit `src/Player.cpp`, line ~7:
```cpp
moveSpeed(5.0f)  // Change this number
```

### Change Gravity
Edit `src/Physics.cpp`, line ~4:
```cpp
const float Physics::GRAVITY = -20.0f;  // More negative = stronger
```

### Change Jump Height
Edit `src/Player.cpp`, line ~9:
```cpp
jumpForce(8.0f)  // Higher = jump higher
```

## 🔧 Known Limitations

- No texture support yet (just colors)
- Simple box collisions only
- No enemies or AI
- No health/damage system
- Editor is keyboard-only (no GUI widgets)

## 🚀 Next Steps

Want to add more features? Check README.md for ideas like:
- Advanced GUI editor (ImGui)
- Enemies and AI
- Multiple weapons
- Health system
- Textures and lighting

Enjoy building! 🐧
