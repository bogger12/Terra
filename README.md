# Terra Engine (🚧 In Progress)
#### Powered by OpenGL


##### The Engine In Action:
![alt text](images/terra-example.png "Example Image")
![alt text](images/terra-instancing.png "Instancing Example")


## Installation:
The Engine build files are prepared with **CMake** and your chosen build tool (`make`, `ninja` etc) is used as for compilation.

### Mac/Windows
> Ensure you have **CMake** installed

To Clone
```bash
git clone https://github.com/bogger12/Terra.git
cd Terra
```
To build with **CMake**
```bash
cmake -S . -B build
cmake --build build --config Release
```

To run the Engine
```bash
cd build/bin
./Terra
```

### Linux

*Instructions in progress, check back soon*

## Controls:

- **WASD** : *Move Camera*
- **Mouse** : *Rotate Camera*
- **ESC** : *Enter/Exit Camera Mode*


## Libraries Used:
GLFW3 - Window Management

OpenGL - 3D Rendering

GLAD - OpenGL Extension Loader

[ENTT](https://github.com/skypjack/entt) - Entity Component System

[Dear ImGui](https://github.com/ocornut/imgui#) - Immediate-Mode GUI
