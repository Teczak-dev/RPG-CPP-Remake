#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace core {

// Data-driven input map (keyboard + gamepad) loaded from JSON.
// This keeps controls editable without recompiling gameplay code.
class InputBindings {
 public:
  // Digital binding stores "pressed/down" sources for a named action.
  struct DigitalBinding {
    std::vector<int> keys;
    std::vector<int> gamepadButtons;
  };

  // Axis binding combines digital directions with optional analog axis input.
  struct AxisBinding {
    DigitalBinding negative;
    DigitalBinding positive;
    int gamepadAxis = -1;
    float deadZone = 0.18f;
  };

  bool LoadFromJsonFile(const std::string &path, std::string &errorMessage);

  bool IsActionPressed(const std::string &actionId) const;
  bool IsActionDown(const std::string &actionId) const;
  float GetAxis(const std::string &axisId) const;

 private:
  std::unordered_map<std::string, DigitalBinding> actions_;
  std::unordered_map<std::string, AxisBinding> axes_;
};

}  // namespace core
