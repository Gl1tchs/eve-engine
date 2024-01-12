// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "ui_layer.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>

#if _WIN32
#include <UserEnv.h>
#include <Windows.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "ui/imgui_utils.h"

namespace fs = std::filesystem;

static std::string GetUserDirectory() {
#ifdef _WIN32
  const char* user_profile = getenv("USERPROFILE");
  return user_profile != nullptr ? user_profile : "";
#else
  const char* home_dir = getenv("HOME");
  if (home_dir == nullptr) {
    // If HOME environment variable is not set, try to retrieve it using getpwuid
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
      home_dir = pw->pw_dir;
    }
  }

  return home_dir != nullptr ? home_dir : "";
#endif
}

UiLayer::UiLayer(eve::Ref<eve::State> state)
    : eve::Layer(state),
      project_name_("My Eve Project"),
      project_directory_(GetUserDirectory()) {

  selected_info_.type = ProjectTemplate::kNone;

  {
    ProjectTemplateInfo info;
    info.type = ProjectTemplate::kBlank;
    info.name = "Blank";
    info.path = "templates/T_Blank";

    templates_.push_back(info);
  }
}

void UiLayer::OnStart() {}

void UiLayer::OnDestroy() {}

void UiLayer::OnGUI(float ds) {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
  ImGui::Begin("##main_window", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

  const ImVec2& window_size = ImGui::GetWindowSize();

  const float content_width = window_size.x * 0.65f;

  ImGui::BeginChild("Content", ImVec2(content_width, 0), true);
  {
    ImGui::SeparatorText("Content");

    const float panel_width = ImGui::GetContentRegionAvail().x;

    static constexpr uint32_t cell_size = 100;

    const uint32_t template_count = templates_.size();
    const uint32_t column_count =
        static_cast<uint32_t>(panel_width / cell_size);

    ImGui::Columns(column_count, 0, false);

    for (const ProjectTemplateInfo& info : templates_) {
      const bool selected = selected_info_.type == info.type;

      glm::vec4 button_active_color;
      static_assert(sizeof(glm::vec4) == sizeof(ImVec4));
      memcpy(&button_active_color,
             &ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered],
             sizeof(glm::vec4));

      ImGui::ScopedStyleColor _color(ImGuiCol_Button, button_active_color,
                                     selected);

      if (ImGui::Button(info.name.c_str(), ImVec2(cell_size, cell_size))) {
        selected_info_ = info;
      }
    }

    ImGui::Columns();
  }
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Options", ImVec2(0, 0), true);
  {
    ImGui::SeparatorText("Options");

    if (selected_info_.type != ProjectTemplate::kNone) {

      ImGui::InputText("Name", &project_name_);
      ImGui::InputText("Directory", &project_directory_);

      if (ImGui::Button("Create", ImVec2(-1, 0))) {
        if (fs::exists(GetProjectPath())) {
          modal_state_ = ModalState::kProjectExists;
        } else if (CreateProject()) {
          modal_state_ = ModalState::kProjectSuccess;
        } else {
          modal_state_ = ModalState::kProjectFailed;
        }
      }
    }
  }
  ImGui::EndChild();

  ImGui::End();

  switch (modal_state_) {
    case ModalState::kProjectSuccess:
      RenderProjectSuccessPopup();
      break;
    case ModalState::kProjectFailed:
      RenderProjectFailedPopup();
      break;
    case ModalState::kProjectExists:
      RenderProjectExistsPopup();
      break;
    case ModalState::kEditorFailed:
      RenderEditorFailedPopup();
      break;
    default:
      break;
  }
}

bool UiLayer::CreateProject() {
  if (project_name_.empty() || project_directory_.empty()) {
    std::cerr << "Invalid project name or path." << std::endl;
    return false;
  }

  if (!fs::exists(selected_info_.path)) {
    std::cerr << "Source path does not exist." << std::endl;
    return false;
  }

  fs::path project_path = fs::path(project_directory_) / project_name_;

  try {
    fs::create_directories(project_path);
    fs::copy(selected_info_.path, project_path, fs::copy_options::recursive);
  } catch (const fs::filesystem_error& e) {
    return false;
  }

  fs::path project_file_path =
      project_path / std::format("{}.eproj", project_name_);

  fs::rename(project_path / "{{project_name}}.eproj", project_file_path);

  std::ifstream project_in(project_file_path);
  if (!project_in.is_open()) {
    return false;
  }

  std::ostringstream text;
  text << project_in.rdbuf();

  std::string content = text.str();

  // Replace project name
  const std::string name_key = "{{project_name}}";
  size_t pos = content.find(name_key);
  content.replace(pos, name_key.length(), project_name_);

  project_in.close();

  // Write out the new data
  std::ofstream project_out(project_file_path);
  if (!project_out.is_open()) {
    return false;
  }

  project_out << content;

  return true;
}

void UiLayer::RenderProjectFailedPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("ProjectFailedModal", &open,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Unable to create project on specified path: %s",
                project_directory_.c_str());
    if (ImGui::Button("Close", ImVec2(-1, 0))) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("ProjectFailedModal");
}

void UiLayer::RenderProjectSuccessPopup() {
  if (ImGui::BeginPopupModal("ProjectSuccessModal", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {

    ImGui::Text("Project created at: %s", GetProjectPath().string().c_str());

    if (ImGui::Button("Open")) {
      TryExecuteEditor();

      modal_state_ = ModalState::kNone;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Close")) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("ProjectSuccessModal");
}

void UiLayer::RenderProjectExistsPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("ProjectExistsModal", &open,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {

    ImGui::Text("Project specified at %s already exists.",
                GetProjectPath().string().c_str());

    if (ImGui::Button("Close", ImVec2(-1, 0))) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("ProjectExistsModal");
}

void UiLayer::RenderEditorFailedPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("EditorFailedModal", &open,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text(
        "Unable to open editor executable. Try to set `EVE_ROOT` variable "
        "manually.");

    if (ImGui::Button("Close", ImVec2(-1, 0))) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("EditorFailedModal");
}

bool UiLayer::TryExecuteEditor() {
  const char* editor_path_cstr = getenv("EVE_ROOT");
  if (editor_path_cstr == nullptr) {
    return false;
  }

  const fs::path editor_path = editor_path_cstr;

  const auto editor_executable = editor_path / "eve_editor";
  const auto project = GetProjectPath() / project_name_;

#if _WIN32
  bool is_windows = true;
#else
  bool is_windows = false;
#endif

  std::string command =
      std::format("{0}{2} \"{1}.eproj\"", editor_executable.string(),
                  project.string(), is_windows ? ".exe" : "");

  // run at background
  if (is_windows) {
    command =
        std::format("start \"\" /D \"{0}\" {1}", editor_path.string(), command);
  } else {
    command =
        std::format("(cd \"{0}\" ; {1} &)", editor_path.string(), command);
  }

  return std::system(command.c_str());
}
