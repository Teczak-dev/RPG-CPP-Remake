#include "ui/Widgets.hpp"

namespace ui {

bool IsHovered(const Button &button, Vector2 mousePos) {
  return CheckCollisionPointRec(mousePos, button.bounds);
}

bool IsClicked(const Button &button, Vector2 mousePos, bool leftClickPressed) {
  return leftClickPressed && IsHovered(button, mousePos);
}

void DrawButton(const Button &button, bool hovered, bool active) {
  // Active state highlights selected/toggled options in multi-step forms.
  const Color fill = active ? Color{206, 236, 206, 235}
                            : (hovered ? Color{255, 255, 255, 235}
                                       : Color{238, 243, 238, 225});
  const Color border = active ? Color{20, 95, 20, 255} : Color{38, 69, 38, 255};
  const Color text = active ? Color{20, 79, 20, 255} : Color{26, 49, 26, 255};
  DrawRectangleRounded(button.bounds, 0.25f, 8, fill);
  DrawRectangleRoundedLinesEx(button.bounds, 0.25f, 8, 2.0f, border);

  const int labelFontSize = 26;
  const int labelWidth = MeasureText(button.label.c_str(), labelFontSize);
  const int textX = static_cast<int>(button.bounds.x +
                                     (button.bounds.width - labelWidth) / 2.0f);
  const int textY = static_cast<int>(
      button.bounds.y + (button.bounds.height - labelFontSize) / 2.0f + 1.0f);
  DrawText(button.label.c_str(), textX, textY, labelFontSize, text);
}

void UpdateTextInput(TextInput &input, Vector2 mousePos,
                     bool leftClickPressed) {
  if (leftClickPressed) {
    input.focused = CheckCollisionPointRec(mousePos, input.bounds);
  }

  if (!input.focused) {
    return;
  }

  for (int key = GetCharPressed(); key != 0; key = GetCharPressed()) {
    // Keep input simple and stable by accepting visible ASCII characters only.
    if (key >= 32 && key <= 126 &&
        static_cast<int>(input.value.size()) < input.maxLength) {
      input.value.push_back(static_cast<char>(key));
    }
  }

  if (IsKeyPressed(KEY_BACKSPACE) && !input.value.empty()) {
    input.value.pop_back();
  }
}

void DrawTextInput(const TextInput &input, const char *placeholder) {
  const Color fill =
      input.focused ? Color{255, 255, 255, 250} : Color{240, 244, 240, 225};
  const Color border = input.focused ? DARKGREEN : Color{70, 98, 70, 255};
  DrawRectangleRounded(input.bounds, 0.2f, 8, fill);
  DrawRectangleRoundedLinesEx(input.bounds, 0.2f, 8, 2.0f, border);

  if (input.value.empty()) {
    DrawText(placeholder, static_cast<int>(input.bounds.x + 14),
             static_cast<int>(input.bounds.y + 16), 24, DARKGRAY);
  } else {
    DrawText(input.value.c_str(), static_cast<int>(input.bounds.x + 14),
             static_cast<int>(input.bounds.y + 16), 24, Color{24, 46, 24, 255});
  }
}

} // namespace ui
