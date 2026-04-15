# RPG CPP Remake

**A story-driven 2D RPG built with C++17 + raylib, featuring a playable main campaign and a creator-friendly engine for building custom adventures.**

This project combines a classic single-player RPG experience with extensible tooling: play the built-in scenario, then create your own maps, quests, and content using external data and assets.

## ✨ Features

### 🧭 Core Gameplay

- Pre-defined main story/scenario with progression-focused RPG gameplay.
- 2D exploration, combat-ready architecture, and event-driven interactions.
- Designed for smooth native execution on **macOS (Apple Silicon)**.

### 🛠️ Extensibility & Editor Workflow

- Built-in engine/editor concepts for creating and loading custom scenarios.
- Scenario and map data structured to be expanded without recompiling core systems.
- Author your own game worlds and story content on top of the engine foundation.

### 🎨 Custom Assets Pipeline

- Import and use custom:
  - Sprites
  - Tilesets
  - Audio files (music/SFX)
- Data-driven asset assignment for rapid iteration and modding workflows.

### ⚔️ Modular Item System

- Add weapons, armor, and consumables through external configuration files.
- JSON-based serialization for flexible balancing and content updates.
- Easily expand item definitions without changing gameplay code paths.

### 🧱 Tech Stack

- **Language:** C++17
- **Graphics/Game Framework:** raylib
- **Build System:** CMake
- **Serialization/Data:** JSON

---

## 🚀 Installation (macOS Apple Silicon)

Clone the repository and enter the project directory:

```bash
git clone https://github.com/Teczak-dev/RPG-CPP-Remake.git
cd RPG-CPP-Remake
```

Install required dependencies with Homebrew:

```bash
brew install raylib cmake
```

Optional sanity check:

```bash
cmake --version
```

---

## 🔧 Build & Compile (CMake)

From the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build -j
```

Run the executable:

```bash
./build/rpg_cpp_remake
```

On macOS, CMake now builds an app bundle:

```bash
open ./build/rpg_cpp_remake.app
```

---

## ▶️ Local Run Scripts

Quick start scripts are available in `scripts/`:

- macOS: `./scripts/run_macos.sh`
- Linux: `./scripts/run_linux.sh`
- Windows (Git Bash): `./scripts/run_windows.sh`

Use:

```bash
chmod +x ./scripts/run_macos.sh ./scripts/run_linux.sh ./scripts/run_windows.sh
```

Then run the script for your platform.

---

## 🖼️ App Icon Configuration

You can set platform icons without changing source code:

- macOS bundle icon (`.icns`): `assets/icons/app.icns`
- Windows executable icon (`.ico`): `assets/icons/app.ico`

If needed, override icon paths at configure time:

```bash
cmake -S . -B build \
  -DAPP_ICON_MACOS=/absolute/path/to/app.icns \
  -DAPP_ICON_WINDOWS=/absolute/path/to/app.ico
```

Generate both icon files from a single source image:

1. Put one source file (`.svg`, `.png`, `.jpg`, `.jpeg`, or `.webp`) in:
   - `assets/icon-source/`
2. Run:

```bash
chmod +x ./scripts/generate_icons.sh
./scripts/generate_icons.sh
```

Or pass an explicit source path:

```bash
./scripts/generate_icons.sh /absolute/path/to/logo.svg
```

Output:
- `assets/icons/app.icns`
- `assets/icons/app.ico`

Dependencies for icon generation on macOS:

```bash
brew install imagemagick librsvg
```

---

## 📦 Usage and Installation Guide

This is an open-source project, and distributed binaries are **not digitally signed** to keep the project free and accessible.  
Because of that, your OS may show a security warning the first time you run the game.

### macOS (Apple Silicon / Intel)

If you see **"unidentified developer"** or **"app is damaged and can't be opened"**:

1. In Finder, right-click the app/binary and choose **Open**.
2. Confirm with **Open** in the dialog.

If the warning still appears:

1. Open **System Settings → Privacy & Security**.
2. Find the blocked app message and click **Open Anyway**.
3. Confirm launch.

### Windows

If Windows SmartScreen blocks startup:

1. Click **More info**.
2. Click **Run anyway**.

### Linux

Install common runtime dependencies for raylib-based games (package names may vary by distro):

- `libasound2`
- `libx11-6`
- `libxrandr2`
- `libxi6`
- `libxcursor1`
- `libxinerama1`
- `libgl1` (Mesa/OpenGL runtime)

On Debian/Ubuntu, for example:

```bash
sudo apt-get update
sudo apt-get install -y libasound2 libx11-6 libxrandr2 libxi6 libxcursor1 libxinerama1 libgl1
```

Make the binary executable and run it:

```bash
chmod +x ./rpg_cpp_remake
./rpg_cpp_remake
```

---

## 📁 Directory Structure

```text
.
├── src/           # Core gameplay logic, systems, and engine/editor runtime
├── include/       # Public headers and shared interfaces
├── assets/        # Sprites, tilesets, audio, and other game resources
├── scenarios/     # Story data, maps, events, and custom scenario definitions
├── scripts/       # Utility scripts (run/build/icons)
├── .github/       # GitHub Actions workflows
├── version.json   # Project metadata (version/build/author/license)
├── LICENSE        # MIT-style license + required author attribution
├── CMakeLists.txt # Build configuration
└── README.md
```

---

## ⚙️ CI/CD

This project uses **GitHub Actions** for automated multi-platform builds on:

- macOS (`.app` bundle packed as `.zip`)
- Ubuntu
- Windows (`.exe` packed as `.zip`)

The workflow reads `version.json`, builds with CMake, then uploads downloadable artifacts for each platform.
When you push a tag like `v0.0.1`, it also creates a GitHub Release and attaches all platform packages automatically.

---

## 🗺️ Roadmap

This roadmap outlines the project’s technical direction and gameplay goals across short-, mid-, and long-term milestones.

### Phase 1: Foundation (Current Year)

- [x] Project bootstrap: C++17 + raylib setup, CMake build pipeline, and 60 FPS base loop.
- [ ] Core Engine: Basic state management, camera 2D system, and asset loading (textures/fonts).
- [ ] Tilemap System: Rendering grid-based maps from JSON.
- [ ] Player Controller: Basic movement, animations, and collision detection.

### Phase 2: World Building & Content (Next Year)

- [ ] In-game Editor: Real-time map editing and scenario scripting tool.
- [ ] Combat System: Turn-based or real-time (basic) combat mechanics.
- [ ] Item System: Advanced weaponry and inventory management via JSON data.

### Phase 3: Expansion & Polishing (Long-term)

- [ ] Audio Engine: Spatial sound effects and background music management.
- [ ] Modding Support: Allowing users to add custom C++ or Lua scripts.
- [ ] Cross-platform optimization: Ensuring 60 FPS on low-end hardware and Steam Deck.

---

## 📄 License

This project is licensed under an **MIT-style license with a required attribution clause**.  
If you modify and redistribute this project, you must credit the original author and indicate changes.  
See [`LICENSE`](./LICENSE) for full terms.

---

## 🤝 Contributing

Contributions are welcome and appreciated.

1. Fork the repository.
2. Create a feature branch.
3. Make focused changes with clear commit messages.
4. Open a pull request describing the problem and solution.

Please keep contributions modular, documented, and consistent with the project’s C++/CMake style.

---

## 📌 Portfolio Note

This project highlights practical game architecture in modern C++: data-driven systems, reusable engine components, and content extensibility for both players and creators.
