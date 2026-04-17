#include "core/TimeController.hpp"

#include "raylib.h"

namespace core {

void TimeController::BeginFrame() {
  realDeltaTime_ = GetFrameTime();
  // When paused, gameplay systems consume zero delta while rendering continues.
  deltaTime_ = paused_ ? 0.0f : realDeltaTime_;
}

void TimeController::SetPaused(bool paused) { paused_ = paused; }

bool TimeController::IsPaused() const { return paused_; }

float TimeController::DeltaTime() const { return deltaTime_; }

float TimeController::RealDeltaTime() const { return realDeltaTime_; }

}  // namespace core
