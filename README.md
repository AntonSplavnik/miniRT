# MiniRT

**MiniRT** is a lightweight ray tracing engine written in pure C. Built as part of the 42 school curriculum, the project focuses on rendering 3D scenes by simulating the path of rays through a virtual environment.

## Screenshots

### Shadows and Reflections
![Scene 1](sreenshots/Screenshot%202025-05-26%20at%2020.29.59.png)
<!-- Or rename to something like `shadows_reflections.png` and use:
![Scene 1](sreenshots/shadows_reflections.png)
-->

## Features

- Ray-object intersection with **spheres**, **planes**, and **cylinders**
- Support for **shadows**, **reflections**, and **multiple light sources**
- **Scene parsing** from `.rt` files
- Basic **camera movement** and field of view
- **Phong lighting model** for realistic shading
- Written from scratch without external graphics libraries
- Uses **MLX42**, a modern graphics library built on top of GLFW

## What I Learned

- Ray tracing fundamentals and vector math
- Optimizing render performance in C
- Parsing custom scene files and building a minimal 3D renderer
- Laying the groundwork for BVH acceleration structures

## Technologies

- Language: **C**
- Graphics: **MLX42** (based on GLFW)
- Platform: Unix/Linux, macOS (M1 supported)
- Build system: `Makefile`, uses `CMake` for MLX42

## 🛠️ Getting Started

### Requirements

- **OS:** Unix/Linux or macOS (M1 supported)
- **Compiler:** `gcc` (with C99 support)
- **MLX42** installed
- **CMake** and **GLFW** (required by MLX42)
- **Make**

#### Linux

```bash
sudo apt install libglfw3-dev cmake
