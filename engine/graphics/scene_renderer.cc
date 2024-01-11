// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/scene_renderer.h"

#include "asset/asset_registry.h"
#include "core/utils/timer.h"
#include "graphics/render_command.h"
#include "graphics/renderer.h"
#include "scene/entity.h"
#include "scene/scene_manager.h"
#include "scene_renderer.h"

namespace eve {

SceneRenderer::SceneRenderer(const Ref<State>& state) : state_(state) {
  skybox_ = SkyBox::Create("assets/textures/skybox_default.png");
}

void SceneRenderer::RenderRuntime(float ds) {
  auto& scene = SceneManager::GetActive();
  if (!scene) {
    return;
  }

  auto camera = scene->GetPrimaryCameraEntity();
  if (camera && camera.HasComponent<CameraComponent>()) {
    auto& cc = camera.GetComponent<CameraComponent>();
    auto& tc = camera.GetTransform();

    CameraData data;
    if (cc.is_orthographic) {
      data = {cc.ortho_camera.GetViewMatrix(tc),
              cc.ortho_camera.GetProjectionMatrix(), tc.position};
    } else {
      data = {cc.persp_camera.GetViewMatrix(tc),
              cc.persp_camera.GetProjectionMatrix(), tc.position};
    }

    RenderSceneRuntime(data);
  }
}

void SceneRenderer::RenderEditor(float ds, Ref<EditorCamera>& editor_camera) {
  auto& scene = SceneManager::GetActive();
  if (!scene) {
    return;
  }

  CameraData data = {editor_camera->GetViewMatrix(),
                     editor_camera->GetProjectionMatrix(),
                     editor_camera->GetTransform().position};

  RenderSceneEditor(data);
}

void SceneRenderer::OnViewportResize(glm::uvec2 size) {
  auto& scene = SceneManager::GetActive();
  if (!scene) {
    return;
  }

  viewport_size_ = size;

  // Resize our non-FixedAspectRatio cameras
  scene->GetAllEntitiesWith<CameraComponent>().each(
      [size](entt::entity, CameraComponent& cc) {
        cc.SetAspectRatio((float)size.x / (float)size.y);
      });
}

void SceneRenderer::RenderSceneEditor(const CameraData& data) {
  auto& scene = SceneManager::GetActive();
  auto& renderer = state_->renderer;

  Timer timer;

  renderer->BeginScene(data);

  // Draw camera bounds if selected
  const Entity selected_entity = scene->GetSelectedEntity();
  if (selected_entity && selected_entity.HasComponent<CameraComponent>()) {
    RenderCameraBounds();
  }

  RenderScene();

  DrawGrid();
  RenderColliderBounds();

  renderer->EndScene();

  auto& stats = renderer->stats_;

  // Render skybox in another draw call
  skybox_->Render();
  stats.draw_calls++;

  stats.last_render_duration = timer.GetElapsedMilliseconds();
}

void SceneRenderer::RenderSceneRuntime(const CameraData& data) {
  auto& renderer = state_->renderer;

  Timer timer;

  renderer->BeginScene(data);

  RenderScene();

  if (settings_.draw_grid) {
    DrawGrid();
  }
  if (settings_.render_physics_bounds) {
    RenderColliderBounds();
  }

  renderer->EndScene();

  auto& stats = renderer->stats_;

  // Render skybox in another draw call
  skybox_->Render();
  stats.draw_calls++;

  stats.last_render_duration = timer.GetElapsedMilliseconds();
}

void SceneRenderer::RenderScene() {
  auto& scene = SceneManager::GetActive();
  auto& renderer = state_->renderer;

  scene->GetAllEntitiesWith<Transform, SpriteRendererComponent>().each(
      [&](entt::entity entity_id, const Transform& transform,
          const SpriteRendererComponent& sprite) {
        Entity entity{entity_id, scene.get()};

        if (auto texture = AssetRegistry::Get<Texture>(sprite.texture);
            texture) {
          renderer->DrawQuad(transform, texture, sprite.color,
                             sprite.tex_tiling, sprite.tex_offset);
        } else {
          renderer->DrawQuad(transform, sprite.color, sprite.tex_tiling,
                             sprite.tex_offset);
        }
      });

  scene->GetAllEntitiesWith<Transform, ModelComponent>().each(
      [&](entt::entity entity_id, const Transform& transform,
          const ModelComponent& model_comp) {
        Entity entity{entity_id, scene.get()};

        Material material{};
        if (entity.HasComponent<Material>()) {
          material = entity.GetComponent<Material>();
        }

        renderer->DrawModel(AssetRegistry::Get<Model>(model_comp.model),
                            transform, material);
      });
}

void SceneRenderer::DrawGrid() {
  auto& renderer = state_->renderer;

  constexpr float kGridSize = 1.0f;
  constexpr int kNumLines = 101;

  renderer->DrawLine({-kGridSize * kNumLines / 2.0f, 0.0f, 0.0f},
                     {kGridSize * kNumLines / 2.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f, 1.0f});  // X-axis (red)

  renderer->DrawLine({0.0f, 0.0f, -kGridSize * kNumLines / 2.0f},
                     {0.0f, 0.0f, kGridSize * kNumLines / 2.0f},
                     {0.0f, 0.0f, 1.0f, 1.0f});  // Z-axis (blue)

  const glm::vec4 sub_grid_color(0.5f, 0.5f, 0.5f, 1.0f);

  for (int i = -kNumLines / 2; i <= kNumLines / 2; ++i) {
    float x_pos = static_cast<float>(i) * kGridSize;
    if (i == 0) {
      continue;
    }

    renderer->DrawLine({x_pos, 0.0f, -kGridSize * kNumLines / 2.0f},
                       {x_pos, 0.0f, kGridSize * kNumLines / 2.0f},
                       sub_grid_color);
  }

  for (int i = -kNumLines / 2; i <= kNumLines / 2; ++i) {
    float z_pos = static_cast<float>(i) * kGridSize;
    if (i == 0) {
      continue;
    }

    renderer->DrawLine({-kGridSize * kNumLines / 2.0f, 0.0f, z_pos},
                       {kGridSize * kNumLines / 2.0f, 0.0f, z_pos},
                       sub_grid_color);
  }
}

void SceneRenderer::RenderCameraBounds() {
  auto& scene = SceneManager::GetActive();
  auto& renderer = state_->renderer;

  // check for selected entity already done
  Entity selected_entity = scene->GetSelectedEntity();
  CameraComponent& cc = selected_entity.GetComponent<CameraComponent>();
  Transform& tc = selected_entity.GetTransform();

  Color color(0.75f, 0.75f, 0.75f, 1.0f);

  // Check if the camera is perspective or orthographic
  if (!cc.is_orthographic) {
    PerspectiveCamera& camera = cc.persp_camera;

    // Perspective camera
    float fov = glm::radians(camera.fov);
    float aspect_ratio = camera.aspect_ratio;
    float near_plane = camera.near_clip;
    float far_plane = camera.far_clip;

    float near_height = 2.0f * near_plane * glm::tan(fov / 2.0f);
    float near_width = near_height * aspect_ratio;

    float far_height = 2.0f * far_plane * glm::tan(fov / 2.0f);
    float far_width = far_height * aspect_ratio;

    // Calculate points of the camera's frustum
    Box near_box = {
        .bottom_left = tc.position - 0.5f * near_height * tc.GetUp() -
                       0.5f * near_width * tc.GetRight() +
                       near_plane * tc.GetForward(),
        .bottom_right = tc.position - 0.5f * near_height * tc.GetUp() +
                        0.5f * near_width * tc.GetRight() +
                        near_plane * tc.GetForward(),
        .top_left = tc.position + 0.5f * near_height * tc.GetUp() -
                    0.5f * near_width * tc.GetRight() +
                    near_plane * tc.GetForward(),
        .top_right = tc.position + 0.5f * near_height * tc.GetUp() +
                     0.5f * near_width * tc.GetRight() +
                     near_plane * tc.GetForward(),
    };

    Box far_box = {.bottom_left = tc.position - 0.5f * far_height * tc.GetUp() -
                                  0.5f * far_width * tc.GetRight() +
                                  far_plane * tc.GetForward(),
                   .bottom_right = tc.position -
                                   0.5f * far_height * tc.GetUp() +
                                   0.5f * far_width * tc.GetRight() +
                                   far_plane * tc.GetForward(),
                   .top_left = tc.position + 0.5f * far_height * tc.GetUp() -
                               0.5f * far_width * tc.GetRight() +
                               far_plane * tc.GetForward(),
                   .top_right = tc.position + 0.5f * far_height * tc.GetUp() +
                                0.5f * far_width * tc.GetRight() +
                                far_plane * tc.GetForward()};

    // Draw frustum boxes
    renderer->DrawBox(near_box, color);
    renderer->DrawBox(far_box, color);

    // Draw connection lines
    renderer->DrawLine(near_box.top_left, far_box.top_left, color);
    renderer->DrawLine(near_box.top_right, far_box.top_right, color);
    renderer->DrawLine(near_box.bottom_left, far_box.bottom_left, color);
    renderer->DrawLine(near_box.bottom_right, far_box.bottom_right, color);
  } else {
    OrthographicCamera& camera = cc.ortho_camera;

    // Orthographic camera
    float zoom_level = camera.zoom_level;
    float aspect_ratio = camera.aspect_ratio;
    float near_plane = camera.near_clip;
    float far_plane = camera.far_clip;

    float half_height = zoom_level * 0.5f;
    float half_width = half_height * aspect_ratio;

    // Calculate points of the camera's frustum
    Box near_box = {
        .bottom_left = tc.position - half_height * tc.GetUp() -
                       half_width * tc.GetRight() +
                       near_plane * tc.GetForward(),
        .bottom_right = tc.position - half_height * tc.GetUp() +
                        half_width * tc.GetRight() +
                        near_plane * tc.GetForward(),
        .top_left = tc.position + half_height * tc.GetUp() -
                    half_width * tc.GetRight() + near_plane * tc.GetForward(),
        .top_right = tc.position + half_height * tc.GetUp() +
                     half_width * tc.GetRight() + near_plane * tc.GetForward()};

    Box far_box = {
        .bottom_left = tc.position - half_height * tc.GetUp() -
                       half_width * tc.GetRight() + far_plane * tc.GetForward(),
        .bottom_right = tc.position - half_height * tc.GetUp() +
                        half_width * tc.GetRight() +
                        far_plane * tc.GetForward(),
        .top_left = tc.position + half_height * tc.GetUp() -
                    half_width * tc.GetRight() + far_plane * tc.GetForward(),
        .top_right = tc.position + half_height * tc.GetUp() +
                     half_width * tc.GetRight() + far_plane * tc.GetForward()};

    // Draw frustum boxes
    renderer->DrawBox(near_box, color);
    renderer->DrawBox(far_box, color);

    // Draw connection lines
    renderer->DrawLine(near_box.top_left, far_box.top_left, color);
    renderer->DrawLine(near_box.top_right, far_box.top_right, color);
    renderer->DrawLine(near_box.bottom_left, far_box.bottom_left, color);
    renderer->DrawLine(near_box.bottom_right, far_box.bottom_right, color);
  }
}

void SceneRenderer::RenderColliderBounds() {
  auto& scene = SceneManager::GetActive();
  auto& renderer = state_->renderer;

  // renderer->DrawBox()
  scene->GetAllEntitiesWith<Transform, BoxCollider>().each(
      [renderer](entt::entity entity_id, Transform& tc, BoxCollider& col) {
        Transform tc_col = tc;
        tc_col.position += col.local_position;
        tc_col.scale = col.local_scale;

        // Render collider bounds as wireframe.
        renderer->DrawCube(tc_col, kColorGreen, PolygonMode::kLine);
      });
}

}  // namespace eve
