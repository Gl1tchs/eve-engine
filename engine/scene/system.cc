// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/system.h"

namespace eve {

System::System(uint16_t run_type) : run_type_(run_type) {}

bool System::IsRuntime() {
  return (run_type_ & SystemRunType_kRuntime) != 0;
}

bool System::IsEditor() {
  return (run_type_ & SystemRunType_kEditor) != 0;
}

bool System::IsSimulation() {
  return (run_type_ & SystemRunType_kSimulation) != 0;
}

}  // namespace eve