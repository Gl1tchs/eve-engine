// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "ui_layer.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace fs = std::filesystem;

UiLayer::UiLayer(Ref<State> state) : Layer(state) {
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
  ImGuiIO& io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
  ImGui::Begin("##main_window", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

  const ImVec2& window_size = ImGui::GetWindowSize();

  const float content_width = window_size.x * 0.7f;
  // const float options_width = window_size.x - content_width;

  ImGui::BeginChild("Content", ImVec2(content_width, 0), true);
  {
    float panel_width = ImGui::GetContentRegionAvail().x;

    static constexpr uint32_t cell_size = 100;

    uint32_t template_count = templates_.size();
    uint32_t column_count = static_cast<uint32_t>(panel_width / cell_size);

    ImGui::Columns(column_count, 0, false);

    for (const ProjectTemplateInfo& info : templates_) {

      ImGui::PushID(info.name.c_str());

      if (ImGui::Button(info.name.c_str(), ImVec2(cell_size, cell_size))) {
        selected_info_ = info;
      }

      ImGui::TextUnformatted(info.name.c_str());

      ImGui::PopID();
    }

    ImGui::Columns();
  }
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Options", ImVec2(0, 0), true);
  if (selected_info_.type != ProjectTemplate::kNone) {

    ImGui::InputText("Name", &project_name_);
    ImGui::InputText("Path", &project_path_);

    if (ImGui::Button("Create", ImVec2(-1, 0))) {
      if (fs::exists(fs::path(project_path_) / project_name_)) {
        modal_state_ = ModalState::kProjectExists;
      } else if (CreateProject()) {
        modal_state_ = ModalState::kProjectSuccess;
      } else {
        modal_state_ = ModalState::kProjectFailed;
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
  if (project_name_.empty() || project_path_.empty()) {
    std::cerr << "Invalid project name or path." << std::endl;
    return false;
  }

  if (!fs::exists(selected_info_.path)) {
    std::cerr << "Source path does not exist." << std::endl;
    return false;
  }

  fs::path new_path = fs::path(project_path_) / project_name_;

  try {
    fs::create_directories(new_path);
    fs::copy(selected_info_.path, new_path, fs::copy_options::recursive);
  } catch (const fs::filesystem_error& e) {
    return false;
  }

  fs::path new_project_path = new_path / std::format("{}.eproj", project_name_);

  fs::rename(new_path / "{{project_name}}.eproj", new_project_path);

  const std::string name_key = "{{project_name}}";

  std::ifstream project_in(new_project_path);
  if (!project_in.is_open()) {
    return false;
  }

  std::ostringstream text;
  text << project_in.rdbuf();

  std::string content = text.str();
  size_t pos = content.find(name_key);
  content.replace(pos, name_key.length(), project_name_);

  project_in.close();

  std::ofstream project_out(new_project_path);
  if (!project_out.is_open()) {
    return false;
  }

  project_out << content;

  return true;
}

void UiLayer::RenderProjectFailedPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("failed_popup", &open,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Unable to create project on specified path: %s",
                project_path_.c_str());
    if (ImGui::Button("Close", ImVec2(-1, 0))) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("failed_popup");
}

void UiLayer::RenderProjectSuccessPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("success_popup", &open,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Project created at: %s\nWould you like to open it?",
                GetProjectPath().string().c_str());

    if (ImGui::Button("Open")) {
      const char* editor_path = getenv("EVE_ROOT");
      if (editor_path != nullptr) {
        const auto editor_executable = fs::path(editor_path) / "eve_editor";
        const auto project = GetProjectPath() / project_name_;

#if _WIN32
        bool is_windows = true;
#else
        bool is_windows = false;
#endif

        std::string command =
            std::format("{0}{2} {1}.eproj", editor_executable.string(),
                        project.string(), is_windows ? ".exe" : "");

        // run at background
        if (is_windows) {
          command = std::format("start \"\" {}", command);
        } else {
          command = std::format("{} &", command);
        }

        bool success = std::system(command.c_str());

        modal_state_ = ModalState::kNone;

        ImGui::CloseCurrentPopup();
      } else {
        modal_state_ = ModalState::kEditorFailed;
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Close")) {
      modal_state_ = ModalState::kNone;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::OpenPopup("success_popup");
}

void UiLayer::RenderProjectExistsPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("project_exists_popup", &open,
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

  ImGui::OpenPopup("project_exists_popup");
}

void UiLayer::RenderEditorFailedPopup() {
  bool open = true;
  if (ImGui::BeginPopupModal("editor_failed_popup", &open,
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

  ImGui::OpenPopup("editor_failed_popup");
}
