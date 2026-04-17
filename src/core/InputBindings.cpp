#include "core/InputBindings.hpp"

#include "raylib.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

namespace core {
namespace {

// Internal helpers stay in an anonymous namespace to avoid exporting parsing
// utilities outside this translation unit.
std::string ToUpper(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
    return static_cast<char>(std::toupper(c));
  });
  return text;
}

bool ReadFile(const std::string &path, std::string &content) {
  std::ifstream file(path);
  if (!file) {
    return false;
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  content = buffer.str();
  return true;
}

bool ExtractStringField(const std::string &objectText, const char *fieldName,
                        std::string &value) {
  const std::regex pattern(std::string("\"") + fieldName +
                           "\"\\s*:\\s*\"([^\"]*)\"");
  std::smatch match;
  if (!std::regex_search(objectText, match, pattern)) {
    return false;
  }
  value = match[1].str();
  return true;
}

bool ExtractFloatField(const std::string &objectText, const char *fieldName,
                       float &value) {
  const std::regex pattern(std::string("\"") + fieldName +
                           "\"\\s*:\\s*(-?[0-9]+(?:\\.[0-9]+)?)");
  std::smatch match;
  if (!std::regex_search(objectText, match, pattern)) {
    return false;
  }
  value = std::stof(match[1].str());
  return true;
}

std::vector<std::string> ExtractStringArrayField(const std::string &objectText,
                                                 const char *fieldName) {
  const std::regex arrayPattern(std::string("\"") + fieldName +
                                "\"\\s*:\\s*\\[([^\\]]*)\\]");
  std::smatch arrayMatch;
  if (!std::regex_search(objectText, arrayMatch, arrayPattern)) {
    return {};
  }

  std::vector<std::string> result;
  const std::string body = arrayMatch[1].str();
  const std::regex valuePattern("\"([^\"]*)\"");
  auto it = std::sregex_iterator(body.begin(), body.end(), valuePattern);
  const auto end = std::sregex_iterator();
  for (; it != end; ++it) {
    result.push_back((*it)[1].str());
  }
  return result;
}

int ParseKeyName(const std::string &rawName) {
  const std::string name = ToUpper(rawName);
  if (name == "A")
    return KEY_A;
  if (name == "B")
    return KEY_B;
  if (name == "C")
    return KEY_C;
  if (name == "D")
    return KEY_D;
  if (name == "E")
    return KEY_E;
  if (name == "F")
    return KEY_F;
  if (name == "G")
    return KEY_G;
  if (name == "H")
    return KEY_H;
  if (name == "I")
    return KEY_I;
  if (name == "J")
    return KEY_J;
  if (name == "K")
    return KEY_K;
  if (name == "L")
    return KEY_L;
  if (name == "M")
    return KEY_M;
  if (name == "N")
    return KEY_N;
  if (name == "O")
    return KEY_O;
  if (name == "P")
    return KEY_P;
  if (name == "Q")
    return KEY_Q;
  if (name == "R")
    return KEY_R;
  if (name == "S")
    return KEY_S;
  if (name == "T")
    return KEY_T;
  if (name == "U")
    return KEY_U;
  if (name == "V")
    return KEY_V;
  if (name == "W")
    return KEY_W;
  if (name == "X")
    return KEY_X;
  if (name == "Y")
    return KEY_Y;
  if (name == "Z")
    return KEY_Z;
  if (name == "UP")
    return KEY_UP;
  if (name == "DOWN")
    return KEY_DOWN;
  if (name == "LEFT")
    return KEY_LEFT;
  if (name == "RIGHT")
    return KEY_RIGHT;
  if (name == "ESCAPE")
    return KEY_ESCAPE;
  return -1;
}

int ParseGamepadButtonName(const std::string &rawName) {
  const std::string name = ToUpper(rawName);
  if (name == "MIDDLE_LEFT")
    return GAMEPAD_BUTTON_MIDDLE_LEFT;
  if (name == "MIDDLE_RIGHT" || name == "START")
    return GAMEPAD_BUTTON_MIDDLE_RIGHT;
  if (name == "LEFT_FACE_UP")
    return GAMEPAD_BUTTON_LEFT_FACE_UP;
  if (name == "LEFT_FACE_DOWN")
    return GAMEPAD_BUTTON_LEFT_FACE_DOWN;
  if (name == "LEFT_FACE_LEFT")
    return GAMEPAD_BUTTON_LEFT_FACE_LEFT;
  if (name == "LEFT_FACE_RIGHT")
    return GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
  return -1;
}

int ParseAxisName(const std::string &rawName) {
  const std::string name = ToUpper(rawName);
  if (name == "LEFT_X")
    return GAMEPAD_AXIS_LEFT_X;
  if (name == "LEFT_Y")
    return GAMEPAD_AXIS_LEFT_Y;
  return -1;
}

InputBindings::DigitalBinding
BuildDigitalBinding(const std::vector<std::string> &keys,
                    const std::vector<std::string> &buttons) {
  InputBindings::DigitalBinding binding;
  for (const std::string &keyName : keys) {
    const int key = ParseKeyName(keyName);
    if (key >= 0) {
      binding.keys.push_back(key);
    }
  }
  for (const std::string &buttonName : buttons) {
    const int button = ParseGamepadButtonName(buttonName);
    if (button >= 0) {
      binding.gamepadButtons.push_back(button);
    }
  }
  return binding;
}

bool AnyKeyDown(const std::vector<int> &keys) {
  for (const int key : keys) {
    if (IsKeyDown(key)) {
      return true;
    }
  }
  return false;
}

bool AnyKeyPressed(const std::vector<int> &keys) {
  for (const int key : keys) {
    if (IsKeyPressed(key)) {
      return true;
    }
  }
  return false;
}

bool AnyGamepadButtonDown(const std::vector<int> &buttons) {
  if (!IsGamepadAvailable(0)) {
    return false;
  }
  for (const int button : buttons) {
    if (IsGamepadButtonDown(0, button)) {
      return true;
    }
  }
  return false;
}

bool AnyGamepadButtonPressed(const std::vector<int> &buttons) {
  if (!IsGamepadAvailable(0)) {
    return false;
  }
  for (const int button : buttons) {
    if (IsGamepadButtonPressed(0, button)) {
      return true;
    }
  }
  return false;
}

} // namespace

bool InputBindings::LoadFromJsonFile(const std::string &path,
                                     std::string &errorMessage) {
  std::string content;
  if (!ReadFile(path, content)) {
    errorMessage = "Cannot read input bindings file: " + path;
    return false;
  }

  // Parse into temporary maps first, then swap on success to keep state valid
  // even if a file is malformed.
  std::unordered_map<std::string, DigitalBinding> parsedActions;
  std::unordered_map<std::string, AxisBinding> parsedAxes;
  const std::regex objectRegex(R"(\{[^{}]*\})");
  auto it = std::sregex_iterator(content.begin(), content.end(), objectRegex);
  const auto end = std::sregex_iterator();
  for (; it != end; ++it) {
    const std::string objectText = it->str();
    std::string type;
    std::string id;
    if (!ExtractStringField(objectText, "type", type) ||
        !ExtractStringField(objectText, "id", id)) {
      continue;
    }

    if (type == "action") {
      parsedActions[id] = BuildDigitalBinding(
          ExtractStringArrayField(objectText, "keys"),
          ExtractStringArrayField(objectText, "gamepadButtons"));
      continue;
    }

    if (type == "axis") {
      AxisBinding axis;
      axis.negative = BuildDigitalBinding(
          ExtractStringArrayField(objectText, "negativeKeys"),
          ExtractStringArrayField(objectText, "negativeGamepadButtons"));
      axis.positive = BuildDigitalBinding(
          ExtractStringArrayField(objectText, "positiveKeys"),
          ExtractStringArrayField(objectText, "positiveGamepadButtons"));
      std::string axisName;
      if (ExtractStringField(objectText, "gamepadAxis", axisName)) {
        axis.gamepadAxis = ParseAxisName(axisName);
      }
      ExtractFloatField(objectText, "deadZone", axis.deadZone);
      parsedAxes[id] = axis;
    }
  }

  if (parsedActions.empty() && parsedAxes.empty()) {
    errorMessage = "No valid bindings found in: " + path;
    return false;
  }

  actions_ = std::move(parsedActions);
  axes_ = std::move(parsedAxes);
  return true;
}

bool InputBindings::IsActionPressed(const std::string &actionId) const {
  const auto it = actions_.find(actionId);
  if (it == actions_.end()) {
    return false;
  }
  const auto &binding = it->second;
  return AnyKeyPressed(binding.keys) ||
         AnyGamepadButtonPressed(binding.gamepadButtons);
}

bool InputBindings::IsActionDown(const std::string &actionId) const {
  const auto it = actions_.find(actionId);
  if (it == actions_.end()) {
    return false;
  }
  const auto &binding = it->second;
  return AnyKeyDown(binding.keys) ||
         AnyGamepadButtonDown(binding.gamepadButtons);
}

float InputBindings::GetAxis(const std::string &axisId) const {
  const auto it = axes_.find(axisId);
  if (it == axes_.end()) {
    return 0.0f;
  }

  const AxisBinding &axis = it->second;
  float value = 0.0f;
  if (AnyKeyDown(axis.negative.keys) ||
      AnyGamepadButtonDown(axis.negative.gamepadButtons)) {
    value -= 1.0f;
  }
  if (AnyKeyDown(axis.positive.keys) ||
      AnyGamepadButtonDown(axis.positive.gamepadButtons)) {
    value += 1.0f;
  }

  if (axis.gamepadAxis >= 0 && IsGamepadAvailable(0)) {
    const float analog = GetGamepadAxisMovement(0, axis.gamepadAxis);
    if (std::fabs(analog) > axis.deadZone) {
      value += analog;
    }
  }

  if (value > 1.0f) {
    value = 1.0f;
  } else if (value < -1.0f) {
    value = -1.0f;
  }
  return value;
}

} // namespace core
