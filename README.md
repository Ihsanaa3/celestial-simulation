# Celestial Simulation

A small C++/OpenGL playground for experimenting with orbital motion, n-body gravity, collisions, and rendered motion trails. The repository contains several standalone simulations built on a shared GLFW/GLAD/GLM rendering layer.

## Features

- 2D n-body simulation with up to 1,000 randomly placed bodies.
- Collision and boundary-bounce experiment with colored balls.
- 3D Sun–Earth–Moon rotational/orbital demonstration.
- Dynamic orbit trails rendered as OpenGL line strips.
- Reusable mesh, shader, camera, VAO, VBO, EBO, texture, and random-number helpers.

## Repository layout

```text
SolarSystem/
  nBody2D.cpp          2D gravitational n-body simulation
  collidingBalls.cpp   2D ball collision and boundary simulation
  rotationalOrbit.cpp  3D Sun–Earth–Moon orbital demonstration
  trailsTest.cpp       Mesh, grid, sphere, rotation, and trail test scene

include/                C++ headers for rendering and simulation helpers
hsource/                Implementations, including GLAD and OpenGL wrappers
```

## Technology

- C++ with a small C component (`hsource/glad.c`)
- OpenGL 3.3 Core Profile
- GLFW for window creation, input, and event processing
- GLAD for loading OpenGL functions
- GLM for vectors, matrices, transformations, and camera math

## How the simulations work

Each demo has its own `main()` function and must be built as a separate executable. The program initializes an OpenGL 3.3 context through `et::initialiseGLFW`, creates meshes and shaders, then runs a frame loop that updates simulation state, renders objects and trails, swaps buffers, and exits when **Esc** is pressed.

`SolarSystem/nBody2D.cpp` models gravitational acceleration by summing the influence of every other `Ball`, integrates velocity and position at a fixed timestep, reflects bodies at the edges of the view, and appends positions to `OrbitTrail` instances. `SolarSystem/rotationalOrbit.cpp` instead builds Earth and Moon model matrices from orbit, translation, and self-rotation transforms, while drawing lit meshes and trails with a movable camera.

## Requirements

Install or provide:

- A C++17-compatible compiler
- GLFW 3
- GLM
- OpenGL development libraries and headers
- GLAD sources/headers matching the included `hsource/glad.c`
- The shader resource files expected by the selected demo

The repository currently does not include a CMake project, Makefile, shader resources, or platform-specific dependency setup. Some demos also reference resources using either the `res` directory calculated by `include/relativeResPath.h` or the absolute `C:/OGL/res/...` paths in the source, so those paths may need to be adjusted locally before building.

## Building

Because there is no build system yet, compile one demo at a time and link the shared implementation files. The exact GLFW and OpenGL linker flags vary by platform.

For Linux, an illustrative command for the relative-resource 2D n-body demo is:

```bash
g++ -std=c++17 -Iinclude \
  SolarSystem/nBody2D.cpp \
  hsource/EBO.cpp hsource/Mesh.cpp hsource/OrbitTrail.cpp \
  hsource/VAO.cpp hsource/VBO.cpp hsource/camera.cpp \
  hsource/extraFunctions.cpp hsource/glad.c \
  hsource/meshDrawTools.cpp hsource/rotation.cpp \
  hsource/shaderClass.cpp hsource/stb.cpp hsource/texture.cpp \
  -lglfw -ldl -lGL -o nbody2d
```

Run it from a working directory where the resource lookup in `relativeResPath.h` resolves to your shader files:

```bash
./nbody2d
```

To build another demo, replace `SolarSystem/nBody2D.cpp` with exactly one of:

```text
SolarSystem/collidingBalls.cpp
SolarSystem/rotationalOrbit.cpp
SolarSystem/trailsTest.cpp
```

Do not compile multiple demo source files together because each defines its own `main()` function.

## Controls

- **Esc** — close the simulation window.
- The 3D orbital demo also passes input through the `Camera` class for camera interaction.

## Notes and limitations

- The simulations are educational experiments rather than physically calibrated astronomical models; constants, radii, and orbital speeds are intentionally simplified.
- `nBody2D.cpp` performs an all-pairs force calculation, so its update cost grows quadratically with the number of bodies.
- `OrbitTrail` streams trail positions to a dynamic OpenGL buffer and can use either a capped trail length or an unlimited trail.
- A future build configuration and checked-in shader/resource directory would make the demos easier to reproduce across platforms.
