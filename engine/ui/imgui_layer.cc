// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "ui/imgui_layer.h"

#include <GLFW/glfw3.h>
#include <IconsFontAwesome4.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace eve {
ImGuiLayer::ImGuiLayer(Ref<State>& state) : Layer(state) {}

void ImGuiLayer::Begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
}

void ImGuiLayer::End() {
  ImGuiIO& io = ImGui::GetIO();
  auto size = GetState()->window->GetSize();
  io.DisplaySize = ImVec2((float)size.x, (float)size.y);

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::OnStart() {
  // Setup ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "assets/fonts/Roboto/Roboto-Regular.ttf", 18.0f);

  ImFontConfig config;
  config.MergeMode = true;
  static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  io.Fonts->AddFontFromFileTTF("assets/fonts/fontawesome-webfont.ttf", 13.0f,
                               &config, icon_ranges);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  SetDarkThemeColors();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(GetState()->window->GetNativeWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiLayer::OnDestroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::SetDarkThemeColors() {
  ImGuiStyle& style = ImGui::GetStyle();

  ImVec4* colors = style.Colors;

  // Set your base color
  style.WindowRounding = 0.0f;
  style.FrameRounding = 0.0f;
  style.GrabRounding = 0.0f;
  style.ScrollbarRounding = 0.0f;
  style.ChildRounding = 0.0f;
  style.PopupRounding = 0.0f;

  // Dark mode colors
  colors[ImGuiCol_Text] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
  colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_Separator] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.6f, 0.6f, 0.6f, 0.5f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  colors[ImGuiCol_TextSelectedBg] =
      ImVec4(0.0f, 0.0f, 1.0f, 0.5f);  // Blue background for selected text
}

}  // namespace eve
