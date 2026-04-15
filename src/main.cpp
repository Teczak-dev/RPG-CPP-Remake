#include "raylib.h"

#include <string>

static int RunGame() {
  const int screenWidth = 1280;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "RPG CPP Remake");
  SetTargetFPS(60);

  const int buttonWidth = 280;
  const int buttonHeight = 58;
  const int buttonSpacing = 18;
  const int firstButtonY = 230;
  const int buttonsX = (screenWidth - buttonWidth) / 2;

  const Rectangle newGameButton = {(float)buttonsX, (float)firstButtonY, (float)buttonWidth, (float)buttonHeight};
  const Rectangle loadGameButton = {(float)buttonsX, (float)(firstButtonY + (buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight};
  const Rectangle optionsButton = {(float)buttonsX, (float)(firstButtonY + 2 * (buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight};
  const Rectangle exitButton = {(float)buttonsX, (float)(firstButtonY + 3 * (buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight};

  const Rectangle infoPanel = {24.0f, (float)(screenHeight - 146), 690.0f, 118.0f};
  const char *projectUrl = "https://github.com/Teczak-dev/RPG-CPP-Remake";
  const int linkFontSize = 20;
  const int linkX = (int)infoPanel.x + 18;
  const int linkY = (int)infoPanel.y + 78;
  const int linkWidth = MeasureText(projectUrl, linkFontSize);
  const Rectangle projectUrlRect = {(float)linkX, (float)linkY, (float)linkWidth, (float)linkFontSize};

  std::string statusMessage = "Click a button to start.";
  float statusMessageTime = 0.0f;
  bool running = true;

  while (running && !WindowShouldClose()) {
    const Vector2 mousePos = GetMousePosition();
    statusMessageTime -= GetFrameTime();

    const bool newGameHover = CheckCollisionPointRec(mousePos, newGameButton);
    const bool loadGameHover = CheckCollisionPointRec(mousePos, loadGameButton);
    const bool optionsHover = CheckCollisionPointRec(mousePos, optionsButton);
    const bool exitHover = CheckCollisionPointRec(mousePos, exitButton);
    const bool projectUrlHover = CheckCollisionPointRec(mousePos, projectUrlRect);

    if (newGameHover || loadGameHover || optionsHover || exitHover || projectUrlHover) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (newGameHover) {
        statusMessage = "New Game is in progress. Stay tuned!";
        statusMessageTime = 2.8f;
      } else if (loadGameHover) {
        statusMessage = "Load Game is in progress. Stay tuned!";
        statusMessageTime = 2.8f;
      } else if (optionsHover) {
        statusMessage = "Options are in progress. Stay tuned!";
        statusMessageTime = 2.8f;
      } else if (exitHover) {
        running = false;
      } else if (projectUrlHover) {
        OpenURL(projectUrl);
        statusMessage = "Opening project page in your browser...";
        statusMessageTime = 2.2f;
      }
    }

    BeginDrawing();
    ClearBackground(DARKGREEN);

    const char *title = "RPG CPP Remake";
    const int titleFontSize = 64;
    const int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (screenWidth - titleWidth) / 2, 96, titleFontSize, RAYWHITE);

    const auto drawButton = [&](const Rectangle &button, const char *label, bool hovered) {
      const Color fill = hovered ? Color{255, 255, 255, 235} : Color{238, 243, 238, 225};
      DrawRectangleRounded(button, 0.25f, 8, fill);
      DrawRectangleRoundedLinesEx(button, 0.25f, 8, 2.0f, Color{38, 69, 38, 255});
      const int labelFontSize = 26;
      const int labelWidth = MeasureText(label, labelFontSize);
      DrawText(label, (int)(button.x + (button.width - labelWidth) / 2), (int)(button.y + 15), labelFontSize, Color{26, 49, 26, 255});
    };

    drawButton(newGameButton, "New Game", newGameHover);
    drawButton(loadGameButton, "Load Game", loadGameHover);
    drawButton(optionsButton, "Options", optionsHover);
    drawButton(exitButton, "Exit", exitHover);

    DrawRectangleRounded(infoPanel, 0.18f, 8, Color{82, 82, 82, 220});
    DrawRectangleRoundedLinesEx(infoPanel, 0.18f, 8, 2.0f, Color{185, 185, 185, 230});
    DrawText("This is a very early build of the game.", (int)infoPanel.x + 18, (int)infoPanel.y + 18, 24, RAYWHITE);
    DrawText("Please consider starring/following the project:", (int)infoPanel.x + 18, (int)infoPanel.y + 50, 20, Color{232, 232, 232, 255});
    const Color linkColor = projectUrlHover ? Color{170, 215, 255, 255} : SKYBLUE;
    DrawText(projectUrl, linkX, linkY, linkFontSize, linkColor);
    DrawLine(linkX, linkY + linkFontSize + 1, linkX + linkWidth, linkY + linkFontSize + 1, linkColor);

    if (statusMessageTime > 0.0f) {
      DrawText(statusMessage.c_str(), 24, 20, 24, GOLD);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

int main() {
  return RunGame();
}

#if defined(_WIN32)
extern "C" int __stdcall WinMain(void *, void *, char *, int) {
  return RunGame();
}
#endif
