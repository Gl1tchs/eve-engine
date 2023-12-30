// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "launch/prelude.h"

#include <cxxopts.hpp>

#include "project/project.h"
#include "scene/scene_manager.h"
#include "scripting/script_engine.h"

#include "runtime_layer.h"

namespace eve {
struct RuntimeArgumentPacket {
  std::optional<std::string> startup_project;
};

std::optional<RuntimeArgumentPacket> ParseArgs(
    const CommandLineArguments& args);

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

  std::optional<RuntimeArgumentPacket> packet = ParseArgs(args);
  if (!packet.has_value()) {
    return nullptr;
  }

  std::optional<std::string> project_path = packet.value().startup_project;
  if (!project_path.has_value()) {
    return nullptr;
  }

  if (std::isspace(project_path.value()[0])) {
    project_path.value().erase(0, project_path.value().find_first_not_of(" "));
  }

  Ref<Project> project = Project::Load(project_path.value());
  if (!project) {
    return nullptr;
  }

  return new EditorInstance(specs);
}

std::optional<RuntimeArgumentPacket> ParseArgs(
    const CommandLineArguments& args) {
  cxxopts::Options options("Eve Runtime",
                           "The eve engine runtime application.");

  options.add_options()("p,project", "Default project to start editor",
                        cxxopts::value<std::string>())("h,help", "Print usage");

  auto result = options.parse(args.argc, args.argv);

  if (result.count("help")) {
    LOG_INFO("{}", options.help());
    return {};
  }

  RuntimeArgumentPacket packet;

  if (result.count("project")) {
    packet.startup_project = result["project"].as<std::string>();
  }

  return packet;
}
}  // namespace eve
