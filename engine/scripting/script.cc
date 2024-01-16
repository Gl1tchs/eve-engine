// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/object.h>
#include <mono/metadata/threads.h>

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

MonoMethod* ScriptClass::GetMethod(const std::string& name,
                                   int parameterCount) {
  return mono_class_get_method_from_name(mono_class_, name.c_str(),
                                         parameterCount);
}

MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method,
                                      void** params) {
  MonoObject* exception = nullptr;
  return mono_runtime_invoke(method, instance, params, &exception);
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
    UUID entityID = entity.GetUUID();
    void* param = &entityID;
    script_class_->InvokeMethod(instance_, ctor_, &param);
  }
}

void ScriptInstance::InvokeOnCreate() {
  if (on_create_method_)
    script_class_->InvokeMethod(instance_, on_create_method_);
}

void ScriptInstance::InvokeOnUpdate(float ts) {
  if (on_update_method_) {
    void* param = &ts;
    script_class_->InvokeMethod(instance_, on_update_method_, &param);
  }
}

void ScriptInstance::InvokeOnDestroy() {
  if (on_destroy_method_)
    script_class_->InvokeMethod(instance_, on_destroy_method_);
}

bool ScriptInstance::GetFieldValueInternal(const std::string& name,
                                           void* buffer) {
  const auto& fields = script_class_->GetFields();
  auto it = fields.find(name);
  if (it == fields.end())
    return false;

  const ScriptField& field = it->second;
  mono_field_get_value(instance_, field.class_field, buffer);
  return true;
}

bool ScriptInstance::SetFieldValueInternal(const std::string& name,
                                           const void* value) {
  const auto& fields = script_class_->GetFields();
  auto it = fields.find(name);
  if (it == fields.end())
    return false;

  const ScriptField& field = it->second;
  mono_field_set_value(instance_, field.class_field, (void*)value);
  return true;
}

}  // namespace eve
