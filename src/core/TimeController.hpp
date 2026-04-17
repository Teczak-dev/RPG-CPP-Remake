#pragma once

namespace core {

// Centralized time gate used to pause gameplay simulation while still allowing
// UI to update and draw every frame.
class TimeController {
 public:
  void BeginFrame();
  void SetPaused(bool paused);
  bool IsPaused() const;
  float DeltaTime() const;
  float RealDeltaTime() const;

 private:
  bool paused_ = false;
  float deltaTime_ = 0.0f;
  float realDeltaTime_ = 0.0f;
};

}  // namespace core
