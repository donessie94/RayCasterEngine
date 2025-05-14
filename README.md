# RayCasterEngine

A lightweight C++ 2.5D raycasting demo built with SDL2 and SDL2_image. Walk through textured floors, ceilings, and walls in a simple Doom-style world—complete with a mini-map, FPS counter, and basic lighting.

<p align="center">
  <img width="1277" alt="Screenshot 2025-05-13 at 8 28 19 PM" src="https://github.com/user-attachments/assets/da81b0b0-e48a-45ac-8edf-17c9018dd6fe" />
  <img width="1280" alt="Screenshot 2025-05-13 at 8 36 11 PM" src="https://github.com/user-attachments/assets/dfe3ab64-84da-4efa-903c-4ebf7d880f96" />
  <img width="1279" alt="Screenshot 2025-05-13 at 8 36 26 PM" src="https://github.com/user-attachments/assets/ca88fa78-6559-4fbe-a584-6413b00b1d0d" />
</p>


---

## Features

- **Classic Raycasting** engine (Doom-style)  
- **Textured** floors, ceilings & walls  
- **Dynamic lighting** and simple shading  
- **Mini-map** overlay showing your position & orientation  
- **FPS counter**  
- **Clean folder structure** under `lib/` and `textures/`  

---

## Controls

- **W / S** — Move forward / back  
- **A / D** — Strafe left / right  
- **↑ / ↓** — Look up / down (camera pitch)  
- **← / →** — Turn left / right (camera yaw)  
- **M** — Toggle mini-map  
- **Q** — Quit  

---

## Build & Run (Makefile)

```bash
# 1. Install prerequisites
# Ubuntu / Debian:
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-image-dev

# macOS (Homebrew):
brew install sdl2 sdl2_image

# 2. Clone & enter project
git clone https://github.com/your-username/RayCasterEngine.git
cd RayCasterEngine

# 3. Build with Makefile
make

# 4. Run
./chess

# 5. Clean up
make clean
