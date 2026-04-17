#include "game/GameApp.hpp"

#include "core/InputBindings.hpp"
#include "core/TimeController.hpp"
#include "game/Player.hpp"
#include "raylib.h"
#include "ui/Widgets.hpp"

#include <array>
#include <filesystem>
#include <string>
#include <vector>

namespace game {
namespace {

// High-level app screens. StoryGameplay is reused by Story start and
// post-creator Sandbox start to keep runtime behavior consistent.
enum class AppScreen {
  MainMenu,
  NewGameMenu,
  SandboxName,
  SandboxOrigin,
  SandboxAppearance,
  SandboxTraits,
  StoryGameplay,
};

class GameApp {
public:
  // Constructor builds static menu layout once. Runtime state transitions are
  // handled in Update* methods to keep drawing code declarative.
  GameApp() {
    const float centerX = (screenWidth_ - buttonWidth_) * 0.5f;
    const float navButtonWidth = 180.0f;
    const float navButtonHeight = 54.0f;
    const float navButtonsY = 492.0f;
    const float screenCenterX = screenWidth_ * 0.5f;
    const float navGap = 20.0f;
    const float navBackX = screenCenterX - navButtonWidth - (navGap * 0.5f);
    const float navNextX = screenCenterX + (navGap * 0.5f);

    mainButtons_ = {
        ui::Button{{centerX, 230.0f, buttonWidth_, buttonHeight_}, "New Game"},
        ui::Button{{centerX, 306.0f, buttonWidth_, buttonHeight_}, "Load Game"},
        ui::Button{{centerX, 382.0f, buttonWidth_, buttonHeight_}, "Options"},
        ui::Button{{centerX, 458.0f, buttonWidth_, buttonHeight_}, "Exit"},
    };

    newGameButtons_ = {
        ui::Button{{centerX, 250.0f, buttonWidth_, buttonHeight_}, "Story"},
        ui::Button{{centerX, 326.0f, buttonWidth_, buttonHeight_}, "Sandbox"},
        ui::Button{{centerX, 402.0f, buttonWidth_, buttonHeight_}, "Back"},
    };

    // Keep navigation buttons anchored to the same position on every sandbox
    // step.
    nameBackButton_ = {{navBackX, navButtonsY, navButtonWidth, navButtonHeight},
                       "Back"};
    nameNextButton_ = {{navNextX, navButtonsY, navButtonWidth, navButtonHeight},
                       "Next"};
    nameInput_ = {{centerX - 130.0f, 292.0f, buttonWidth_ + 120.0f, 58.0f},
                  "",
                  20,
                  false};

    originButtons_ = {
        ui::Button{{centerX, 230.0f, buttonWidth_, buttonHeight_}, "Peasant"},
        ui::Button{{centerX, 306.0f, buttonWidth_, buttonHeight_}, "Townsman"},
        ui::Button{{centerX, 382.0f, buttonWidth_, buttonHeight_}, "Prince"},
    };

    appearanceButtons_ = {
        ui::Button{{centerX, 230.0f, buttonWidth_, buttonHeight_}, "Wanderer"},
        ui::Button{{centerX, 306.0f, buttonWidth_, buttonHeight_}, "Knight"},
        ui::Button{{centerX, 382.0f, buttonWidth_, buttonHeight_}, "Archer"},
    };

    stepBackButton_ = {{navBackX, navButtonsY, navButtonWidth, navButtonHeight},
                       "Back"};
    stepNextButton_ = {{navNextX, navButtonsY, navButtonWidth, navButtonHeight},
                       "Next"};
    finishButton_ = {{navNextX, navButtonsY, navButtonWidth, navButtonHeight},
                     "Confirm"};

    const float traitButtonWidth = 250.0f;
    const float traitCenter = screenWidth_ * 0.5f;
    const float traitGap = 20.0f;
    const float traitLeftX = traitCenter - traitButtonWidth - (traitGap * 0.5f);
    const float traitRightX = traitCenter + (traitGap * 0.5f);
    traitButtons_ = {
        ui::Button{{traitLeftX, 230.0f, traitButtonWidth, buttonHeight_},
                   "Blacksmith"},
        ui::Button{{traitRightX, 230.0f, traitButtonWidth, buttonHeight_},
                   "Sword Mastery"},
        ui::Button{{traitLeftX, 306.0f, traitButtonWidth, buttonHeight_},
                   "Bow Mastery"},
        ui::Button{{traitRightX, 306.0f, traitButtonWidth, buttonHeight_},
                   "Alchemy"},
    };

    pauseButtons_ = {
        ui::Button{{centerX, 232.0f, buttonWidth_, buttonHeight_}, "Resume"},
        ui::Button{{centerX, 308.0f, buttonWidth_, buttonHeight_}, "Options"},
        ui::Button{{centerX, 384.0f, buttonWidth_, buttonHeight_}, "Main Menu"},
        ui::Button{{centerX, 460.0f, buttonWidth_, buttonHeight_}, "Exit"},
    };

    infoPanel_ = {24.0f, static_cast<float>(screenHeight_ - 146), 690.0f,
                  118.0f};
    linkX_ = static_cast<int>(infoPanel_.x) + 18;
    linkY_ = static_cast<int>(infoPanel_.y) + 78;
  }

  int Run() {
    InitWindow(screenWidth_, screenHeight_, "RPG CPP Remake");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    linkWidth_ = MeasureText(projectUrl_, linkFontSize_);
    projectUrlRect_ = {static_cast<float>(linkX_), static_cast<float>(linkY_),
                       static_cast<float>(linkWidth_),
                       static_cast<float>(linkFontSize_)};
    LoadInputBindings();

    while (running_ && !WindowShouldClose()) {
      const Vector2 mousePos = GetMousePosition();
      const bool leftClickPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
      timeController_.BeginFrame();
      statusMessageTime_ -= timeController_.RealDeltaTime();

      Update(mousePos, leftClickPressed);
      Draw(mousePos);
    }

    CloseWindow();
    return 0;
  }

private:
  static constexpr int screenWidth_ = 1280;
  static constexpr int screenHeight_ = 720;
  static constexpr float buttonWidth_ = 280.0f;
  static constexpr float buttonHeight_ = 58.0f;

  const char *projectUrl_ = "https://github.com/Teczak-dev/RPG-CPP-Remake";
  int linkX_ = 0;
  int linkY_ = 0;
  int linkWidth_ = 0;
  int linkFontSize_ = 20;

  bool running_ = true;
  AppScreen screen_ = AppScreen::MainMenu;
  bool pauseMenuOpen_ = false;
  bool sandboxGameplayActive_ = false;
  std::string statusMessage_ = "Click a button to start.";
  float statusMessageTime_ = 0.0f;

  core::TimeController timeController_{};
  core::InputBindings inputBindings_{};
  Player storyPlayer_{};

  ui::TextInput nameInput_{};
  int selectedOrigin_ = 0;
  int selectedAppearance_ = 0;
  std::array<bool, 4> selectedTraits_ = {false, false, false, false};

  std::array<ui::Button, 4> mainButtons_{};
  std::array<ui::Button, 3> newGameButtons_{};
  ui::Button nameNextButton_{};
  ui::Button nameBackButton_{};
  std::array<ui::Button, 3> originButtons_{};
  std::array<ui::Button, 3> appearanceButtons_{};
  std::array<ui::Button, 4> traitButtons_{};
  ui::Button stepBackButton_{};
  ui::Button stepNextButton_{};
  ui::Button finishButton_{};
  std::array<ui::Button, 4> pauseButtons_{};

  Rectangle infoPanel_{};
  Rectangle projectUrlRect_{};

  void LoadInputBindings() {
    // We probe multiple candidate paths because app bundles and build-folder
    // runs have different working directories on macOS.
    const std::array<std::string, 2> fileNames = {"bindings.json",
                                                  "input.json"};
    const std::array<std::string, 5> relativePrefixes = {
        "assets/input/",
        "../assets/input/",
        "../../assets/input/",
        "../../../assets/input/",
        "../../../../assets/input/",
    };

    std::vector<std::string> bindingPaths;
    bindingPaths.reserve(24);

    for (const auto &name : fileNames) {
      for (const auto &prefix : relativePrefixes) {
        bindingPaths.push_back(prefix + name);
      }
    }

    const std::filesystem::path appDir =
        std::filesystem::path(GetApplicationDirectory());
    for (const auto &name : fileNames) {
      bindingPaths.push_back((appDir / ("../Resources/assets/input/" + name))
                                 .lexically_normal()
                                 .string());
      bindingPaths.push_back((appDir / ("../../../../assets/input/" + name))
                                 .lexically_normal()
                                 .string());
    }

    std::string errorMessage;
    bool loaded = false;
    for (const auto &path : bindingPaths) {
      if (inputBindings_.LoadFromJsonFile(path, errorMessage)) {
        loaded = true;
        break;
      }
    }

    if (!loaded) {
      SetStatus("Warning: input bindings JSON could not be loaded.", 4.0f);
    }
  }

  void SetStatus(const std::string &message, float duration) {
    statusMessage_ = message;
    statusMessageTime_ = duration;
  }

  bool AnyTraitSelected() const {
    for (const bool selected : selectedTraits_) {
      if (selected) {
        return true;
      }
    }
    return false;
  }

  void ResetSandboxDraft() {
    nameInput_.value.clear();
    nameInput_.focused = false;
    selectedOrigin_ = 0;
    selectedAppearance_ = 0;
    selectedTraits_ = {false, false, false, false};
  }

  void StartStoryMode() {
    pauseMenuOpen_ = false;
    sandboxGameplayActive_ = false;
    timeController_.SetPaused(false);
    storyPlayer_.Reset({screenWidth_ * 0.5f, screenHeight_ * 0.5f});
    storyPlayer_.SetColor(SKYBLUE);
    screen_ = AppScreen::StoryGameplay;
    SetStatus("Story scene loaded. Press Pause to open the menu.", 2.6f);
  }

  void StartSandboxMode() {
    pauseMenuOpen_ = false;
    sandboxGameplayActive_ = true;
    timeController_.SetPaused(false);
    storyPlayer_.Reset({screenWidth_ * 0.5f, screenHeight_ * 0.5f});
    storyPlayer_.SetColor(PURPLE);
    screen_ = AppScreen::StoryGameplay;
    SetStatus("Sandbox scene loaded. Press Pause to open the menu.", 2.6f);
  }

  bool IsPauseRequested() const {
    return inputBindings_.IsActionPressed("pause");
  }

  void OpenPauseMenu() {
    pauseMenuOpen_ = true;
    timeController_.SetPaused(true);
  }

  void ClosePauseMenu() {
    pauseMenuOpen_ = false;
    timeController_.SetPaused(false);
  }

  void Update(Vector2 mousePos, bool leftClickPressed) {
    int cursor = MOUSE_CURSOR_DEFAULT;

    switch (screen_) {
    case AppScreen::MainMenu:
      UpdateMainMenu(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::NewGameMenu:
      UpdateNewGameMenu(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::SandboxName:
      UpdateSandboxName(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::SandboxOrigin:
      UpdateSandboxOrigin(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::SandboxAppearance:
      UpdateSandboxAppearance(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::SandboxTraits:
      UpdateSandboxTraits(mousePos, leftClickPressed, cursor);
      break;
    case AppScreen::StoryGameplay:
      UpdateStoryGameplay(mousePos, leftClickPressed, cursor);
      break;
    }

    SetMouseCursor(cursor);
  }

  void UpdateMainMenu(Vector2 mousePos, bool leftClickPressed, int &cursor) {
    const bool newGameHover = ui::IsHovered(mainButtons_[0], mousePos);
    const bool loadGameHover = ui::IsHovered(mainButtons_[1], mousePos);
    const bool optionsHover = ui::IsHovered(mainButtons_[2], mousePos);
    const bool exitHover = ui::IsHovered(mainButtons_[3], mousePos);
    const bool projectUrlHover =
        CheckCollisionPointRec(mousePos, projectUrlRect_);
    if (newGameHover || loadGameHover || optionsHover || exitHover ||
        projectUrlHover) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (newGameHover) {
      screen_ = AppScreen::NewGameMenu;
      return;
    }
    if (loadGameHover) {
      SetStatus("Load Game is in progress. Stay tuned!", 2.8f);
      return;
    }
    if (optionsHover) {
      SetStatus("Options are in progress. Stay tuned!", 2.8f);
      return;
    }
    if (exitHover) {
      running_ = false;
      return;
    }
    if (projectUrlHover) {
      OpenURL(projectUrl_);
      SetStatus("Opening project page in your browser...", 2.2f);
    }
  }

  void UpdateNewGameMenu(Vector2 mousePos, bool leftClickPressed, int &cursor) {
    const bool storyHover = ui::IsHovered(newGameButtons_[0], mousePos);
    const bool sandboxHover = ui::IsHovered(newGameButtons_[1], mousePos);
    const bool backHover = ui::IsHovered(newGameButtons_[2], mousePos);
    if (storyHover || sandboxHover || backHover) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (storyHover) {
      StartStoryMode();
      return;
    }
    if (sandboxHover) {
      ResetSandboxDraft();
      screen_ = AppScreen::SandboxName;
      return;
    }
    if (backHover) {
      screen_ = AppScreen::MainMenu;
    }
  }

  void UpdateSandboxName(Vector2 mousePos, bool leftClickPressed, int &cursor) {
    ui::UpdateTextInput(nameInput_, mousePos, leftClickPressed);
    if (CheckCollisionPointRec(mousePos, nameInput_.bounds)) {
      cursor = MOUSE_CURSOR_IBEAM;
    }
    const bool nextHover = ui::IsHovered(nameNextButton_, mousePos);
    const bool backHover = ui::IsHovered(nameBackButton_, mousePos);
    if (nextHover || backHover) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (backHover) {
      screen_ = AppScreen::MainMenu;
      return;
    }
    if (nextHover) {
      if (nameInput_.value.empty()) {
        SetStatus("Type a character name to continue.", 2.3f);
      } else {
        screen_ = AppScreen::SandboxOrigin;
      }
    }
  }

  void UpdateSandboxOrigin(Vector2 mousePos, bool leftClickPressed,
                           int &cursor) {
    for (size_t i = 0; i < originButtons_.size(); ++i) {
      if (ui::IsHovered(originButtons_[i], mousePos)) {
        cursor = MOUSE_CURSOR_POINTING_HAND;
      }
      if (leftClickPressed &&
          ui::IsClicked(originButtons_[i], mousePos, leftClickPressed)) {
        selectedOrigin_ = static_cast<int>(i);
      }
    }

    if (ui::IsHovered(stepBackButton_, mousePos) ||
        ui::IsHovered(stepNextButton_, mousePos)) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (ui::IsClicked(stepBackButton_, mousePos, leftClickPressed)) {
      screen_ = AppScreen::MainMenu;
      return;
    }
    if (ui::IsClicked(stepNextButton_, mousePos, leftClickPressed)) {
      screen_ = AppScreen::SandboxAppearance;
    }
  }

  void UpdateSandboxAppearance(Vector2 mousePos, bool leftClickPressed,
                               int &cursor) {
    for (size_t i = 0; i < appearanceButtons_.size(); ++i) {
      if (ui::IsHovered(appearanceButtons_[i], mousePos)) {
        cursor = MOUSE_CURSOR_POINTING_HAND;
      }
      if (leftClickPressed &&
          ui::IsClicked(appearanceButtons_[i], mousePos, leftClickPressed)) {
        selectedAppearance_ = static_cast<int>(i);
      }
    }

    if (ui::IsHovered(stepBackButton_, mousePos) ||
        ui::IsHovered(stepNextButton_, mousePos)) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (ui::IsClicked(stepBackButton_, mousePos, leftClickPressed)) {
      screen_ = AppScreen::MainMenu;
      return;
    }
    if (ui::IsClicked(stepNextButton_, mousePos, leftClickPressed)) {
      screen_ = AppScreen::SandboxTraits;
    }
  }

  void UpdateSandboxTraits(Vector2 mousePos, bool leftClickPressed,
                           int &cursor) {
    for (size_t i = 0; i < traitButtons_.size(); ++i) {
      if (ui::IsHovered(traitButtons_[i], mousePos)) {
        cursor = MOUSE_CURSOR_POINTING_HAND;
      }
      if (ui::IsClicked(traitButtons_[i], mousePos, leftClickPressed)) {
        selectedTraits_[i] = !selectedTraits_[i];
      }
    }

    if (ui::IsHovered(stepBackButton_, mousePos) ||
        ui::IsHovered(finishButton_, mousePos)) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (ui::IsClicked(stepBackButton_, mousePos, leftClickPressed)) {
      screen_ = AppScreen::MainMenu;
      return;
    }
    if (ui::IsClicked(finishButton_, mousePos, leftClickPressed)) {
      if (!AnyTraitSelected()) {
        SetStatus("Choose at least one starting trait.", 2.3f);
        return;
      }

      StartSandboxMode();
    }
  }

  void UpdatePauseMenu(Vector2 mousePos, bool leftClickPressed, int &cursor) {
    const bool resumeHover = ui::IsHovered(pauseButtons_[0], mousePos);
    const bool optionsHover = ui::IsHovered(pauseButtons_[1], mousePos);
    const bool mainMenuHover = ui::IsHovered(pauseButtons_[2], mousePos);
    const bool exitHover = ui::IsHovered(pauseButtons_[3], mousePos);
    if (resumeHover || optionsHover || mainMenuHover || exitHover) {
      cursor = MOUSE_CURSOR_POINTING_HAND;
    }

    if (!leftClickPressed) {
      return;
    }

    if (resumeHover) {
      ClosePauseMenu();
      return;
    }
    if (optionsHover) {
      SetStatus("Pause options are in progress.", 2.2f);
      return;
    }
    if (mainMenuHover) {
      ClosePauseMenu();
      screen_ = AppScreen::MainMenu;
      return;
    }
    if (exitHover) {
      running_ = false;
    }
  }

  void UpdateStoryGameplay(Vector2 mousePos, bool leftClickPressed,
                           int &cursor) {
    if (pauseMenuOpen_) {
      UpdatePauseMenu(mousePos, leftClickPressed, cursor);
      if (IsPauseRequested()) {
        ClosePauseMenu();
      }
      return;
    }

    if (IsPauseRequested()) {
      OpenPauseMenu();
      return;
    }

    // Input comes from JSON bindings so keyboard/pad mapping is editable
    // without touching this gameplay logic.
    Vector2 moveAxis = {inputBindings_.GetAxis("move_x"),
                        inputBindings_.GetAxis("move_y")};
    // Movement uses game-time delta so pause freezes simulation consistently.
    storyPlayer_.Update(timeController_.DeltaTime(), moveAxis);
  }

  void DrawTitle(const char *titleText) const {
    constexpr int titleFontSize = 56;
    const int titleWidth = MeasureText(titleText, titleFontSize);
    DrawText(titleText, (screenWidth_ - titleWidth) / 2, 86, titleFontSize,
             RAYWHITE);
  }

  void DrawCenteredText(const char *text, int y, int fontSize,
                        Color color) const {
    const int width = MeasureText(text, fontSize);
    DrawText(text, (screenWidth_ - width) / 2, y, fontSize, color);
  }

  void DrawMainMenu(Vector2 mousePos) const {
    DrawTitle("RPG CPP Remake");
    for (const auto &button : mainButtons_) {
      ui::DrawButton(button, ui::IsHovered(button, mousePos));
    }

    DrawRectangleRounded(infoPanel_, 0.18f, 8, Color{82, 82, 82, 220});
    DrawRectangleRoundedLinesEx(infoPanel_, 0.18f, 8, 2.0f,
                                Color{185, 185, 185, 230});
    DrawText("This is a very early build of the game.",
             static_cast<int>(infoPanel_.x) + 18,
             static_cast<int>(infoPanel_.y) + 18, 24, RAYWHITE);
    DrawText("Please consider starring/following the project:",
             static_cast<int>(infoPanel_.x) + 18,
             static_cast<int>(infoPanel_.y) + 50, 20,
             Color{232, 232, 232, 255});
    const bool projectUrlHover =
        CheckCollisionPointRec(mousePos, projectUrlRect_);
    const Color linkColor =
        projectUrlHover ? Color{170, 215, 255, 255} : SKYBLUE;
    DrawText(projectUrl_, linkX_, linkY_, linkFontSize_, linkColor);
    DrawLine(linkX_, linkY_ + linkFontSize_ + 1, linkX_ + linkWidth_,
             linkY_ + linkFontSize_ + 1, linkColor);
  }

  void DrawNewGameMenu(Vector2 mousePos) const {
    DrawTitle("New Game");
    DrawCenteredText("Select game mode", 176, 28, RAYWHITE);
    for (const auto &button : newGameButtons_) {
      ui::DrawButton(button, ui::IsHovered(button, mousePos));
    }
  }

  void DrawSandboxName(Vector2 mousePos) const {
    DrawTitle("Sandbox - Character Name");
    DrawCenteredText("Type your character name", 246, 28, RAYWHITE);
    ui::DrawTextInput(nameInput_, "e.g. Arin");
    ui::DrawButton(nameNextButton_, ui::IsHovered(nameNextButton_, mousePos));
    ui::DrawButton(nameBackButton_, ui::IsHovered(nameBackButton_, mousePos));
  }

  void DrawSandboxOrigin(Vector2 mousePos) const {
    DrawTitle("Sandbox - Origin");
    DrawCenteredText("Choose your starting origin", 176, 28, RAYWHITE);
    for (size_t i = 0; i < originButtons_.size(); ++i) {
      ui::DrawButton(originButtons_[i],
                     ui::IsHovered(originButtons_[i], mousePos),
                     selectedOrigin_ == static_cast<int>(i));
    }
    ui::DrawButton(stepBackButton_, ui::IsHovered(stepBackButton_, mousePos));
    ui::DrawButton(stepNextButton_, ui::IsHovered(stepNextButton_, mousePos));
  }

  void DrawSandboxAppearance(Vector2 mousePos) const {
    DrawTitle("Sandbox - Appearance");
    DrawCenteredText("Choose your appearance", 176, 28, RAYWHITE);
    for (size_t i = 0; i < appearanceButtons_.size(); ++i) {
      ui::DrawButton(appearanceButtons_[i],
                     ui::IsHovered(appearanceButtons_[i], mousePos),
                     selectedAppearance_ == static_cast<int>(i));
    }
    ui::DrawButton(stepBackButton_, ui::IsHovered(stepBackButton_, mousePos));
    ui::DrawButton(stepNextButton_, ui::IsHovered(stepNextButton_, mousePos));
  }

  void DrawSandboxTraits(Vector2 mousePos) const {
    DrawTitle("Sandbox - Traits");
    DrawCenteredText("Choose your starting traits", 176, 28, RAYWHITE);
    for (size_t i = 0; i < traitButtons_.size(); ++i) {
      ui::DrawButton(traitButtons_[i],
                     ui::IsHovered(traitButtons_[i], mousePos),
                     selectedTraits_[i]);
    }
    ui::DrawButton(stepBackButton_, ui::IsHovered(stepBackButton_, mousePos));
    ui::DrawButton(finishButton_, ui::IsHovered(finishButton_, mousePos));
  }

  void DrawStoryGameplay() const {
    const char *modeTitle = sandboxGameplayActive_ ? "Sandbox placeholder scene"
                                                   : "Story placeholder scene";
    DrawCenteredText(modeTitle, 26, 28, RAYWHITE);
    DrawCenteredText("Move with configured keyboard/gamepad controls", 58, 22,
                     Color{200, 235, 200, 255});
    DrawCenteredText("Use configured Pause action to open menu", 84, 20,
                     Color{200, 235, 200, 255});
    storyPlayer_.Draw();
  }

  void DrawPauseMenu(Vector2 mousePos) const {
    DrawRectangle(0, 0, screenWidth_, screenHeight_, Color{0, 0, 0, 145});
    DrawCenteredText("Paused", 160, 52, RAYWHITE);
    for (const auto &button : pauseButtons_) {
      ui::DrawButton(button, ui::IsHovered(button, mousePos));
    }
  }

  void Draw(Vector2 mousePos) {
    BeginDrawing();
    ClearBackground(DARKGREEN);

    switch (screen_) {
    case AppScreen::MainMenu:
      DrawMainMenu(mousePos);
      break;
    case AppScreen::NewGameMenu:
      DrawNewGameMenu(mousePos);
      break;
    case AppScreen::SandboxName:
      DrawSandboxName(mousePos);
      break;
    case AppScreen::SandboxOrigin:
      DrawSandboxOrigin(mousePos);
      break;
    case AppScreen::SandboxAppearance:
      DrawSandboxAppearance(mousePos);
      break;
    case AppScreen::SandboxTraits:
      DrawSandboxTraits(mousePos);
      break;
    case AppScreen::StoryGameplay:
      DrawStoryGameplay();
      if (pauseMenuOpen_) {
        DrawPauseMenu(mousePos);
      }
      break;
    }

    if (statusMessageTime_ > 0.0f) {
      DrawText(statusMessage_.c_str(), 24, 20, 24, GOLD);
    }

    EndDrawing();
  }
};

} // namespace

int RunGame() {
  GameApp app;
  return app.Run();
}

} // namespace game
