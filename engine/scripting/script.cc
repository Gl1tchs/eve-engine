// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/object.h>
#include <mono/metadata/threads.h>

#include "scene/scene_manager.h"
#include "scripting/script_engine.h"

namespace eve {

ScriptClass::ScriptClass(const std::string& class_namespace,
                         const std::string& class_name, bool is_core)
    : class_namespace_(class_namespace), class_name_(class_name) {
  mono_class_ =
      mono_class_from_name(is_core ? ScriptEngine::GetCoreAssemblyImage()
                                   : ScriptEngine::GetAppAssemblyImage(),
                           class_namespace.c_str(), class_name.c_str());
}

MonoObject* ScriptClass::Instantiate() {
  return ScriptEngine::InstantiateClass(mono_class_);
}

MonoMethod* ScriptClass::GetMethod(const std::string& name, int param_count) {
  return mono_class_get_method_from_name(mono_class_, name.c_str(),
                                         param_count);
}

MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method,
                                      void** params) {
  MonoObject* exception = nullptr;
  return mono_runtime_invoke(method, instance, params, &exception);
}

ScriptInstance::ScriptInstance(Ref<ScriptClass> script_class,
                               MonoObject* managed_object)
    : script_class_(script_class) {
  instance_ = managed_object;

  ctor_ = ScriptEngine::GetEntityClass().GetMethod(".ctor", 1);
  on_create_method_ = script_class->GetMethod("OnCreate", 0);
  on_update_method_ = script_class->GetMethod("OnUpdate", 1);
  on_destroy_method_ = script_class->GetMethod("OnDestroy", 0);
}

ScriptInstance::ScriptInstance(Ref<ScriptClass> script_class, Entity entity)
    : script_class_(script_class) {
  instance_ = script_class->Instantiate();

  ctor_ = ScriptEngine::GetEntityClass().GetMethod(".ctor", 1);
  on_create_method_ = script_class->GetMethod("OnCreate", 0);
  on_update_method_ = script_class->GetMethod("OnUpdate", 1);
  on_destroy_method_ = script_class->GetMethod("OnDestroy", 0);

  // Call Entity constructor
  {
    UUID entity_id = entity.GetUUID();
    void* param = &entity_id;
    script_class_->InvokeMethod(instance_, ctor_, &param);
  }
}

void ScriptInstance::InvokeOnCreate() {
  if (on_create_method_) {
    script_class_->InvokeMethod(instance_, on_create_method_);
  }
}

void ScriptInstance::InvokeOnUpdate(float ds) {
  if (on_update_method_) {
    void* param = &ds;
    script_class_->InvokeMethod(instance_, on_update_method_, &param);
  }
}

void ScriptInstance::InvokeOnDestroy() {
  if (on_destroy_method_) {
    script_class_->InvokeMethod(instance_, on_destroy_method_);
  }
}

bool ScriptInstance::GetFieldValueInternal(const std::string& name,
                                           void* buffer) {
  const auto& fields = script_class_->GetFields();
  auto it = fields.find(name);
  if (it == fields.end()) {
    return false;
  }

  const ScriptField& field = it->second;

  if (field.type != ScriptFieldType::kEntity) {
    mono_field_get_value(instance_, field.class_field, buffer);
  } else {
    MonoObject* entity_object = (MonoObject*)malloc(sizeof(MonoObject));
    mono_field_get_value(instance_, field.class_field, entity_object);

    if (!entity_object) {
      return false;
    }

    MonoProperty* id_property = mono_class_get_property_from_name(
        ScriptEngine::GetEntityClass().GetMonoClass(), "Id");

    if (!id_property) {
      return false;
    }

    MonoObject* id_value =
        mono_property_get_value(id_property, entity_object, nullptr, nullptr);

    memcpy(buffer, mono_object_unbox(id_value), sizeof(UUID));

    free(entity_object);
  }

  return true;
}

bool ScriptInstance::SetFieldValueInternal(const std::string& name,
                                           const void* value) {
  const auto& fields = script_class_->GetFields();
  auto it = fields.find(name);
  if (it == fields.end()) {
    return false;
  }

  const ScriptField& field = it->second;

  void* data = (void*)value;

  if (field.type == ScriptFieldType::kEntity) {
    UUID uuid = *(UUID*)value;

    Entity entity = SceneManager::GetActive()->TryGetEntityByUUID(uuid);
    if (!entity) {
      EVE_LOG_ENGINE_WARNING("Unable to set entity instance of {}.",
                             (uint64_t)uuid);
      return false;
    }

    MonoObject* managed_instance = ScriptEngine::GetManagedInstance(uuid);
    if (managed_instance) {
      data = (void*)managed_instance;
    } else {
      EVE_LOG_ENGINE_WARNING(
          "Entity {}, does not have an managed script instance. Using default "
          "instead.",
          entity.GetName());

      ScriptInstance entity_instance(
          CreateRef<ScriptClass>(ScriptEngine::GetEntityClass()), entity);

      data = (void*)entity_instance.instance_;
    }
  }

  mono_field_set_value(instance_, field.class_field, data);

  return true;
}

size_t GetFieldTypeSize(ScriptFieldType type) {
  switch (type) {
    case ScriptFieldType::kNone:
      return 0;
    case ScriptFieldType::kFloat:
      return sizeof(float);
    case ScriptFieldType::kDouble:
      return sizeof(double);
    case ScriptFieldType::kBool:
      return sizeof(bool);
    case ScriptFieldType::kChar:
      return sizeof(char);
    case ScriptFieldType::kByte:
      return sizeof(int8_t);
    case ScriptFieldType::kShort:
      return sizeof(int16_t);
    case ScriptFieldType::kInt:
      return sizeof(int32_t);
    case ScriptFieldType::kLong:
      return sizeof(int64_t);
    case ScriptFieldType::kUByte:
      return sizeof(uint8_t);
    case ScriptFieldType::kUShort:
      return sizeof(uint16_t);
    case ScriptFieldType::kUInt:
      return sizeof(uint32_t);
    case ScriptFieldType::kULong:
      return sizeof(uint64_t);
    case ScriptFieldType::kVector2:
      return 2 * sizeof(float);
    case ScriptFieldType::kVector3:
      return 3 * sizeof(float);
    case ScriptFieldType::kVector4:
      return 4 * sizeof(float);
    case ScriptFieldType::kColor:
      return 4 * sizeof(float);
    case ScriptFieldType::kEntity:
      return sizeof(UUID);
    default:
      return 0;
  }
}

}  // namespace eve
