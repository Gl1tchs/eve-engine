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
  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.92f);
  colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.29f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.28f, 0.29f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.54f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.44f, 0.44f, 0.44f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.43f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.44f, 0.44f, 0.54f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
  colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 0.54f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.28f, 0.29f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.28f, 0.29f, 0.33f);
  colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.29f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.54f, 0.54f, 0.29f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 0.54f, 0.57f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.38f, 0.38f, 0.38f, 0.29f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.54f, 0.54f, 0.29f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.50f, 0.54f, 0.57f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.36f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.43f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.38f, 0.38f, 0.38f, 0.29f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.28f, 0.29f, 1.00f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.43f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding = ImVec2(8.00f, 8.00f);
  style.FramePadding = ImVec2(5.00f, 2.00f);
  style.CellPadding = ImVec2(6.00f, 6.00f);
  style.ItemSpacing = ImVec2(6.00f, 6.00f);
  style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
  style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
  style.IndentSpacing = 25;
  style.ScrollbarSize = 15;
  style.GrabMinSize = 10;
  style.WindowBorderSize = 0;
  style.ChildBorderSize = 0;
  style.PopupBorderSize = 0;
  style.FrameBorderSize = 0;
  style.TabBorderSize = 1;
  style.WindowRounding = 7;
  style.ChildRounding = 4;
  style.FrameRounding = 3;
  style.PopupRounding = 4;
  style.ScrollbarRounding = 9;
  style.GrabRounding = 3;
  style.LogSliderDeadzone = 4;
  style.TabRounding = 4;
}

}  // namespace eve
