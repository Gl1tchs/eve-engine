#include "graphics/platforms/opengl/opengl_shader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "core/debug/assert.h"

std::string SerializeShaderType(ShaderType type) {
  switch (type) {
    case ShaderType::kVertex:
      return "vertex";
    case ShaderType::kFragment:
      return "fragment";
    case ShaderType::kGeometry:
      return "geometry";
    default:
      return "";
  }
}

ShaderType DeserializeShaderType(const std::string& value) {
  if (value == "vertex")
    return ShaderType::kVertex;
  else if (value == "fragment")
    return ShaderType::kFragment;
  else if (value == "geometry")
    return ShaderType::kGeometry;
  else
    return ShaderType::kNone;
}

int GetOpenGLShaderType(ShaderType type) {
  switch (type) {
    case ShaderType::kVertex:
      return GL_VERTEX_SHADER;
    case ShaderType::kFragment:
      return GL_FRAGMENT_SHADER;
    case ShaderType::kGeometry:
      return GL_GEOMETRY_SHADER;
    default:
      return 0;
  }
}

OpenGLShader::OpenGLShader(const std::string& vs_source,
                           const std::string& fs_source,
                           const std::string& gs_source) {
  program_ = glCreateProgram();

  const uint32_t vertex_shader = CompileShader(vs_source, ShaderType::kVertex);
  glAttachShader(program_, vertex_shader);

  const uint32_t fragment_shader =
      CompileShader(fs_source, ShaderType::kFragment);
  glAttachShader(program_, fragment_shader);

  uint32_t geometry_shader{0};
  if (!gs_source.empty()) {
    geometry_shader = CompileShader(gs_source, ShaderType::kGeometry);
    glAttachShader(program_, geometry_shader);
  }

  glLinkProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  if (geometry_shader != 0) {
    glDeleteShader(geometry_shader);
  }
}

OpenGLShader::OpenGLShader(const std::filesystem::path& vs_path,
                           const std::filesystem::path& fs_path,
                           const std::filesystem::path& gs_path) {
  program_ = glCreateProgram();

  // Load Vulkan SPIR-V shader files
  const std::vector<uint32_t> vertex_spirv = LoadSPIRV(vs_path);
  const std::vector<uint32_t> fragment_spirv = LoadSPIRV(fs_path);
  const std::vector<uint32_t> geometry_spirv =
      (!gs_path.empty()) ? LoadSPIRV(gs_path) : std::vector<uint32_t>();

  uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderBinary(1, &vertex_shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
                 vertex_spirv.data(), vertex_spirv.size() * sizeof(uint32_t));
  glSpecializeShaderARB(vertex_shader, "main", 0, nullptr, nullptr);
  glAttachShader(program_, vertex_shader);

  uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderBinary(1, &fragment_shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
                 fragment_spirv.data(),
                 fragment_spirv.size() * sizeof(uint32_t));
  glSpecializeShaderARB(fragment_shader, "main", 0, nullptr, nullptr);
  glAttachShader(program_, fragment_shader);

  uint32_t geometry_shader = 0;
  if (!gs_path.empty()) {
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderBinary(1, &geometry_shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
                   geometry_spirv.data(),
                   geometry_spirv.size() * sizeof(uint32_t));
    glSpecializeShaderARB(geometry_shader, "main", 0, nullptr, nullptr);
    glAttachShader(program_, geometry_shader);
  }

  glLinkProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  if (geometry_shader != 0) {
    glDeleteShader(geometry_shader);
  }
}

OpenGLShader::~OpenGLShader() {
  glDeleteProgram(program_);
}

void OpenGLShader::Bind() const {
  glUseProgram(program_);
}

void OpenGLShader::Unbind() const {
  glUseProgram(0);
}

std::vector<uint32_t> OpenGLShader::LoadSPIRV(
    const std::filesystem::path& path) {
  if (!std::filesystem::exists(path)) {
    LOG_ENGINE_ERROR("Shader file not found at: {0}", path.string());
    return {};
  }

  std::ifstream file(path, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    LOG_ENGINE_ERROR("Failed to open SPIR-V file: {0}", path.string());
    return {};
  }

  size_t file_size = static_cast<size_t>(file.tellg());
  std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()), file_size);
  file.close();

  return buffer;
}

int OpenGLShader::GetUniformLocation(const std::string& name) const {
  return glGetUniformLocation(program_, name.c_str());
}

void OpenGLShader::SetUniform(const std::string& name, const int value) const {
  glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform(const std::string& name,
                              const float value) const {
  glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform(const std::string& name,
                              const glm::vec2 value) const {
  glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetUniform(const std::string& name,
                              const glm::vec3 value) const {
  glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetUniform(const std::string& name,
                              const glm::vec4 value) const {
  glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetUniform(const std::string& name,
                              const glm::mat4& value) const {
  glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(value));
}

void OpenGLShader::SetUniform(const std::string& name, int count,
                              int* value) const {
  glUniform1iv(GetUniformLocation(name), count, value);
}

void OpenGLShader::SetUniform(const std::string& name, int count,
                              float* value) const {
  glUniform1fv(GetUniformLocation(name), count, value);
}

bool OpenGLShader::CheckCompileErrors(const uint32_t shader,
                                      const ShaderType type) {
  int success;
  char info_log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    LOG_ENGINE_ERROR("Unable to link shader of type: ",
                     SerializeShaderType(type), "\n", info_log);
    return false;
  }

  return true;
}

uint32_t OpenGLShader::CompileShader(const std::string& source,
                                     ShaderType type) {
  ENGINE_ASSERT(type != ShaderType::kNone)

  const char* source_c_str = source.c_str();
  const uint32_t shader = glCreateShader(GetOpenGLShaderType(type));
  glShaderSource(shader, 1, &source_c_str, nullptr);
  glCompileShader(shader);

  ENGINE_ASSERT(CheckCompileErrors(shader, type))

  return shader;
}
