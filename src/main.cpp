#include "raylib.h"

int main() {
  // Rozdzielczość i tytuł okna
  const int screenWidth = 1280;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "RPG CPP Remake - Edytor");

  SetTargetFPS(60); // Ustawienie stałej liczby klatek (kluczowe w grach)

  // Główna pętla gry
  while (!WindowShouldClose()) {
    // 1. Logika (Update)
    // Tutaj będziemy dodawać obsługę klawiszy, ruch itp.

    // 2. Rysowanie (Draw)
    BeginDrawing();
    ClearBackground(RAYWHITE); // Czyszczenie tła

    DrawText("Witaj w RPG CPP Remake!", 190, 200, 20, LIGHTGRAY);
    DrawFPS(10, 10); // Licznik klatek w rogu

    EndDrawing();
  }

  // Zamknięcie okna i zwolnienie zasobów
  CloseWindow();

  return 0;
}
