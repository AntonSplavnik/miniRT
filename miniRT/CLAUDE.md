# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Standard Build
- `make` - Build the regular miniRT executable
- `make bonus` - Build the bonus version (minirt_bonus) with additional features
- `make clean` - Remove object files and build artifacts
- `make fclean` - Full clean including executables and static libraries
- `make re` - Clean and rebuild regular version
- `make bonus_re` - Clean and rebuild bonus version

### Running
- `./minirt scenes/mandatory/basic_shapes.rt` - Run regular version with scene file
- `./minirt_bonus scenes/bonus/refractivety.rt` - Run bonus version with advanced scene

## High-Level Architecture

### Core Components

**Scene Management (`t_scene`)**
- Central structure containing all scene data: camera, lights, objects, graphics settings
- Supports both regular and bonus features through conditional compilation
- Graphics settings control rendering quality vs performance (GI, SSAA, shadows, reflections)

**Rendering Pipeline**
- **Multi-threaded work-stealing renderer** (`rendering/render.c`) - Uses 8 threads with dynamic load balancing
- **BVH acceleration** (`rendering/BVH/`) - Scene-wide and per-mesh bounding volume hierarchies for fast ray-object intersection
- **Global Illumination** (`rendering/color/global_illumination.c`) - Monte Carlo path tracing with Russian Roulette termination
- **Material system** - Supports PBR materials, refraction, reflection, emissive surfaces, textures, bump mapping

**Ray Tracing Core**
- **Intersection testing** (`objects/`) - Primitive-specific ray intersection (sphere, plane, cylinder, cube, triangle, mesh)
- **Ray management** (`rendering/trace_ray/`) - Primary ray casting, reflection/refraction ray spawning, GI ray sampling
- **Color processing** (`rendering/color/`) - HDR processing, tone mapping, gamma correction

### Advanced Features (Bonus)

**Global Illumination**
- Monte Carlo sampling with configurable depth and samples per bounce
- Russian Roulette termination to prevent infinite recursion
- Energy-based ray culling with throughput tracking
- Emissive materials contribute to indirect lighting

**Area Lights** 
- Soft shadows through Monte Carlo sampling of light surface
- Configurable size and sample count for quality vs performance
- Works alongside point lights and GI

**Complex Geometry**
- OBJ mesh loading with triangle optimization
- Per-mesh BVH for fast intersection testing
- Support for cones, cubes, and other primitives

**Material Properties**
- Physically-based reflection/refraction with Fresnel equations
- Texture mapping (PNG) with UV coordinates
- Bump mapping for surface detail
- Emissive materials for light sources
- Checkerboard procedural textures

### Performance Optimizations

**Multithreading**
- Work-stealing thread pool prevents load imbalance
- Lock-free work queue for minimal contention
- Progress reporting with real-time timing

**BVH Acceleration**
- Scene-level BVH for object culling
- Mesh-level BVH for triangle culling
- AABB-based spatial partitioning

**Memory Management**
- Precomputed triangle transformations for meshes
- Efficient linked list structures for scene objects
- Proper cleanup functions to prevent leaks

### Scene File Format (.rt)

Supports both basic primitives and advanced features:
- `A` - Ambient lighting
- `C` - Camera position/orientation/FOV  
- `L` - Point lights
- `AL` - Area lights (bonus)
- `sp/pl/cy/cu/tr` - Geometry primitives
- Material properties in `{key:value}` format for advanced materials

### Key Configuration

**Global Illumination tuning** (`init.c`):
- `gi.max_depth` - Ray bounce limit (quality vs performance)
- `gi.samples_per_bounce` - Monte Carlo samples (noise vs speed)

**Graphics settings** (`t_graphic_settings`):
- Enable/disable individual features (GI, reflections, shadows, etc.)
- SSAA anti-aliasing sample count
- Resolution scaling factor

### Development Notes

- **42 Norminette compliance** - Follows 42 school coding standards
- **MLX42 graphics library** - Cross-platform OpenGL wrapper
- **Libft dependency** - Custom C standard library implementation
- **Thread-safe design** - All shared data structures use proper synchronization
- **HDR rendering pipeline** - Linear color space with tone mapping for display

The codebase is optimized for both educational purposes (clear separation of concerns) and performance (multithreading, BVH acceleration, work-stealing).

## 42 Norminette Rules

Follow these strict norms when modifying code:

**Language Standards:**
- Adhere to the 42 Norminette and C89 standard
- Max 25 lines per function
- Use only while loops (no for, do-while, or recursion)
- No ternary operators
- Max 80 characters per line
- Max 5 functions per `.c` file
- Max 5 variable declarations per function
- Variable declarations must be placed only at the top of each function
- Max 4 arguments per function

**Memory & Functions:**
- No banned functions (e.g. printf); use only allowed standard and project functions
- malloc must be used correctly (with null-checks and no memory leaks)
- No stdlib.h extras beyond what's explicitly allowed
- Global variables are restricted and discouraged

**Code Organization:**
- `typedef` declarations only in header (`.h`) files
- Comments must appear **only outside** of functions
- Code must be modular, clean, and follow a separation of concerns
- It must compile and run on both macOS (M1) and Linux

**Allowed Libraries:**
- MLX42 (for display)
- pthreads (for multithreading)
- libft (custom C library)