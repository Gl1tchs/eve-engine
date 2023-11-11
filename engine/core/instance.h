// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include <type_traits>

#include "core/core_minimal.h"
#include "core/layer_stack.h"
#include "core/state.h"
#include "core/utils/memory.h"
#include "ui/imgui_layer.h"

struct InstanceSpecifications {
  std::string name;
  std::string description;
};

class Instance {
 public:
  Instance(const InstanceSpecifications& specs);
  virtual ~Instance();

  void StartEventLoop();

  Ref<State> GetState() { return state_; }
  const Ref<State>& GetState() const { return state_; }

  static Instance& Get() { return *instance_; };

 protected:
  void PushLayer(Layer* layer);

  template <typename T, typename... Args>
    requires std::is_base_of_v<Layer, T>
  void PushLayer(Args&&... args) {
    PushLayer(new T(args...));
  }

  void PushOverlay(Layer* overlay);

  template <typename T, typename... Args>
    requires std::is_base_of_v<Layer, T>
  void PushOverlay(Args&&... args) {
    PushOverlay(new T(args...));
  }

  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

 private:
  static Instance* instance_;

  Ref<State> state_;

  LayerStack layers_;
  ImGuiLayer* imgui_layer_;

  InstanceSpecifications specs_;
};