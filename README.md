# miniRT

A ray tracing engine written in pure C, rendering photorealistic 3D scenes with reflections, refractions, soft shadows, and mesh support. Built as part of the 42 School curriculum.

<!-- TODO: Add hero render here -->
<!-- ![miniRT Render](screenshots/hero.png) -->

## Renders

| Reflections & Checkerboard | Colored Lights |
|:-:|:-:|
| ![Reflections](sreenshots/Screenshot%202025-05-26%20at%2020.32.49.png) | ![Colored Lights](sreenshots/Screenshot%202025-05-26%20at%2020.30.35.png) |

| Materials & Refraction | Checker Room |
|:-:|:-:|
| ![Materials](sreenshots/Screenshot%202025-05-26%20at%2020.33.39.png) | ![Checker Room](sreenshots/Screenshot%202025-05-26%20at%2020.29.59.png) |

<!-- TODO: Add renders for soft shadows, mesh models (dragon, Beetle, skull), UI panel -->

## Features

**Rendering**
- Recursive ray tracing with configurable bounce depth
- Phong lighting model with ambient, diffuse, and specular components
- Soft shadows via area light Monte Carlo sampling
- Hard shadow rays with distance attenuation
- Supersampling anti-aliasing (SSAA) with jittered sampling
- HDR tone mapping (Reinhard operator) and sRGB gamma correction

**Optics**
- Fresnel equations for angle-dependent reflectivity
- Snell's law refraction with configurable refractive indices
- Total internal reflection
- Fresnel blending between reflected and refracted rays

**Materials**
- Per-object reflectivity, specular intensity, and shininess
- PNG texture mapping with bilinear filtering
- Bump mapping via PPM height maps with tangent-space normal perturbation
- Procedural checkerboard patterns with configurable size and colors
- Emissive materials for light-emitting surfaces

**Primitives**
- Sphere, plane, cylinder, cone, cube, triangle
- Triangle meshes with OBJ file loading
- Per-vertex normal interpolation (smooth shading on meshes)

**Performance**
- Bounding Volume Hierarchy (BVH) for scene and per-mesh intersection culling
- Multithreaded rendering with work-stealing thread pool
- Compiled with `-O3 -ffast-math -march=native -flto`

**In-Game UI**
- Custom-drawn control panel overlay with animated open/close
- Checkboxes to toggle rendering features at runtime: hard shadows, reflections, specular highlights, refraction
- Status bar displaying camera position and render info
- Toggle button to show/hide the panel without interrupting the scene

## Building

### Prerequisites

**macOS:**
```bash
brew install glfw cmake
```

**Linux:**
```bash
sudo apt install libglfw3-dev cmake
```

### Compile

```bash
git clone https://github.com/AntonSplavnik/miniRT.git
cd miniRT
make
```

The build automatically fetches and compiles MLX42 (OpenGL/GLFW graphics library) and libft.

## Usage

```bash
./minirt path/to/scene.rt
```

### Controls

| Key | Action |
|-----|--------|
| W / A / S / D | Move camera |
| Arrow keys | Rotate camera |
| Q / E | Move up / down |
| Mouse drag | Look around |
| ESC | Quit |

## Scene Format

Scenes are defined in `.rt` files with a declarative syntax.

**Basic scene:**
```
A  0.2                          255,255,255
C  0,0,20       0,0,-1    70
L  10,10,10     0.8             255,255,255

sp  0,0,0       2               255,0,0
pl  0,-1,0      0,1,0           0,255,0
cy  3,0,0       0,1,0    1  4   0,0,255
```

**With materials:**
```
sp  -6,1,-3  3  255,50,50    {reflectivity:0.9 specular:1.0 shininess:128}
sp  0,0,0    7  255,255,255  {texture:scenes/textures/earth.png bumpmap:scenes/bumpmaps/earth.ppm}
pl  0,-2,0   0,1,0  200,200,200  {checker_color:30,30,30 checker_size:4 reflectivity:0.2}
sp  2,1,0    2  255,255,255  {reflectivity:0.1 transparency:1.5}
```

### Scene Elements

| ID | Element | Parameters |
|----|---------|------------|
| `A` | Ambient light | intensity, color |
| `C` | Camera | position, direction, FOV |
| `L` | Point light | position, intensity, color |
| `AL` | Area light | position, radius, intensity, color |

### Primitives

| ID | Shape | Parameters |
|----|-------|------------|
| `sp` | Sphere | center, radius, color |
| `pl` | Plane | point, normal, color |
| `cy` | Cylinder | center, axis, radius, height, color |
| `co` | Cone | apex, axis, height, radius, color |
| `cu` | Cube | center, side length, color |
| `tr` | Triangle | 3 vertices, color |
| `me` | Mesh | OBJ file path, color |

### Material Properties

All primitives accept optional `{key:value}` material overrides:

| Property | Description |
|----------|-------------|
| `reflectivity` | Mirror reflectivity (0.0 - 1.0) |
| `specular` | Specular highlight intensity |
| `shininess` | Specular exponent (low = plastic, high = metal) |
| `transparency` | Refractive index (1.5 = glass, 2.42 = diamond) |
| `texture` | Path to PNG texture file |
| `bumpmap` | Path to PPM height map |
| `checker_color` | Second color for checkerboard pattern |
| `checker_size` | Scale of the checkerboard grid |

Example scenes are available in the `scenes/` directory.

## Architecture

```
miniRT/
├── srcs/
│   ├── rendering/          Ray tracing core, lighting, color processing
│   ├── objects/            Per-primitive intersection tests
│   ├── parser/             Scene file tokenizer and validator (27 files)
│   ├── camera/             Projection and basis vector computation
│   ├── controls/           Keyboard and mouse input
│   ├── ui/                 Draggable control panel with toggles
│   ├── vector_utils/       3D vector math library
│   └── utils/              Color math, memory, BVH construction
├── includes/               Core types, API headers
├── libft/                  Custom C standard library
├── scenes/                 Scene files, textures, bump maps, OBJ models
└── MLX42/                  Graphics library (OpenGL/GLFW)
```

### Rendering Pipeline

```
Primary ray ──► BVH traversal ──► Closest hit
                                      │
                          ┌───────────┼───────────┐
                          ▼           ▼           ▼
                     Refraction   Reflection   Direct lighting
                    (Fresnel +    (recursive   (Phong + shadow
                     Snell's law)  bounce)      rays)
                          │           │           │
                          └───────────┼───────────┘
                                      ▼
                              Color accumulation
                              Tone mapping (Reinhard)
                              Gamma correction (sRGB 2.2)
                                      ▼
                                    Pixel
```

## Technical Notes

- Written in C, compiled with `-Wall -Wextra -Werror`
- Rendering at 1920x1080 by default
- Double-precision floating point throughout
- POSIX threads with work-stealing for load balancing
- Conforms to 42's norminette standard (25 lines/function, 4 parameters max, no `for` loops)

## License

This project is open source under the [MIT License](LICENSE).
