// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include <vector>

#include "core/layer.h"
#include "project/project.h"

using namespace eve;

enum class ProjectTemplate : uint32_t { kNone = 0, kBlank = 1 };

struct ProjectTemplateInfo {
  ProjectTemplate type;
  std::string name;
  std::string path;
};

enum class ModalState {
  kNone,
  kProjectSuccess,
  kProjectFailed,
  kProjectExists,
  kEditorFailed
};

class UiLayer : public Layer {
 public:
  UiLayer(Ref<State> state);

  void OnStart() override;

  void OnDestroy() override;

  void OnGUI(float ds) override;

 private:
  bool CreateProject();

  void RenderProjectFailedPopup();

  void RenderProjectSuccessPopup();

  void RenderProjectExistsPopup();

  void RenderEditorFailedPopup();

  inline fs::path GetProjectPath() {
    return fs::path(project_path_) / project_name_;
  }

 private:
  std::vector<ProjectTemplateInfo> templates_;
  ProjectTemplateInfo selected_info_{};

  std::string project_name_ = "";
  std::string project_path_ = "";

  ModalState modal_state_ = ModalState::kNone;
};
