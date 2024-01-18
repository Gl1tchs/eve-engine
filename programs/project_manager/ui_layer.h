// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include <vector>

#include "core/layer.h"
#include "project/project.h"

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

class UiLayer : public eve::Layer {
 public:
  UiLayer(eve::Ref<eve::State> state);

  void OnCreate() override;

  void OnDestroy() override;

  void OnGUI(float ds) override;

 private:
  bool CreateProject();

  void RenderProjectFailedPopup();

  void RenderProjectSuccessPopup();

  void RenderProjectExistsPopup();

  void RenderEditorFailedPopup();

  inline fs::path GetProjectPath() {
    return fs::path(project_directory_) / project_name_;
  }

  bool TryExecuteEditor();

 private:
  std::vector<ProjectTemplateInfo> templates_;
  ProjectTemplateInfo selected_info_{};

  std::string project_name_;
  std::string project_directory_;

  ModalState modal_state_ = ModalState::kNone;
};
