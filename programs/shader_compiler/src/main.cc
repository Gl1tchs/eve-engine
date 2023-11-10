// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include <fstream>
#include <iostream>

#include <cxxopts.hpp>

#include "compiler.h"

int main(int argc, char* argv[]) {
  try {
    cxxopts::Options cli_options("ShaderCompiler",
                                 "Eve engine shader compiler.");

    cli_options.add_options()("h,help", "Show help")(
        "o,output", "Output file path", cxxopts::value<std::string>())(
        "i,input", "Input file path", cxxopts::value<std::string>())(
        "d,define", "Preprocessor macros", cxxopts::value<std::string>())(
        "e,env", "Target environment", cxxopts::value<std::string>())(
        "k,kind", "Shader kind", cxxopts::value<std::string>());

    auto result = cli_options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << cli_options.help() << std::endl;
      return 0;
    }

    // Check if required arguments are provided
    if (!result.count("input") || !result.count("output") ||
        !result.count("env") || !result.count("kind")) {
      throw std::runtime_error(
          "Error: Input and output file paths are required.");
    }

    std::filesystem::path input_path = result["input"].as<std::string>();
    std::filesystem::path output_path = result["output"].as<std::string>();

    auto env = DeserializeTargetEnv(result["env"].as<std::string>());
    if (!env.has_value()) {
      throw std::runtime_error("Error: Invalid target environment.");
    }

    auto kind = DeserializeShaderKind(result["kind"].as<std::string>());
    if (!kind.has_value()) {
      throw std::runtime_error("Error: Invalid shader kind.");
    }

    // Read the input shader file
    std::string input_shader_source = LoadShaderSource(input_path);

    std::string definitions;
    if (result.count("define")) {
      definitions = result["define"].as<std::string>();
    }

    auto compiled_shader = CompileShader(
        input_shader_source, input_path.filename().string().c_str(),
        kind.value(), env.value(), definitions);
    if (compiled_shader.empty()) {
      return 1;
    }

    // Write the compiled shader to the output file
    std::ofstream output_file(output_path, std::ios::out | std::ios::binary);
    if (!output_file.is_open()) {
      throw std::runtime_error("Error: Failed to open output file.");
    }

    output_file.write(reinterpret_cast<const char*>(compiled_shader.data()),
                      compiled_shader.size() * sizeof(uint32_t));
    output_file.flush();
    output_file.close();

    std::cout << "Shader compilation successful.\n";

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
