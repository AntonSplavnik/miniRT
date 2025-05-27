# MiniRT

**MiniRT** is a lightweight, efficient ray tracing engine developed in pure C. This project was created as part of the curriculum at 42 School and is designed to render realistic 3D scenes by simulating the path of rays through virtual environments.

---

## 🌟 Features

* **Ray-Object Intersection**: Supports **spheres**, **planes**, **cylinders**, **triangles**, **cones**, and complex **meshes**.
* **Advanced Lighting**: Implements **shadows**, **reflections**, and **multiple light sources**.
* **Scene Parsing**: Custom `.rt` scene file parsing to define scene elements easily.
* **Camera Controls**: Basic interactive **camera movement** and adjustable field of view.
* **Realistic Shading**: Utilizes the **Phong lighting model** for natural-looking rendering.
* **Graphics Engine**: Built on **MLX42**, a modern graphics library leveraging OpenGL and GLFW.

---

## 📸 Screenshots

### Shadows and Reflections

![Shadows and Reflections](screenshots/Screenshot%202025-05-26%20at%2020.29.59.png)

---

## 💡 What I Learned

* **Ray tracing fundamentals** including vector math and ray-object intersections.
* **Performance optimization** techniques for rendering in C.
* Efficiently **parsing and managing custom scene files**.
* Preparing the foundation for implementing **BVH acceleration structures**.

---

## 🛠️ Technologies

* **Programming Language**: C
* **Graphics Library**: MLX42 (built on GLFW and OpenGL)
* **Supported Platforms**: Unix/Linux, macOS (including Apple M1)
* **Build Tools**: `Makefile`, integrating `CMake` for MLX42

---

## 🚀 Quick Start

### ✅ Installation Requirements

#### macOS (with Homebrew)

```bash
brew install glfw cmake
```

#### Linux (with APT)

```bash
sudo apt install libglfw3-dev cmake
```

---

## 🔧 Building the Project

### 1. Clone the Repository

```bash
git clone https://github.com/AntonSplavnik/miniRT.git
cd miniRT
```

### 2. Compile and Build

```bash
make
```

This command automatically:

* Checks and clones MLX42 if it’s missing.
* Builds MLX42 via `cmake`.
* Compiles MiniRT using the system's default compiler (`gcc`).

---

## ▶️ Running MiniRT

To render a scene, run:

```bash
./minirt path/to/scene.rt
```

---

## 📁 Scene File Format (.rt)

#### Minimal Example

```c
A 0.2 255,255,255
C 0,0,20 0,0,-1 70
L 10,10,10 0.8 255,255,255
sp 0,0,0 2 255,0,0
pl 0,-1,0 0,1,0 0,255,0
```

* **A**: Ambient light (intensity, RGB color)
* **C**: Camera (position, orientation, field of view)
* **L**: Light source (position, intensity, RGB color)
* **sp**: Sphere (position, radius, RGB color)
* **pl**: Plane (position, normal vector, RGB color)

Additional examples are available in the `scenes/` directory.

---

## 🚧 Project Status

Currently working on implementing object transformations and further optimizing the render loop for better performance.
