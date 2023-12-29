// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/entity.h"

extern "C" {
typedef struct _MonoClass MonoClass;
typedef struct _MonoObject MonoObject;
typedef struct _MonoMethod MonoMethod;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoImage MonoImage;
typedef struct _MonoClassField MonoClassField;
typedef struct _MonoString MonoString;
}

enum class ScriptFieldType {
  kNone = 0,
  kFloat,
  kDouble,
  kBool,
  kChar,
  kByte,
  kShort,
  kInt,
  kLong,
  kUByte,
  kUShort,
  kUInt,
  kULong,
  kVector2,
  kVector3,
  kVector4,
  kScriptEntity
};

struct ScriptField {
  ScriptFieldType type;
  std::string name;

  MonoClassField* class_field;
};

// ScriptField + data storage
struct ScriptFieldInstance {
  ScriptField field;

  ScriptFieldInstance() { memset(buffer_, 0, sizeof(buffer_)); }

  template <typename T>
  T GetValue() {
    static_assert(sizeof(T) <= 16, "Type too large!");
    return *(T*)buffer_;
  }

  template <typename T>
  void SetValue(T value) {
    static_assert(sizeof(T) <= 16, "Type too large!");
    memcpy(buffer_, &value, sizeof(T));
  }

 private:
  uint8_t buffer_[16];

  friend class ScriptEngine;
  friend class ScriptInstance;
};

using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

class ScriptClass {
 public:
  ScriptClass() = default;
  ScriptClass(const std::string& class_namespace, const std::string& class_name,
              bool is_core = false);

  MonoObject* Instantiate();
  MonoMethod* GetMethod(const std::string& name, int param_count);
  MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method,
                           void** params = nullptr);

  const std::unordered_map<std::string, ScriptField>& GetFields() const {
    return fields_;
  }

 private:
  std::string class_namespace_;
  std::string class_name_;

  std::unordered_map<std::string, ScriptField> fields_;

  MonoClass* mono_class_ = nullptr;

  friend class ScriptEngine;
};

class ScriptInstance {
 public:
  ScriptInstance(Ref<ScriptClass> script_class, Entity entity);

  void InvokeOnCreate();
  void InvokeOnUpdate(float ts);
  void InvokeOnDestroy();

  Ref<ScriptClass> GetScriptClass() { return script_class_; }

  template <typename T>
  T GetFieldValue(const std::string& name) {
    static_assert(sizeof(T) <= 16, "Type too large!");

    bool success = GetFieldValueInternal(name, field_value_buffer_);
    if (!success)
      return T();

    return *(T*)field_value_buffer_;
  }

  template <typename T>
  void SetFieldValue(const std::string& name, T value) {
    static_assert(sizeof(T) <= 16, "Type too large!");

    SetFieldValueInternal(name, &value);
  }

  MonoObject* GetManagedObject() { return instance_; }

 private:
  bool GetFieldValueInternal(const std::string& name, void* buffer);
  bool SetFieldValueInternal(const std::string& name, const void* value);

 private:
  Ref<ScriptClass> script_class_;

  MonoObject* instance_ = nullptr;
  MonoMethod* ctor_ = nullptr;
  MonoMethod* on_create_method_ = nullptr;
  MonoMethod* on_update_method_ = nullptr;
  MonoMethod* on_destroy_method_ = nullptr;

  inline static char field_value_buffer_[16];

  friend class ScriptEngine;
  friend struct ScriptFieldInstance;
};
