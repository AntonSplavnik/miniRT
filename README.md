# MiniRT

**MiniRT** is a lightweight ray tracing engine written in pure C. Built as part of the 42 school curriculum, the project focuses on rendering 3D scenes by simulating the path of rays through a virtual environment.

## Screenshots

### Shadows and Reflections
![Scene 1](sreenshots/sreenshots/Screenshot%202025-05-26%20at%2020.29.59.png)

## Features
- Ray-object intersection with **spheres**, **planes**, and **cylinders**
- Support for **shadows**, **reflections**, and **multiple light sources**
- **Scene parsing** from `.rt` files
- Basic **camera movement** and field of view
- **Phong lighting model** for realistic shading
- Written from scratch without external graphics libraries

## What I Learned
- Ray tracing fundamentals and vector math
- Optimizing render performance in C
- Parsing custom scene files and building a minimal 3D renderer
- Laying the groundwork for BVH acceleration structures

## Technologies
- Language: **C**
- Graphics: **MiniLibX**
- Platform: Unix/Linux (macOS with M1 supported)
- Build system: `Makefile`

## Screenshots (optional)

## 🚧 Status
Currently implementing object transformations and optimizing render loop performance.
