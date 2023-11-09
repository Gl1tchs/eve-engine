#include "runtime/launch/linux_main.h"

extern int GuardedMain();

int main(int argc, const char* argv[]) {
  return GuardedMain();
}
