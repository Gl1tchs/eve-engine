// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch.h"

#include "launch/prelude.h"

#include "project/project.h"
#include "scene/scene_manager.h"
#include "scripting/script_engine.h"

#include "layers/editor_layer.h"

namespace eve {
class EditorInstance : public Instance {
 public:
  EditorInstance(const InstanceSpecifications& specs) : Instance(specs) {
    // If default project provided load it.
    if (Ref<Project> project = Project::GetActive(); project) {
      ScriptEngine::Init();
      SceneManager::SetActive(0);
    }

    PushLayer<EditorLayer>(GetState());
  }
};

Instance* CreateInstance(CommandLineArguments args) {
  InstanceSpecifications specs;
  specs.name = "Eve Editor";
  specs.description = "Editor application for the eve engine.";
  specs.args = args;

  if (args.argc == 2) {
    std::string startup_project = args.argv[1];
    if (!startup_project.empty()) {
      if (std::isspace(startup_project[0])) {
        startup_project.erase(0, startup_project.find_first_not_of(" "));
      }

      Project::Load(startup_project);
    }
  }

  return new EditorInstance(specs);
}
}  // namespace eve
