#include "launch/platform/win32_main.h"

#include "core/core_minimal.h"

namespace eve {
extern int GuardedMain(CommandLineArguments args);
}

int main(int argc, char* argv[]) {
  return eve::GuardedMain({argc, argv});
}
