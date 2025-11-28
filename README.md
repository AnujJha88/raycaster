# Raycaster

A simple 3D raycasting engine built with C++ and SDL2, inspired by classic games like Wolfenstein 3D.

## Overview

This project implements a raycasting renderer that creates a pseudo-3D perspective from a 2D map. The engine uses the DDA (Digital Differential Analysis) algorithm to cast rays and render walls with different colors and shading based on orientation.

## Features

- Real-time 3D rendering using raycasting
- Player movement (WASD controls)
- Camera rotation (arrow keys)
- Collision detection
- Multiple wall types with different colors
- Dynamic lighting (walls appear darker on certain sides)
- Interactive map manipulation (spacebar)

## Prerequisites

- C++ compiler with C++17 support (g++)
- SDL2 library
- Make

### Installing SDL2

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libsdl2-dev
```

**macOS:**
```bash
brew install sdl2
```

**Windows:**
Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/download-2.0.php)

## Building

Clone the repository and build using Make:

```bash
make
```

This will compile the source files and create the `raycaster` executable.

To clean build artifacts:

```bash
make clean
```

## Running

After building, run the executable:

```bash
./raycaster
```

## Controls

- **W** - Move forward
- **S** - Move backward
- **A** - Strafe left
- **D** - Strafe right
- **Left Arrow** - Turn left
- **Right Arrow** - Turn right
- **Spacebar** - Interact with map
- **Close window** - Exit

## Project Structure

```
.
├── include/          # Header files
│   ├── Engine.h      # Main rendering engine
│   ├── Map.h         # Map data structure
│   └── Player.h      # Player movement and camera
├── src/              # Source files
│   ├── Engine.cpp
│   ├── Map.cpp
│   ├── Player.cpp
│   └── main.cpp
├── obj/              # Compiled object files
├── Makefile          # Build configuration
└── raycaster         # Executable (after build)
```

## How It Works

The raycaster works by:

1. Casting a ray for each vertical line on the screen
2. Using DDA algorithm to trace rays through the 2D grid
3. Calculating wall distances to determine wall height
4. Rendering vertical lines with heights based on distance
5. Applying color and shading based on wall type and orientation

The map is a 24x24 grid where each cell can be empty (0) or contain different wall types (1, 2, 3).

## License

This project is open source and available for educational purposes.
