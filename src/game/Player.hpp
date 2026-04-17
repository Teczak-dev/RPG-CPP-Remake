#pragma once

#include "raylib.h"

namespace game {

// Player is a lightweight gameplay entity used by both Story and Sandbox
// runtime scenes. It only owns movement/render state so it can be reused
// across modes without depending on UI flow.
class Player {
public:
  void Reset(Vector2 position);
  void SetColor(Color color);
  void Update(float deltaTime, Vector2 moveAxis);
  void Draw() const;

private:
  Vector2 position_{640.0f, 360.0f};
  float size_ = 44.0f;
  float moveSpeed_ = 260.0f;
  Color color_ = SKYBLUE;
};

} // namespace game
