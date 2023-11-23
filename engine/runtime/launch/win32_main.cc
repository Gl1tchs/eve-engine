#include "runtime/launch/win32_main.h"

#include "core/core_minimal.h"

extern int GuardedMain(CommandLineArguments args);

#ifdef EVE_DIST
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
  return GuardedMain({__argc, __argv});
}
#else
int main(int argc, char* argv[]) {
  return GuardedMain({argc, argv});
}
#endif
