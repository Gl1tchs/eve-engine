// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch.h"

#include "launch/prelude.h"

#include <cxxopts.hpp>

#include "project/project.h"
#include "scene/scene_manager.h"
#include "scripting/script_engine.h"

#include "layers/editor_layer.h"

struct EditorArgumentPacket {
  std::optional<std::string> startup_project;
};

std::optional<EditorArgumentPacket> ParseArgs(const CommandLineArguments& args);

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

  std::optional<EditorArgumentPacket> packet = ParseArgs(args);
  if (packet.has_value() && packet->startup_project.has_value()) {
    if (std::isspace(packet->startup_project.value()[0])) {
      packet->startup_project.value().erase(
          0, packet->startup_project.value().find_first_not_of(" "));
    }

    Project::Load(packet->startup_project.value());
  }

  return new EditorInstance(specs);
}

std::optional<EditorArgumentPacket> ParseArgs(
    const CommandLineArguments& args) {
  cxxopts::Options options("Eve Editor", "The eve engine editor application.");

  options.add_options()("p,project", "Default project to start editor",
                        cxxopts::value<std::string>())("h,help", "Print usage");

  auto result = options.parse(args.argc, args.argv);

  if (result.count("help")) {
    LOG_INFO("{}", options.help());
    return {};
  }

  EditorArgumentPacket packet;

  if (result.count("project")) {
    packet.startup_project = result["project"].as<std::string>();
  }

  return packet;
}
