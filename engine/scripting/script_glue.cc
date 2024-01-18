// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch_shared.h"

#include "scripting/script_glue.h"

#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include "core/color.h"
#include "core/event/input.h"
#include "core/event/key_code.h"
#include "core/instance.h"
#include "core/uuid.h"
#include "physics/rigidbody.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/transform.h"
#include "scripting/script_engine.h"

namespace eve {

std::string MonoStringToString(MonoString* string) {
  char* c_str = mono_string_to_utf8(string);
  std::string str(c_str);
  mono_free(c_str);
  return str;
}

static std::unordered_map<MonoType*, std::function<bool(Entity)>>
    entity_has_component_funcs;

static std::unordered_map<MonoType*, std::function<void(Entity)>>
    entity_add_component_funcs;

#define ADD_INTERNAL_CALL(Name) \
  mono_add_internal_call("EveEngine.Interop::" #Name, Name)

static MonoObject* GetScriptInstance(UUID entity_id) {
  return ScriptEngine::GetManagedInstance(entity_id);
}

#pragma region Application

static void Application_Quit() {
  Instance::Get().Quit();
}

#pragma endregion
#pragma region Window

static CursorMode Window_GetCursorMode() {
  auto state = Instance::Get().GetState();
  return state->window->GetCursorMode();
}

static void Window_SetCursorMode(CursorMode mode) {
  auto state = Instance::Get().GetState();
  state->window->SetCursorMode(mode);
}

#pragma endregion
#pragma region Debug

static void Debug_Log(MonoString* string) {
  EVE_LOG_CLIENT_TRACE("{}", MonoStringToString(string));
}

static void Debug_LogInfo(MonoString* string) {
  EVE_LOG_CLIENT_INFO("{}", MonoStringToString(string));
}

static void Debug_LogWarning(MonoString* string) {
  EVE_LOG_CLIENT_WARNING("{}", MonoStringToString(string));
}

static void Debug_LogError(MonoString* string) {
  EVE_LOG_CLIENT_ERROR("{}", MonoStringToString(string));
}

static void Debug_LogFatal(MonoString* string) {
  EVE_LOG_CLIENT_FATAL("{}", MonoStringToString(string));
}

#pragma endregion
#pragma region Entity

static void Entity_Destroy(UUID entity_id) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  scene->DestroyEntity(entity);
}

static uint64_t Entity_GetParent(UUID entity_id) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  Entity parent_entity = entity.GetParent();

  return parent_entity ? parent_entity.GetUUID() : kInvalidUUID;
}

static MonoString* Entity_GetName(UUID entity_id) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  return ScriptEngine::CreateMonoString(entity.GetName().c_str());
}

static bool Entity_HasComponent(UUID entity_id,
                                MonoReflectionType* component_type) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  MonoType* managed_type = mono_reflection_type_get_type(component_type);
  EVE_ASSERT_ENGINE(entity_has_component_funcs.find(managed_type) !=
                    entity_has_component_funcs.end());
  return entity_has_component_funcs.at(managed_type)(entity);
}

static void Entity_AddComponent(UUID entity_id,
                                MonoReflectionType* component_type) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  MonoType* managed_type = mono_reflection_type_get_type(component_type);
  EVE_ASSERT_ENGINE(entity_add_component_funcs.find(managed_type) !=
                    entity_add_component_funcs.end());

  entity_add_component_funcs.at(managed_type)(entity);
}

static uint64_t Entity_TryGetEntityByName(MonoString* name) {
  char* name_cstr = mono_string_to_utf8(name);

  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByName(name_cstr);
  mono_free(name_cstr);

  if (!entity) {
    return 0;
  }

  return entity.GetUUID();
}

static uint64_t Entity_Instantiate(MonoString* name, UUID parent_id,
                                   glm::vec3* position, glm::vec3* rotation,
                                   glm::vec3* scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);

  Entity created_entity =
      scene->CreateEntity({MonoStringToString(name), parent_id});
  if (!created_entity) {
    return 0;
  }

  Transform& tc = created_entity.GetTransform();
  tc.local_position = *position;
  tc.local_rotation = *rotation;
  tc.local_scale = *scale;

  return created_entity.GetUUID();
}

static void Entity_AssignScript(UUID entity_id, MonoString* class_name) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  auto& sc = entity.AddComponent<ScriptComponent>();
  sc.class_name = MonoStringToString(class_name);

  // Register entity to the scripting engine
  ScriptEngine::OnCreateEntity(entity);
}

#pragma endregion
#pragma region TransformComponent

static void TransformComponent_GetLocalPosition(UUID entity_id,
                                                glm::vec3* out_position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_position = entity.GetComponent<Transform>().local_position;
}

static void TransformComponent_SetLocalPosition(UUID entity_id,
                                                glm::vec3* position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Transform>().local_position = *position;
}

static void TransformComponent_GetLocalRotation(UUID entity_id,
                                                glm::vec3* out_rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_rotation = entity.GetComponent<Transform>().local_rotation;
}

static void TransformComponent_SetLocalRotation(UUID entity_id,
                                                glm::vec3* rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Transform>().local_rotation = *rotation;
}

static void TransformComponent_GetLocalScale(UUID entity_id,
                                             glm::vec3* out_scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_scale = entity.GetComponent<Transform>().local_scale;
}

static void TransformComponent_SetLocalScale(UUID entity_id, glm::vec3* scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Transform>().local_scale = *scale;
}

static void TransformComponent_GetPosition(UUID entity_id,
                                           glm::vec3* out_position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_position = entity.GetComponent<Transform>().GetPosition();
}

static void TransformComponent_GetRotation(UUID entity_id,
                                           glm::vec3* out_rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_rotation = entity.GetComponent<Transform>().GetRotation();
}

static void TransformComponent_GetScale(UUID entity_id, glm::vec3* out_scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_scale = entity.GetComponent<Transform>().GetScale();
}

static void TransformComponent_GetForward(UUID entity_id,
                                          glm::vec3* out_forward) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_forward = entity.GetComponent<Transform>().GetForward();
}

static void TransformComponent_GetRight(UUID entity_id, glm::vec3* out_right) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_right = entity.GetComponent<Transform>().GetRight();
}

static void TransformComponent_GetUp(UUID entity_id, glm::vec3* out_up) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_up = entity.GetComponent<Transform>().GetUp();
}

static void TransformComponent_Translate(UUID entity_id,
                                         glm::vec3* translation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetTransform().Translate(*translation);
}

static void TransformComponent_Rotate(UUID entity_id, const float angle,
                                      glm::vec3* axis) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetTransform().Rotate(angle, *axis);
}

static void TransformComponent_LookAt(UUID entity_id, glm::vec3* target) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetTransform().LookAt(*target);
}

#pragma endregion
#pragma region CameraComponent

static void CameraComponent_OrthographicCamera_GetAspectRatio(
    UUID entity_id, float* out_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_aspect_ratio =
      entity.GetComponent<CameraComponent>().ortho_camera.aspect_ratio;
}

static void CameraComponent_OrthographicCamera_SetAspectRatio(
    UUID entity_id, float* aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.aspect_ratio =
      *aspect_ratio;
}

static void CameraComponent_OrthographicCamera_GetZoomLevel(
    UUID entity_id, float* out_zoom_level) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_zoom_level =
      entity.GetComponent<CameraComponent>().ortho_camera.zoom_level;
}

static void CameraComponent_OrthographicCamera_SetZoomLevel(UUID entity_id,
                                                            float* zoom_level) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.zoom_level = *zoom_level;
}

static void CameraComponent_OrthographicCamera_GetNearClip(
    UUID entity_id, float* out_near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_near_clip =
      entity.GetComponent<CameraComponent>().ortho_camera.near_clip;
}

static void CameraComponent_OrthographicCamera_SetNearClip(UUID entity_id,
                                                           float* near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.near_clip = *near_clip;
}

static void CameraComponent_OrthographicCamera_GetFarClip(UUID entity_id,
                                                          float* out_far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_far_clip = entity.GetComponent<CameraComponent>().ortho_camera.far_clip;
}

static void CameraComponent_OrthographicCamera_SetFarClip(UUID entity_id,
                                                          float* far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.far_clip = *far_clip;
}

static void CameraComponent_PerspectiveCamera_GetAspectRatio(
    UUID entity_id, float* out_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_aspect_ratio =
      entity.GetComponent<CameraComponent>().persp_camera.aspect_ratio;
}

static void CameraComponent_PerspectiveCamera_SetAspectRatio(
    UUID entity_id, float* aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().persp_camera.aspect_ratio =
      *aspect_ratio;
}

static void CameraComponent_PerspectiveCamera_GetFov(UUID entity_id,
                                                     float* out_fov) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_fov = entity.GetComponent<CameraComponent>().persp_camera.fov;
}

static void CameraComponent_PerspectiveCamera_SetFov(UUID entity_id,
                                                     float* fov) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().persp_camera.fov = *fov;
}

static void CameraComponent_PerspectiveCamera_GetNearClip(
    UUID entity_id, float* out_near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_near_clip =
      entity.GetComponent<CameraComponent>().persp_camera.near_clip;
}

static void CameraComponent_PerspectiveCamera_SetNearClip(UUID entity_id,
                                                          float* near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().persp_camera.near_clip = *near_clip;
}

static void CameraComponent_PerspectiveCamera_GetFarClip(UUID entity_id,
                                                         float* out_far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_far_clip = entity.GetComponent<CameraComponent>().persp_camera.far_clip;
}

static void CameraComponent_PerspectiveCamera_SetFarClip(UUID entity_id,
                                                         float* far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().persp_camera.far_clip = *far_clip;
  ;
}

static void CameraComponent_GetIsOrthographic(UUID entity_id,
                                              float* out_is_orthographic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_is_orthographic = entity.GetComponent<CameraComponent>().is_orthographic;
}

static void CameraComponent_SetIsOrthographic(UUID entity_id,
                                              float* is_orthographic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().is_orthographic = *is_orthographic;
}

static void CameraComponent_GetIsPrimary(UUID entity_id,
                                         float* out_is_primary) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_is_primary = entity.GetComponent<CameraComponent>().is_primary;
}

static void CameraComponent_SetIsPrimary(UUID entity_id, float* is_primary) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().is_primary = *is_primary;
}

static void CameraComponent_GetIsFixedAspectRato(
    UUID entity_id, float* out_is_fixed_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_is_fixed_aspect_ratio =
      entity.GetComponent<CameraComponent>().is_fixed_aspect_ratio;
}

static void CameraComponent_SetIsFixedAspectRato(UUID entity_id,
                                                 float* is_fixed_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<CameraComponent>().is_fixed_aspect_ratio =
      *is_fixed_aspect_ratio;
}

#pragma endregion
#pragma region Material

static void Material_GetAlbedo(UUID entity_id, Color* out_albedo) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_albedo = entity.GetComponent<Material>().albedo;
}

static void Material_SetAlbedo(UUID entity_id, Color* albedo) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Material>().albedo = *albedo;
}

#pragma endregion
#pragma region ScriptComponent

static MonoString* ScriptComponent_GetClassName(UUID entity_id) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  const auto& sc = entity.GetComponent<ScriptComponent>();
  return ScriptEngine::CreateMonoString(sc.class_name.c_str());
}

#pragma endregion
#pragma region RigidbodyComponent

static void Rigidbody_GetVelocity(UUID entity_id, glm::vec3* out_velocity) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_velocity = entity.GetComponent<Rigidbody>().velocity;
}

static void Rigidbody_SetVelocity(UUID entity_id, glm::vec3* velocity) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().velocity = *velocity;
}

static void Rigidbody_GetAcceleration(UUID entity_id,
                                      glm::vec3* out_acceleration) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_acceleration = entity.GetComponent<Rigidbody>().acceleration;
}

static void Rigidbody_SetAcceleration(UUID entity_id, glm::vec3* acceleration) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().acceleration = *acceleration;
}

static void Rigidbody_GetMass(UUID entity_id, float* out_mass) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_mass = entity.GetComponent<Rigidbody>().mass;
}

static void Rigidbody_SetMass(UUID entity_id, float* mass) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().mass = *mass;
}

static void Rigidbody_GetUseGravity(UUID entity_id, bool* out_use_gravity) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_use_gravity = entity.GetComponent<Rigidbody>().use_gravity;
}

static void Rigidbody_SetUseGravity(UUID entity_id, bool* use_gravity) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().use_gravity = *use_gravity;
}

static void Rigidbody_GetPositionConstraints(
    UUID entity_id, PositionConstraints* out_position_constraints) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_position_constraints =
      entity.GetComponent<Rigidbody>().position_constraints;
}

static void Rigidbody_SetPositionConstraints(
    UUID entity_id, PositionConstraints* position_constraints) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().position_constraints = *position_constraints;
}

static void Rigidbody_GetRotationConstraints(
    UUID entity_id, RotationConstraints* out_rotation_constraints) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  *out_rotation_constraints =
      entity.GetComponent<Rigidbody>().rotation_constraints;
}

static void Rigidbody_SetRotationConstraints(
    UUID entity_id, RotationConstraints* rotation_constraints) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  entity.GetComponent<Rigidbody>().rotation_constraints = *rotation_constraints;
}

#pragma endregion
#pragma region BoxCollider

static bool BoxCollider_GetIsTrigger(UUID entity_id) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  return box_collider.is_trigger;
}

static void BoxCollider_SetIsTrigger(UUID entity_id, bool is_trigger) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  box_collider.is_trigger = is_trigger;
}

static void BoxCollider_GetLocalPosition(UUID entity_id,
                                         glm::vec3* out_position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  *out_position = box_collider.local_position;
}

static void BoxCollider_SetLocalPosition(UUID entity_id, glm::vec3* position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  box_collider.local_position = *position;
}

static void BoxCollider_GetLocalScale(UUID entity_id, glm::vec3* out_scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  *out_scale = box_collider.local_scale;
}

static void BoxCollider_SetLocalScale(UUID entity_id, glm::vec3* scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  box_collider.local_scale = *scale;
}

static void BoxCollider_GetOnTrigger(UUID entity_id,
                                     BoxCollider::TriggerFunc out_on_trigger) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  out_on_trigger = box_collider.on_trigger;
}

static void BoxCollider_SetOnTrigger(UUID entity_id,
                                     BoxCollider::TriggerFunc on_trigger) {
  Scene* scene = ScriptEngine::GetSceneContext();
  EVE_ASSERT_ENGINE(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  EVE_ASSERT_ENGINE(entity);

  BoxCollider& box_collider = entity.GetComponent<BoxCollider>();

  box_collider.on_trigger = on_trigger;
}

#pragma endregion
#pragma region Input

static bool Input_IsKeyPressed(KeyCode keycode) {
  return Input::IsKeyPressed(keycode);
}

static bool Input_IsKeyPressedString(MonoString* keycode) {
  return Input::IsKeyPressed(MonoStringToString(keycode));
}

static bool Input_IsKeyReleased(KeyCode keycode) {
  return Input::IsKeyReleased(keycode);
}

static bool Input_IsKeyReleasedString(MonoString* keycode) {
  return Input::IsKeyReleased(MonoStringToString(keycode));
}

static bool Input_IsMouseButtonPressed(MouseCode mouse_code) {
  return Input::IsMouseButtonPressed(mouse_code);
}

static bool Input_IsMouseButtonPressedString(MonoString* keycode) {
  return Input::IsMouseButtonPressed(MonoStringToString(keycode));
}

static bool Input_IsMouseButtonReleased(MouseCode mouse_code) {
  return Input::IsMouseButtonReleased(mouse_code);
}

static bool Input_IsMouseButtonReleasedString(MonoString* keycode) {
  return Input::IsMouseButtonReleased(MonoStringToString(keycode));
}

static void Input_GetMousePosition(glm::vec2* out_position) {
  *out_position = Input::GetMousePosition();
}

#pragma endregion
#pragma region SceneManager

static void SceneManager_SetActive(int index) {
  SceneManager::SetActive(index);
}

static int SceneManager_GetActiveIndex() {
  return SceneManager::GetActiveIndex();
}

#pragma endregion

template <typename... Component>
static void RegisterComponent() {
  (
      []() {
        std::string_view type_name = typeid(Component).name();
        size_t pos = type_name.find_last_of('::');
        std::string_view struct_name = type_name.substr(pos + 1);
        std::string managed_type_name =
            std::format("EveEngine.{}", struct_name);

        MonoType* managed_type = mono_reflection_type_from_name(
            managed_type_name.data(), ScriptEngine::GetCoreAssemblyImage());
        if (!managed_type) {
          EVE_LOG_ENGINE_ERROR("Could not find component type {}",
                               managed_type_name);
          return;
        }
        entity_has_component_funcs[managed_type] = [](Entity entity) {
          return entity.HasComponent<Component>();
        };
        entity_add_component_funcs[managed_type] = [](Entity entity) {
          entity.AddComponent<Component>();
          EVE_ASSERT_ENGINE(entity.HasComponent<Component>());
        };
      }(),
      ...);
}

template <typename... Component>
static void RegisterComponent(ComponentGroup<Component...>) {
  RegisterComponent<Component...>();
}

namespace script_glue {

void RegisterComponents() {
  entity_has_component_funcs.clear();
  RegisterComponent(AllComponents{});
}

void RegisterFunctions() {
  ADD_INTERNAL_CALL(GetScriptInstance);

  // Begin Application
  ADD_INTERNAL_CALL(Application_Quit);

  // Begin Window
  ADD_INTERNAL_CALL(Window_GetCursorMode);
  ADD_INTERNAL_CALL(Window_SetCursorMode);

  // Begin Debug
  ADD_INTERNAL_CALL(Debug_Log);
  ADD_INTERNAL_CALL(Debug_LogInfo);
  ADD_INTERNAL_CALL(Debug_LogWarning);
  ADD_INTERNAL_CALL(Debug_LogError);
  ADD_INTERNAL_CALL(Debug_LogFatal);

  // Begin Entity
  ADD_INTERNAL_CALL(Entity_Destroy);
  ADD_INTERNAL_CALL(Entity_GetParent);
  ADD_INTERNAL_CALL(Entity_GetName);
  ADD_INTERNAL_CALL(Entity_HasComponent);
  ADD_INTERNAL_CALL(Entity_AddComponent);
  ADD_INTERNAL_CALL(Entity_TryGetEntityByName);
  ADD_INTERNAL_CALL(Entity_Instantiate);
  ADD_INTERNAL_CALL(Entity_AssignScript);

  // Begin Transform Component
  ADD_INTERNAL_CALL(TransformComponent_GetLocalPosition);
  ADD_INTERNAL_CALL(TransformComponent_SetLocalPosition);
  ADD_INTERNAL_CALL(TransformComponent_GetLocalRotation);
  ADD_INTERNAL_CALL(TransformComponent_SetLocalRotation);
  ADD_INTERNAL_CALL(TransformComponent_GetLocalScale);
  ADD_INTERNAL_CALL(TransformComponent_SetLocalScale);
  ADD_INTERNAL_CALL(TransformComponent_GetPosition);
  ADD_INTERNAL_CALL(TransformComponent_GetRotation);
  ADD_INTERNAL_CALL(TransformComponent_GetScale);
  ADD_INTERNAL_CALL(TransformComponent_GetForward);
  ADD_INTERNAL_CALL(TransformComponent_GetRight);
  ADD_INTERNAL_CALL(TransformComponent_GetUp);
  ADD_INTERNAL_CALL(TransformComponent_Translate);
  ADD_INTERNAL_CALL(TransformComponent_Rotate);
  ADD_INTERNAL_CALL(TransformComponent_LookAt);

  // Begin Camera Component
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetZoomLevel);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetZoomLevel);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetFov);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetFov);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_GetIsOrthographic);
  ADD_INTERNAL_CALL(CameraComponent_SetIsOrthographic);
  ADD_INTERNAL_CALL(CameraComponent_GetIsPrimary);
  ADD_INTERNAL_CALL(CameraComponent_SetIsPrimary);
  ADD_INTERNAL_CALL(CameraComponent_GetIsFixedAspectRato);
  ADD_INTERNAL_CALL(CameraComponent_SetIsFixedAspectRato);

  // Begin Material
  ADD_INTERNAL_CALL(Material_GetAlbedo);
  ADD_INTERNAL_CALL(Material_SetAlbedo);

  // Begin ScriptComponent
  ADD_INTERNAL_CALL(ScriptComponent_GetClassName);

  // Begin Rigidbody
  ADD_INTERNAL_CALL(Rigidbody_GetVelocity);
  ADD_INTERNAL_CALL(Rigidbody_SetVelocity);
  ADD_INTERNAL_CALL(Rigidbody_GetAcceleration);
  ADD_INTERNAL_CALL(Rigidbody_SetAcceleration);
  ADD_INTERNAL_CALL(Rigidbody_GetMass);
  ADD_INTERNAL_CALL(Rigidbody_SetMass);
  ADD_INTERNAL_CALL(Rigidbody_GetUseGravity);
  ADD_INTERNAL_CALL(Rigidbody_SetUseGravity);
  ADD_INTERNAL_CALL(Rigidbody_GetPositionConstraints);
  ADD_INTERNAL_CALL(Rigidbody_SetPositionConstraints);
  ADD_INTERNAL_CALL(Rigidbody_GetRotationConstraints);
  ADD_INTERNAL_CALL(Rigidbody_SetRotationConstraints);

  // Begin BoxCollider
  ADD_INTERNAL_CALL(BoxCollider_GetIsTrigger);
  ADD_INTERNAL_CALL(BoxCollider_SetIsTrigger);
  ADD_INTERNAL_CALL(BoxCollider_GetLocalPosition);
  ADD_INTERNAL_CALL(BoxCollider_SetLocalPosition);
  ADD_INTERNAL_CALL(BoxCollider_GetLocalScale);
  ADD_INTERNAL_CALL(BoxCollider_SetLocalScale);
  ADD_INTERNAL_CALL(BoxCollider_GetOnTrigger);
  ADD_INTERNAL_CALL(BoxCollider_SetOnTrigger);

  // Begin Scene Manager
  ADD_INTERNAL_CALL(SceneManager_SetActive);
  ADD_INTERNAL_CALL(SceneManager_GetActiveIndex);

  // Begin Input
  ADD_INTERNAL_CALL(Input_IsKeyPressed);
  ADD_INTERNAL_CALL(Input_IsKeyPressedString);
  ADD_INTERNAL_CALL(Input_IsKeyReleased);
  ADD_INTERNAL_CALL(Input_IsKeyReleasedString);
  ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
  ADD_INTERNAL_CALL(Input_IsMouseButtonPressedString);
  ADD_INTERNAL_CALL(Input_IsMouseButtonReleased);
  ADD_INTERNAL_CALL(Input_IsMouseButtonReleasedString);
  ADD_INTERNAL_CALL(Input_GetMousePosition);
}

}  // namespace script_glue

}  // namespace eve
