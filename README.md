# 3D Bouncing Ball – C++ & OpenGL

A 3D physics simulation built with C++ and OpenGL (GLUT), where a ball bounces inside a wireframe cube with realistic gravity, elasticity, and friction. The scene includes obstacles, dynamic lighting, and full camera control.

---

## Features

- Real-time 3D physics: gravity, elasticity, and friction
- Sphere-cube collision detection with reflection vectors
- Two in-scene obstacles (ground cube and wall cube)
- Dual light sources with toggleable control and adjustable shininess
- Free camera movement and rotation (keyboard + mouse)
- Mouse-drag to launch the ball in any direction
- Smooth 60+ FPS rendering loop

---

## Controls

| Key / Input | Action |
|-------------|--------|
| `W / S` | Move camera forward / backward |
| `A / D` | Move camera left / right |
| `Q / E` | Move camera up / down |
| Arrow Keys | Rotate camera (pitch & yaw) |
| Left Mouse Drag | Launch ball |
| `Space` | Stop the ball |
| `R` | Reset ball to center |
| `0` | Toggle Light 0 |
| `1` | Toggle Light 1 |
| `2` | Increase shininess |
| `Esc` | Quit |

---

## Technologies

- **Language:** C++
- **Graphics:** OpenGL, GLUT
- **Physics:** Custom gravity, elasticity, friction, sphere-cube collision

---

## How to Build

### Requirements
- OpenGL & GLUT (freeglut)
- g++ compiler

### Compile
```bash
g++ main.cpp -o ball -lGL -lGLU -lglut
./ball
```

> On Windows with MinGW:
> ```bash
> g++ main.cpp -o ball.exe -lfreeglut -lopengl32 -lglu32
> ```

---

## Project Structure

```
├── main.cpp       # Full source code
└── README.md
```
