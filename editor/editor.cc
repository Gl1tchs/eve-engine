// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "runtime/prelude.h"

#include <cxxopts.hpp>

#include "core/debug/log.h"
#include "graphics/graphics.h"
#include "graphics/shader_compiler.h"
#include "project/project.h"

#include "layers/editor_layer.h"

void CompileAndSaveShader(const std::filesystem::path& path,
                          const std::filesystem::path& spirv_dir);

std::filesystem::file_time_type GetLastModificationTime(
    const std::filesystem::path& path);

bool NeedsRecompilation(const std::filesystem::path& shader_path,
                        const std::filesystem::path& spirv_dir);

void CompileAndSaveShaderIfNeeded(const std::filesystem::path& shader_path,
                                  const std::filesystem::path& spirv_dir);

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

    std::filesystem::path shader_dir = "assets/shaders/";
    std::filesystem::path spirv_dir = ".eve/shaders/";
    if (!std::filesystem::exists(spirv_dir)) {
      std::filesystem::create_directories(spirv_dir);
    }

    for (const auto& entry : std::filesystem::directory_iterator(shader_dir)) {
      // Check if the file is a shader source file
      if (entry.path().extension() == ".vert" ||
          entry.path().extension() == ".frag") {
        // Compile and save the shader
        CompileAndSaveShaderIfNeeded(entry.path(), spirv_dir);
      }
    }

    PushLayer<EditorLayer>(GetState());
  }

 private:
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

void CompileAndSaveShader(const std::filesystem::path& path,
                          const std::filesystem::path& spirv_dir) {
  std::string source = LoadShaderSource(path);

  // Determine shader kind based on file extension
  ShaderKind kind;
  if (path.extension() == ".vert") {
    kind = ShaderKind::kVertex;
  } else if (path.extension() == ".frag") {
    kind = ShaderKind::kFragment;
  } else {
    // Unsupported shader type
    LOG_ENGINE_ERROR("Unsupported shader type: {0}", path.string());
    return;
  }

  // Compile shader
  std::vector<uint32_t> spirv = CompileShader(
      source, path.filename().string().c_str(), kind, GetGraphicsAPI(),
      GetGraphicsAPI() == GraphicsAPI::kOpenGL   ? "ENV_OPENGL"
      : GetGraphicsAPI() == GraphicsAPI::kVulkan ? "ENV_VULKAN"
                                                 : "");

  // Save the compiled shader to a new file
  std::filesystem::path spirv_path = spirv_dir / path.filename();
  spirv_path.replace_extension(path.extension().string() + ".spirv");

  std::ofstream output(spirv_path, std::ios::binary);
  output.write(reinterpret_cast<const char*>(spirv.data()),
               spirv.size() * sizeof(uint32_t));
  output.flush();
  output.close();
}

std::filesystem::file_time_type GetLastModificationTime(
    const std::filesystem::path& path) {
  return std::filesystem::last_write_time(path);
}

bool NeedsRecompilation(const std::filesystem::path& shader_path,
                        const std::filesystem::path& spirv_dir) {
  // Get the last modification time of the shader source file
  auto shader_time = GetLastModificationTime(shader_path);

  // Form the path to the corresponding SPIR-V file
  std::filesystem::path spirv_path = spirv_dir / shader_path.filename();
  spirv_path.replace_extension(shader_path.extension().string() + ".spirv");

  // Check if the SPIR-V file exists
  if (!std::filesystem::exists(spirv_path)) {
    return true;  // SPIR-V file does not exist, needs compilation
  }

  // Get the last modification time of the existing SPIR-V file
  auto spirv_time = GetLastModificationTime(spirv_path);

  // Check if the shader has been modified since the last compilation
  return shader_time > spirv_time;
}

void CompileAndSaveShaderIfNeeded(const std::filesystem::path& shader_path,
                                  const std::filesystem::path& spirv_dir) {
  if (NeedsRecompilation(shader_path, spirv_dir)) {
    // Shader needs recompilation
    CompileAndSaveShader(shader_path, spirv_dir);
  } else {
    LOG_ENGINE_TRACE("Shader {} is up to date!", shader_path.string());
  }
}

std::optional<EditorArgumentPacket> ParseArgs(
    const CommandLineArguments& args) {
  cxxopts::Options options("Eve Editor", "The eve engine editor application.");

  options.add_options()("p,project", "Default project to start editor",
                        cxxopts::value<std::string>())("h,help", "Print usage");

  auto result = options.parse(args.argc, args.argv);

  if (result.count("help")) {
    LOG_ENGINE_INFO("{}", options.help());
    return {};
  }

  EditorArgumentPacket packet;

  if (result.count("project")) {
    packet.startup_project = result["project"].as<std::string>();
  }

  return packet;
}
