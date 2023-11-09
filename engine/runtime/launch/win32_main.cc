#include "runtime/launch/win32_main.h"

extern int GuardedMain();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
  return GuardedMain();
}
