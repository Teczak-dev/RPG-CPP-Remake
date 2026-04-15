# RPG CPP Remake - Release Notes

## Changelog

> Update this section before publishing.

- Added a new main menu screen with title and buttons: **New Game**, **Load Game**, **Options**, and **Exit**.
- Added in-game status messaging to clearly communicate that selected features are still in development.

[Jump to installation guide](#installation-guide)

## Installation Guide

These binaries are **unsigned**.  
Because there is no digital signature yet, your OS may show a warning on first launch.

### macOS

1. Download the `...-macos.zip` asset and extract it.
2. Open `rpg_cpp_remake.app`.
3. If blocked by Gatekeeper:
   - Right-click the app -> **Open**.
   - Or go to **System Settings -> Privacy & Security** and click **Open Anyway**.

Apple Silicon tip (if macOS still says the app is damaged/unverified):

```bash
xattr -dr com.apple.quarantine ./rpg_cpp_remake.app
```

### Windows

1. Download the `...-windows.zip` asset and extract it.
2. Run `rpg_cpp_remake.exe`.
3. If SmartScreen appears:
   - Click **More info**.
   - Click **Run anyway**.

### Linux

1. Download the `...-linux.tar.gz` asset and extract it.
2. Open terminal in extracted directory and run:

```bash
chmod +x ./rpg_cpp_remake
./rpg_cpp_remake
```

If needed, install runtime dependencies (example for Debian/Ubuntu):

```bash
sudo apt-get update
sudo apt-get install -y libasound2 libx11-6 libxrandr2 libxi6 libxcursor1 libxinerama1 libgl1
```
