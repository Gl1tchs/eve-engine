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
  ImGuiIO& io = ImGui::GetIO();
  if (settings_.enable_docking)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  else
    io.ConfigFlags &= ImGuiConfigFlags_DockingEnable;

  if (settings_.enable_viewports)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  else
    io.ConfigFlags &= ImGuiConfigFlags_ViewportsEnable;

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

void ImGuiLayer::OnCreate() {
  // Setup ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  if (settings_.enable_docking)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  if (settings_.enable_viewports)
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

  style.WindowPadding = ImVec2(7.00f, 7.00f);
  style.FramePadding = ImVec2(7.00f, 7.00f);
  style.CellPadding = ImVec2(6.00f, 6.00f);
  style.ItemSpacing = ImVec2(6.00f, 6.00f);
  style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
  style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
  style.IndentSpacing = 25;
  style.ScrollbarSize = 15;
  style.GrabMinSize = 10;
  style.WindowBorderSize = 1;
  style.ChildBorderSize = 1;
  style.PopupBorderSize = 1;
  style.FrameBorderSize = 0;
  style.TabBorderSize = 1;
  style.WindowRounding = 0;
  style.ChildRounding = 0;
  style.FrameRounding = 0;
  style.PopupRounding = 0;
  style.ScrollbarRounding = 0;
  style.GrabRounding = 0;
  style.LogSliderDeadzone = 4;
  style.TabRounding = 0;

  constexpr ImVec4 kBackground(0.14f, 0.12f, 0.16f, 1.0f);
  constexpr ImVec4 kBackgroundHover(0.16f, 0.14f, 0.18f, 1.0f);

  constexpr ImVec4 kBackgroundDim(kBackground.x, kBackground.y, kBackground.z,
                                  0.54f);
  constexpr ImVec4 kTransparent(kBackground.x, kBackground.y, kBackground.z,
                                0.0f);

  constexpr ImVec4 kPrimary(0.29f, 0.26f, 0.32f, 1.0f);
  constexpr ImVec4 kPrimaryHover(0.31f, 0.29f, 0.34f, 1.0f);

  constexpr ImVec4 kSecondary(0.26f, 0.18f, 0.36f, 1.0f);
  constexpr ImVec4 kSecondaryHover(0.28f, 0.20f, 0.38f, 1.0f);
  constexpr ImVec4 kSecondaryLight(0.52f, 0.36f, 0.72f, 1.0f);
  constexpr ImVec4 kSecondaryLightHover(0.55f, 0.39f, 0.75f, 1.0f);

  constexpr ImVec4 kTertiary(0.21f, 0.18f, 0.25f, 1.0f);
  constexpr ImVec4 kTertiaryHover(0.23f, 0.20f, 0.27f, 1.0f);

  // Color schema
  style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.28f, 0.29f, 1.00f);

  style.Colors[ImGuiCol_WindowBg] = kBackground;
  style.Colors[ImGuiCol_ChildBg] = kTransparent;
  style.Colors[ImGuiCol_PopupBg] = kBackground;

  style.Colors[ImGuiCol_Border] = kBackground;
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.24f);

  style.Colors[ImGuiCol_FrameBg] = kTertiary;
  style.Colors[ImGuiCol_FrameBgHovered] = kTertiaryHover;
  style.Colors[ImGuiCol_FrameBgActive] = kPrimary;

  style.Colors[ImGuiCol_TitleBg] = kBackground;
  style.Colors[ImGuiCol_TitleBgActive] = kBackgroundHover;
  style.Colors[ImGuiCol_TitleBgCollapsed] = kBackground;

  style.Colors[ImGuiCol_MenuBarBg] = kBackground;

  style.Colors[ImGuiCol_ScrollbarBg] = kBackground;
  style.Colors[ImGuiCol_ScrollbarGrab] = kPrimary;
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = kPrimaryHover;
  style.Colors[ImGuiCol_ScrollbarGrabActive] = kSecondary;

  style.Colors[ImGuiCol_CheckMark] = kSecondaryLight;

  style.Colors[ImGuiCol_SliderGrab] = kSecondaryLight;
  style.Colors[ImGuiCol_SliderGrabActive] = kSecondaryLightHover;

  style.Colors[ImGuiCol_Button] = kPrimary;
  style.Colors[ImGuiCol_ButtonHovered] = kPrimaryHover;
  style.Colors[ImGuiCol_ButtonActive] = kSecondary;

  style.Colors[ImGuiCol_Header] = kTertiary;
  style.Colors[ImGuiCol_HeaderHovered] = kTertiaryHover;
  style.Colors[ImGuiCol_HeaderActive] = kPrimary;

  style.Colors[ImGuiCol_Separator] = kPrimary;
  style.Colors[ImGuiCol_SeparatorHovered] = kPrimaryHover;
  style.Colors[ImGuiCol_SeparatorActive] = kSecondary;

  style.Colors[ImGuiCol_ResizeGrip] = kPrimary;
  style.Colors[ImGuiCol_ResizeGripHovered] = kPrimaryHover;
  style.Colors[ImGuiCol_ResizeGripActive] = kSecondary;

  style.Colors[ImGuiCol_Tab] = kPrimary;
  style.Colors[ImGuiCol_TabHovered] = kSecondary;
  style.Colors[ImGuiCol_TabActive] = kSecondary;
  style.Colors[ImGuiCol_TabUnfocused] = kPrimary;
  style.Colors[ImGuiCol_TabUnfocusedActive] = kSecondary;

  style.Colors[ImGuiCol_DockingPreview] = kSecondary;
  style.Colors[ImGuiCol_DockingEmptyBg] = kPrimary;

  style.Colors[ImGuiCol_PlotLines] = kSecondary;
  style.Colors[ImGuiCol_PlotLinesHovered] = kSecondary;
  style.Colors[ImGuiCol_PlotHistogram] = kSecondary;
  style.Colors[ImGuiCol_PlotHistogramHovered] = kSecondary;

  style.Colors[ImGuiCol_TableHeaderBg] = kSecondary;
  style.Colors[ImGuiCol_TableBorderStrong] = kSecondary;
  style.Colors[ImGuiCol_TableBorderLight] = kSecondary;
  style.Colors[ImGuiCol_TableRowBg] = kTransparent;
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

  style.Colors[ImGuiCol_DragDropTarget] = kSecondary;

  style.Colors[ImGuiCol_NavHighlight] = kSecondary;
  style.Colors[ImGuiCol_NavWindowingHighlight] = kBackgroundDim;
  style.Colors[ImGuiCol_NavWindowingDimBg] = kBackgroundDim;

  style.Colors[ImGuiCol_ModalWindowDimBg] = kBackgroundDim;
}

}  // namespace eve
