// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scripting/script.h"

class ScriptEngine {
 public:
  static void Init(bool is_runtime = false);

  static bool IsInitialized();

  static void Shutdown();

  static bool LoadAssembly(const fs::path& filepath);
  static bool LoadAppAssembly(const fs::path& filepath);

  static void ReloadAssembly();

  static void OnRuntimeStart(Scene* scene);
  static void OnRuntimeStop();

  static bool EntityClassExists(const std::string& full_class_name);
  static void OnCreateEntity(Entity entity);
  static void OnUpdateEntity(Entity entity, float ds);

  static Scene* GetSceneContext();
  static Ref<ScriptInstance> GetEntityScriptInstance(GUUID entity_id);

  static ScriptClass GetEntityClass();
  static Ref<ScriptClass> GetEntityClass(const std::string& name);
  static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
  static ScriptFieldMap& GetScriptFieldMap(Entity entity);

  static MonoImage* GetCoreAssemblyImage();

  static MonoImage* GetAppAssemblyImage();

  static MonoObject* GetManagedInstance(GUUID uuid);

  static MonoString* CreateString(const char* string);

 private:
  static void InitMono();
  static void ShutdownMono();

  static MonoObject* InstantiateClass(MonoClass* mono_class);
  static void LoadAssemblyClasses();

  friend class ScriptClass;
  friend class ScriptGlue;
};

const char* ScriptFieldTypeToString(ScriptFieldType field_type);

ScriptFieldType ScriptFieldTypeFromString(std::string_view field_type);
