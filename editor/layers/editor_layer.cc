// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include "core/debug/instrumentor.h"

EditorLayer::EditorLayer() {
  PROFILE_FUNCTION();

  editor_logger_ = LoggerManager::AddLogger("EDITOR");
}

EditorLayer::~EditorLayer() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnStart() {
  PROFILE_FUNCTION();

  LOG_INFO(editor_logger_, "EditorLayer::OnStart");
}

void EditorLayer::OnDestroy() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(float ds) {
  PROFILE_FUNCTION();
}

void EditorLayer::OnGUI(float ds) {
  PROFILE_FUNCTION();
}
