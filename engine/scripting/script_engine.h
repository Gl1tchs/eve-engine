// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scripting/script.h"

namespace eve {

class ScriptEngine {
 public:
  static void Init(bool is_runtime = false);

  static void Reinit();

  static void Shutdown();

  static bool IsInitialized();

  static bool LoadAssembly(const fs::path& filepath);
  static bool LoadAppAssembly(const fs::path& filepath);

  static void ReloadAssembly();

  static void OnRuntimeStart(Scene* scene);
  static void OnRuntimeStop();

  static bool EntityClassExists(const std::string& full_class_name);

  static void CreateEntityInstance(Entity entity);

  static void SetEntityManagedFieldValues(Entity entity);

  static void InvokeCreateEntity(Entity entity);

  static void InvokeUpdateEntity(Entity entity, float ds);

  static void InvokeDestroyEntity(Entity entity);

  static Scene* GetSceneContext();
  static Ref<ScriptInstance> GetEntityScriptInstance(UUID entity_id);

  static ScriptClass GetEntityClass();
  static Ref<ScriptClass> GetEntityClass(const std::string& name);
  static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

  static ScriptFieldMap& GetScriptFieldMap(Entity entity);

  static MonoImage* GetCoreAssemblyImage();

  static MonoImage* GetAppAssemblyImage();

  static MonoObject* GetManagedInstance(UUID uuid);

  static MonoString* CreateMonoString(const char* string);

  static void GenerateProjectFiles();

  static void BuildScripts();

 private:
  static void InitMono();
  static void ShutdownMono();

  static MonoObject* InstantiateClass(MonoClass* mono_class);

  static void LoadAssemblyClasses();

  friend class ScriptClass;
};

[[nodiscard]] bool IsManagedScriptFieldType(ScriptFieldType field_type);

const char* SerializeScriptField(ScriptFieldType field_type);

ScriptFieldType DeserializeScriptField(std::string_view field_type);

}  // namespace eve
