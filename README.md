# 🎱 Bouncing Ball — 3D Physics Simulation

A 3D physics-based bouncing ball simulation built from scratch in **C++** using **OpenGL (GLUT)**. The ball moves inside a wireframe cube, interacts with obstacles, responds to gravity and elasticity, and can be controlled via keyboard and mouse.

![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Graphics](https://img.shields.io/badge/graphics-OpenGL%20%2F%20GLUT-orange)

---

## 📖 About the Project

This project simulates realistic 3D ball physics inside a bounded environment. The ball bounces off walls, the floor, the ceiling, and two solid obstacles using reflection-based collision response. The user can interact with the ball using the mouse and freely navigate the scene with the camera.

---

## ✨ Features

- **Real-time 3D physics** — gravity, velocity, elasticity, and friction applied every frame
- **Collision detection & response** — sphere-vs-wall and sphere-vs-cube (AABB) collisions with reflection vectors
- **Two solid obstacles** — a ground cube and a wall cube, each with accurate face-normal collision response
- **Bounce tracking** — the ball gradually loses energy and settles after repeated bounces
- **Free camera control** — move and rotate the camera using keyboard and mouse
- **Dual lighting system** — two configurable light sources with material properties (ambient, diffuse, specular, shininess)
- **Mouse interaction** — click and drag to throw the ball in any direction
- **Ball rotation** — the ball visually spins as it moves

---

## 🕹️ Controls

### Ball & Game

| Key | Action |
|-----|--------|
| `Space` | Stop the ball |
| `R` | Reset ball to starting position |

### Camera Movement

| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `Q` | Move up |
| `E` | Move down |
| `↑ / ↓` | Pitch camera up / down |
| `← / →` | Yaw camera left / right |

### Lighting

| Key | Action |
|-----|--------|
| `0` | Toggle Light 0 |
| `1` | Toggle Light 1 |
| `2` | Increase material shininess |

### Mouse

| Action | Effect |
|--------|--------|
| Left click + drag | Launch the ball in the drag direction |

---

## ⚙️ How to Run

This project requires a C++ compiler and the **freeglut**/GLUT development library.

### Windows (MinGW example)
```bash
g++ main.cpp -o BouncingBall -lfreeglut -lopengl32 -lglu32
BouncingBall.exe
```

### Linux
```bash
sudo apt-get install freeglut3-dev
g++ main.cpp -o BouncingBall -lglut -lGL -lGLU
./BouncingBall
```

---

## 🛠️ Built With

- **C++**
- **OpenGL** (fixed-function pipeline)
- **GLUT / freeglut** — windowing, input handling, and the render loop

---

## 🧠 Physics Overview

| Property | Value |
|----------|-------|
| Gravity | −0.002 per tick |
| Elasticity | 0.8 (80% velocity retained on bounce) |
| Friction | 0.999 per frame |
| Bounce limit | 3 rapid bounces before the ball settles |

Collision with obstacles uses **AABB closest-point** detection and **reflection vectors** based on the nearest face normal.

---

## 📚 What I Learned

Building this project helped me strengthen my skills in:
- 3D graphics programming with OpenGL (transformations, lighting, materials)
- Real-time physics simulation (gravity, elasticity, friction, reflection)
- Sphere-AABB collision detection using closest-point algorithms
- Free-look camera implementation with yaw and pitch
- Frame-independent movement using delta time

---

## 👤 Author

**Your Name Here**
Computer Science Student
[LinkedIn](https://linkedin.com)
