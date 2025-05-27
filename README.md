# MiniRT

**MiniRT** is a lightweight ray tracing engine written in pure C. Built as part of the 42 school curriculum, the project focuses on rendering 3D scenes by simulating the path of rays through a virtual environment.

## Screenshots

### Shadows and Reflections
![Scene 1](sreenshots/Screenshot%202025-05-26%20at%2020.29.59.png)

## Features

- Ray-object intersection with **spheres**, **planes**, **cylinders**, **triangles**, **cones**, and **meshes**
- Support for **shadows**, **reflections**, and **multiple light sources**
- **Scene parsing** from `.rt` files
- Basic **camera movement** and field of view
- **Phong lighting model** for realistic shading
- Uses **MLX42**, a modern OpenGL-based graphics library

## What I Learned

- Ray tracing fundamentals and vector math
- Optimizing render performance in C
- Parsing custom scene files and building a minimal 3D renderer
- Laying the groundwork for BVH acceleration structures

## Technologies

- Language: **C**
- Graphics: **MLX42** (based on GLFW and OpenGL)
- Platform: Unix/Linux, macOS (including M1)
- Build system: `Makefile`, uses `CMake` for MLX42

---

## 🛠️ Getting Started

### ✅ Requirements

#### macOS (via Homebrew)

```bash
brew install glfw cmake
```

#### Linux (via APT)
``` bash
sudo apt install libglfw3-dev cmake
```

## 🔧 Build Instructions

### 1. Clone the repository
``` bash
git clone https://github.com/AntonSplavnik/miniRT.git
cd miniRT
```

### 2. Build MLX42 and the project
```bash
make
```
This will automatically:

 - Check if MLX42 exists (and clone it if not)
 - Build MLX42 using cmake
 - Compile MiniRT using the system's gcc

## 🚀 Running the Program

### To render a scene:
```bash 
./minirt path/to/scene.rt
```

## 📁 Scene File Format (.rt)

#### A minimal example:
```c
A 0.2 255,255,255
C 0,0,20 0,0,-1 70
L 10,10,10 0.8 255,255,255
sp 0,0,0 2 255,0,0
pl 0,-1,0 0,1,0 0,255,0
```

- A: Ambient light
- C: Camera position, direction, FOV
- L: Light source position, brightness, color
- sp: Sphere
- pl: Plane

#### More examples can be found in the scenes/ directory.


🚧 Status
Currently implementing object transformations and optimizing render loop performance.

