#pragma once

#include "raylib.h"

#include <string>

namespace ui {

// Minimal reusable UI primitives shared by menus and creator screens.
struct Button {
  Rectangle bounds{};
  std::string label;
};

bool IsHovered(const Button &button, Vector2 mousePos);
bool IsClicked(const Button &button, Vector2 mousePos, bool leftClickPressed);
void DrawButton(const Button &button, bool hovered, bool active = false);

struct TextInput {
  Rectangle bounds{};
  std::string value;
  int maxLength = 18;
  bool focused = false;
};

void UpdateTextInput(TextInput &input, Vector2 mousePos, bool leftClickPressed);
void DrawTextInput(const TextInput &input, const char *placeholder);

} // namespace ui
