#include "runtime/launch/linux_main.h"

#include "core/core_minimal.h"

extern int GuardedMain(CommandLineArguments args);

int main(int argc, const char* argv[]) {
  return GuardedMain({argc, argv});
}
