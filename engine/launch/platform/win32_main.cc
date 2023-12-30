#include "launch/platform/win32_main.h"

#include "core/core_minimal.h"

namespace eve {
extern int GuardedMain(CommandLineArguments args);
}

#ifdef _DEBUG
int main(int argc, char* argv[]) {
  return eve::GuardedMain({argc, argv});
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
  return eve::GuardedMain({__argc, __argv});
}
#endif
