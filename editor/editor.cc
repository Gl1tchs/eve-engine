// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "runtime/prelude.h"

#include "core/debug/log.h"
#include "layers/editor_layer.h"

class EditorInstance : public Instance {
 public:
  EditorInstance(const InstanceSpecifications& specs) : Instance(specs) {
    PushLayer<EditorLayer>(GetState());
  }
};

Instance* CreateInstance(CommandLineArguments args) {
  InstanceSpecifications specs;
  specs.name = "Eve Editor";
  specs.description = "Editor application for the eve engine.";
  return new EditorInstance(specs);
}
