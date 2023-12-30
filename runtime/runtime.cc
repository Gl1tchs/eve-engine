// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "launch/prelude.h"

#include "project/project.h"
#include "scene/scene_manager.h"
#include "scripting/script_engine.h"

#include "runtime_layer.h"

namespace eve {
class EditorInstance : public Instance {
 public:
  EditorInstance(const InstanceSpecifications& specs) : Instance(specs) {
    ScriptEngine::Init(true);
    SceneManager::SetActive(0);

    PushLayer<RuntimeLayer>(GetState());
  }
};

Instance* CreateInstance(CommandLineArguments args) {
  InstanceSpecifications specs;
  specs.name = "Eve Runtime";
  specs.description = "Runtime application for the eve engine.";
  specs.args = args;

  if (args.argc == 1 || args.argc > 2) {
    return nullptr;
  }

  std::string project_path = args.argv[1];
  if (project_path.empty()) {
    return nullptr;
  }

  if (std::isspace(project_path[0])) {
    project_path.erase(0, project_path.find_first_not_of(" "));
  }

  Ref<Project> project = Project::Load(project_path);
  if (!project) {
    return nullptr;
  }

  return new EditorInstance(specs);
}
}  // namespace eve
