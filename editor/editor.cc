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
  EditorInstance(const InstanceSpecifications& specs, const std::string& path)
      : Instance(specs) {
    // If default project provided load it.
    if (!path.empty()) {
      EnqueueMain([this, path] {
        if (Ref<Project> project = Project::Load(path); project) {
          ScriptEngine::Init();
          SceneManager::SetActive(0);
        } else {
          LOG_EDITOR_ERROR("Unable to open project from: {}", path);
        }
      });
    }

    PushLayer<EditorLayer>(GetState());
  }
};

Instance* CreateInstance(CommandLineArguments args) {
  InstanceSpecifications specs;
  specs.name = "Eve Editor";
  specs.description = "Editor application for the eve engine.";
  specs.args = args;

  std::string path = args.argc == 2 ? args.argv[1] : "";

  if (!path.empty()) {
    if (std::isspace(path[0])) {
      path.erase(0, path.find_first_not_of(" "));
    }
  }

  return new EditorInstance(specs, path);
}
}  // namespace eve
