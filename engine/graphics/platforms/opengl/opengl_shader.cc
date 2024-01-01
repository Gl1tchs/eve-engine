#include "graphics/platforms/opengl/opengl_shader.h"

#include <glad/glad.h>

namespace eve {
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

OpenGLShader::OpenGLShader(const std::string& vs_path,
                           const std::string& fs_path,
                           const std::string& custom_shader) {
  program_ = glCreateProgram();

  const std::string vertex_source = LoadShaderSource(vs_path);
  const uint32_t vertex_shader =
      CompileShader(vertex_source, ShaderType::kVertex);
  glAttachShader(program_, vertex_shader);

  const std::string fragment_source =
      LoadShaderSource(fs_path, true, custom_shader);
  const uint32_t fragment_shader =
      CompileShader(fragment_source, ShaderType::kFragment);
  glAttachShader(program_, fragment_shader);

  glLinkProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

OpenGLShader::~OpenGLShader() {
  glDeleteProgram(program_);
}

void OpenGLShader::Recompile(const std::string& vs_path,
                             const std::string& fs_path,
                             const std::string& custom_shader) {
  glDeleteProgram(program_);
  program_ = glCreateProgram();

  const std::string vertex_source = LoadShaderSource(vs_path);
  const uint32_t vertex_shader =
      CompileShader(vertex_source, ShaderType::kVertex);
  glAttachShader(program_, vertex_shader);

  const std::string fragment_source =
      LoadShaderSource(fs_path, true, custom_shader);
  const uint32_t fragment_shader =
      CompileShader(fragment_source, ShaderType::kFragment);
  glAttachShader(program_, fragment_shader);

  glLinkProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void OpenGLShader::Bind() const {
  glUseProgram(program_);
}

void OpenGLShader::Unbind() const {
  glUseProgram(0);
}

std::string OpenGLShader::LoadShaderSource(const fs::path& path,
                                           bool is_fragment,
                                           const std::string& custom_shader) {
  if (!fs::exists(path)) {
    LOG_ENGINE_ERROR("Shader file not found at: {0}", path.string());
    return "";
  }

  const std::string include_identifier = "#include ";
  const std::string begin_custom_identifier = "#pragma custom";
  static bool is_recursive_call = false;

  std::string full_source_code;
  std::ifstream file(path);

  if (!file.is_open()) {
    LOG_ENGINE_ERROR("Could not open the shader at: {0}", path.string());
    return full_source_code;
  }

  std::string line_buffer;
  while (std::getline(file, line_buffer)) {
    if (line_buffer.find(include_identifier) != std::string::npos) {
      line_buffer.erase(0, include_identifier.size());
      line_buffer.erase(0, 1);
      line_buffer.erase(line_buffer.size() - 1);

      fs::path p = path.parent_path();
      line_buffer.insert(0, p.string() + "/");

      is_recursive_call = true;
      full_source_code += LoadShaderSource(line_buffer);
      continue;
    } else if (is_fragment &&
               std::regex_search(line_buffer,
                                 std::regex(begin_custom_identifier))) {
      // Handle custom shaders
      if (custom_shader.empty() || !ParseCustomShader(custom_shader)) {
        continue;
      }

      full_source_code += "#define CUSTOM_SHADER\n";
      full_source_code += custom_shader;

      continue;
    }

    full_source_code += line_buffer + '\n';
  }

  if (!is_recursive_call)
    full_source_code += '\0';

  file.close();

  return full_source_code;
}

bool OpenGLShader::ParseCustomShader(const std::string& custom_shader) {
  const std::regex fragment_function_regex(
      R"(vec3\s+fragment\s*\(\s*vec3\s+\w+\s*\)\s*\{)");

  if (!std::regex_search(custom_shader, fragment_function_regex)) {
    return false;
  }

  const std::regex uniform_regex(R"(uniform\s+(\w+)\s+(\w+)\s*;)");

  std::sregex_iterator iter(custom_shader.begin(), custom_shader.end(),
                            uniform_regex);
  std::sregex_iterator end;

  // Clear old uniforms
  custom_uniforms_.clear();

  for (; iter != end; ++iter) {
    std::smatch match = *iter;

    if (match.size() == 3) {
      std::string uniform_type = match[1].str();
      std::string uniform_name = match[2].str();

      // Create ShaderUniform instance and add it to the vector
      ShaderUniform uniform;
      uniform.name = uniform_name;
      uniform.type = ConvertStringToShaderUniformType(uniform_type);
      uniform.value = GetDefaultShaderValue(uniform.type);

      custom_uniforms_.push_back(uniform);
    }
  }

  return !custom_uniforms_.empty();
}

int OpenGLShader::GetUniformLocation(const std::string& name) const {
  return glGetUniformLocation(program_, name.c_str());
}

void OpenGLShader::SetUniform(const std::string& name,
                              ShaderValueVariant value) const {
  std::visit(
      [&](const auto& val) {
        using ValueType = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<ValueType, float> ||
                      std::is_same_v<ValueType, glm::vec2> ||
                      std::is_same_v<ValueType, glm::vec3> ||
                      std::is_same_v<ValueType, glm::vec4> ||
                      std::is_same_v<ValueType, glm::mat3> ||
                      std::is_same_v<ValueType, glm::mat4> ||
                      std::is_same_v<ValueType, int> ||
                      std::is_same_v<ValueType, bool>) {
          SetUniform(name, val);
        }
      },
      value);
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
                              const glm::mat3& value) const {
  glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(value));
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
    LOG_ENGINE_ERROR("Unable to link shader of type: ", SerializeShaderType(type),
              "\n", info_log);
    return false;
  }

  return true;
}

uint32_t OpenGLShader::CompileShader(const std::string& source,
                                     ShaderType type) {
  ASSERT(type != ShaderType::kNone)

  const char* source_c_str = source.c_str();
  const uint32_t shader = glCreateShader(GetOpenGLShaderType(type));
  glShaderSource(shader, 1, &source_c_str, nullptr);
  glCompileShader(shader);

  if (!CheckCompileErrors(shader, type)) {
    LOG_ENGINE_ERROR("Unable to compile shader:\n{0}", source);
    ASSERT(false);
  }

  return shader;
}
}  // namespace eve