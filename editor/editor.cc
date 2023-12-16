// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "runtime/prelude.h"

#include <cxxopts.hpp>

#include "core/debug/log.h"
#include "graphics/graphics.h"
#include "project/project.h"

#include "layers/editor_layer.h"

struct EditorArgumentPacket {
  std::optional<std::string> startup_project;
};

std::optional<EditorArgumentPacket> ParseArgs(const CommandLineArguments& args);

class EditorInstance : public Instance {
 public:
  EditorInstance(const InstanceSpecifications& specs,
                 std::optional<std::string> startup_project)
      : Instance(specs) {

    if (startup_project.has_value()) {
      std::string project_path = startup_project.value();
      if (std::isspace(project_path[0])) {
        project_path.erase(0, project_path.find_first_not_of(" "));
      }

      Project::Load(project_path);
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
  if (!packet.has_value()) {
    return nullptr;
  }

  return new EditorInstance(specs, packet.value().startup_project);
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
