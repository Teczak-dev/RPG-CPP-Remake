#include "game/GameApp.hpp"

int main() {
  return game::RunGame();
}

#if defined(_WIN32)
extern "C" int __stdcall WinMain(void *, void *, char *, int) {
  return game::RunGame();
}
#endif
