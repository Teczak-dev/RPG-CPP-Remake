#include "game/Player.hpp"

#include <cmath>

namespace game {
namespace {

// Normalize input so diagonal movement is not faster than horizontal/vertical.
Vector2 NormalizeAxis(Vector2 axis) {
  const float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
  if (length <= 1.0f || length == 0.0f) {
    return axis;
  }
  return {axis.x / length, axis.y / length};
}

} // namespace

void Player::Reset(Vector2 position) { position_ = position; }

void Player::SetColor(Color color) { color_ = color; }

void Player::Update(float deltaTime, Vector2 moveAxis) {
  const Vector2 axis = NormalizeAxis(moveAxis);
  position_.x += axis.x * moveSpeed_ * deltaTime;
  position_.y += axis.y * moveSpeed_ * deltaTime;

  const float screenWidth = static_cast<float>(GetScreenWidth());
  const float screenHeight = static_cast<float>(GetScreenHeight());
  // Clamp to the visible area to keep the placeholder actor always on-screen.
  const float halfSize = size_ * 0.5f;
  if (position_.x < halfSize)
    position_.x = halfSize;
  if (position_.y < halfSize)
    position_.y = halfSize;
  if (position_.x > screenWidth - halfSize)
    position_.x = screenWidth - halfSize;
  if (position_.y > screenHeight - halfSize)
    position_.y = screenHeight - halfSize;
}

void Player::Draw() const {
  const Rectangle body = {position_.x - size_ * 0.5f,
                          position_.y - size_ * 0.5f, size_, size_};
  DrawRectangleRounded(body, 0.18f, 4, color_);
}

} // namespace game
