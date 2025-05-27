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
Linux (via APT)
bash
Copy
Edit
sudo apt install libglfw3-dev cmake
📦 Build Instructions
Clone the repo:

bash
Copy
Edit
git clone https://github.com/AntonSplavnik/miniRT.git
cd miniRT
Build MLX42 and the project:

bash
Copy
Edit
make
This will automatically check for and clone MLX42 (if missing), compile it using CMake, and then build miniRT.

🚀 Running the Program
Basic Run
bash
Copy
Edit
./minirt path/to/scene.rt
Save Screenshot
bash
Copy
Edit
./minirt path/to/scene.rt --save
This saves a .bmp screenshot of the rendered frame using screenshot.c.

📂 Scene Files
Scene files must follow the .rt format expected by the parser. A minimal example:

css
Copy
Edit
A 0.2 255,255,255
C 0,0,20 0,0,-1 70
L 10,10,10 0.8 255,255,255
sp 0,0,0 2 255,0,0
pl 0,-1,0 0,1,0 0,255,0
You can find more examples in the scenes/ directory.

🚧 Status
Currently implementing object transformations and optimizing render loop performance.

yaml
Copy
Edit

---

Let me know if you'd like a section about **writing `.rt` files**, **keyboard controls**, or **debugging tips** added next!






