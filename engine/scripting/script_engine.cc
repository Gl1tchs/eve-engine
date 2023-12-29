// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script_engine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/threads.h>
#include <FileWatcher.hpp>

#include "core/file_system.h"
#include "core/instance.h"
#include "project/project.h"
#include "scripting/script_glue.h"

static std::unordered_map<std::string, ScriptFieldType> script_field_type_map =
    {
        {"System.Single", ScriptFieldType::kFloat},
        {"System.Double", ScriptFieldType::kDouble},
        {"System.Boolean", ScriptFieldType::kBool},
        {"System.Char", ScriptFieldType::kChar},
        {"System.Int16", ScriptFieldType::kShort},
        {"System.Int32", ScriptFieldType::kInt},
        {"System.Int64", ScriptFieldType::kLong},
        {"System.Byte", ScriptFieldType::kByte},
        {"System.UInt16", ScriptFieldType::kUShort},
        {"System.UInt32", ScriptFieldType::kUInt},
        {"System.UInt64", ScriptFieldType::kULong},

        {"EveEngine.Vector2", ScriptFieldType::kVector2},
        {"EveEngine.Vector3", ScriptFieldType::kVector3},
        {"EveEngine.Vector4", ScriptFieldType::kVector4},

        {"EveEngine.Entity", ScriptFieldType::kEntity},
};

static MonoAssembly* LoadMonoAssembly(const fs::path& assembly_path,
                                      bool load_pdb = false) {
  ScopedBuffer file_data = FileSystem::ReadFileBinary(assembly_path);

  // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
  MonoImageOpenStatus status;
  MonoImage* image = mono_image_open_from_data_full(
      file_data.As<char>(), file_data.Size(), 1, &status, 0);

  if (status != MONO_IMAGE_OK) {
    const char* error_message = mono_image_strerror(status);
    LOG_ERROR("{}", error_message);
    return nullptr;
  }

  if (load_pdb) {
    fs::path pdb_path = assembly_path;
    pdb_path.replace_extension(".pdb");

    if (fs::exists(pdb_path)) {
      ScopedBuffer pdb_file_data = FileSystem::ReadFileBinary(pdb_path);
      mono_debug_open_image_from_memory(
          image, pdb_file_data.As<const mono_byte>(), pdb_file_data.Size());
      LOG_INFO("Loaded PDB {}", pdb_path.string());
    }
  }

  std::string path_string = assembly_path.string();
  MonoAssembly* assembly =
      mono_assembly_load_from_full(image, path_string.c_str(), &status, 0);
  mono_image_close(image);

  return assembly;
}

void PrintAssemblyTypes(MonoAssembly* assembly) {
  MonoImage* image = mono_assembly_get_image(assembly);
  const MonoTableInfo* type_definitions_table =
      mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
  int num_types = mono_table_info_get_rows(type_definitions_table);

  for (int i = 0; i < num_types; i++) {
    uint32_t cols[MONO_TYPEDEF_SIZE];
    mono_metadata_decode_row(type_definitions_table, i, cols,
                             MONO_TYPEDEF_SIZE);

    const char* class_namespace =
        mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
    const char* name =
        mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
    LOG_TRACE("{}.{}", class_namespace, name);
  }
}

ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType) {
  std::string type_name = mono_type_get_name(monoType);

  auto it = script_field_type_map.find(type_name);
  if (it == script_field_type_map.end()) {
    LOG_ERROR("Unknown type: {}", type_name);
    return ScriptFieldType::kNone;
  }

  return it->second;
}

struct ScriptEngineData {
  MonoDomain* root_domain = nullptr;
  MonoDomain* app_domain = nullptr;

  MonoAssembly* core_assembly = nullptr;
  MonoImage* core_assembly_image = nullptr;

  MonoAssembly* app_assembly = nullptr;
  MonoImage* app_assembly_image = nullptr;

  fs::path core_assembly_path;
  fs::path app_assembly_path;

  ScriptClass entity_class;

  std::unordered_map<std::string, Ref<ScriptClass>> entity_classes;
  std::unordered_map<GUUID, Ref<ScriptInstance>> entity_instances;
  std::unordered_map<GUUID, ScriptFieldMap> entity_script_fields;

  std::vector<Scope<filewatch::FileWatch<std::string>>> script_file_watchers;
  bool assembly_reload_pending = false;

#ifdef EVE_DEBUG
  bool enable_debugging = true;
#else
  bool enable_debugging = false;
#endif

  Scene* scene_context = nullptr;
};

static ScriptEngineData* data = nullptr;

static void CreateProjectFiles();

static void BuildScripts();

static void OnScriptFileChanged(const std::string& path,
                                const filewatch::Event change_type) {
  if (data->scene_context && data->scene_context->IsRunning()) {
    return;
  }

  if (!data->assembly_reload_pending) {
    data->assembly_reload_pending = true;

    Instance::Get().EnqueueMain([&]() {
      for (auto& file_watcher : data->script_file_watchers) {
        file_watcher.reset();
      }

      CreateProjectFiles();
      BuildScripts();
      ScriptEngine::ReloadAssembly();
    });
  }
}

void ScriptEngine::Init() {
  data = new ScriptEngineData();

  InitMono();

  ScriptGlue::RegisterFunctions();

  CreateProjectFiles();
  BuildScripts();

  bool status = LoadAssembly("script-core.dll");
  if (!status) {
    LOG_ERROR("[ScriptEngine] Could not load script-core assembly.");
    return;
  }

  auto script_module_path =
      Project::GetProjectDirectory() /
      std::format("out/{}.dll", Project::GetProjectName());
  status = LoadAppAssembly(script_module_path);
  if (!status) {
    LOG_ERROR("[ScriptEngine] Could not load app assembly.");
    return;
  }

  LoadAssemblyClasses();

  ScriptGlue::RegisterComponents();

  // Retrieve and instantiate class
  data->entity_class = ScriptClass("EveEngine", "ScriptEntity", true);
}

void ScriptEngine::Shutdown() {
  ShutdownMono();
  delete data;
}

void ScriptEngine::InitMono() {
  CreateProjectFiles();

  mono_set_assemblies_path("mono/lib");

  if (data->enable_debugging) {
    const char* argv[2] = {
        "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,"
        "suspend=n,loglevel=3,logfile=MonoDebugger.log",
        "--soft-breakpoints"};

    mono_jit_parse_options(2, (char**)argv);
    mono_debug_init(MONO_DEBUG_FORMAT_MONO);
  }

  MonoDomain* root_domain = mono_jit_init("EveJITRuntime");
  ASSERT(root_domain);

  // Store the root domain pointer
  data->root_domain = root_domain;

  if (data->enable_debugging)
    mono_debug_domain_create(data->root_domain);

  mono_thread_set_main(mono_thread_current());
}

void ScriptEngine::ShutdownMono() {
  mono_domain_set(mono_get_root_domain(), false);

  mono_domain_unload(data->app_domain);
  data->app_domain = nullptr;

  mono_jit_cleanup(data->root_domain);
  data->root_domain = nullptr;
}

bool ScriptEngine::LoadAssembly(const fs::path& filepath) {
  // Create an App Domain
  char friendly_name[] = "EveScriptRuntime";
  data->app_domain = mono_domain_create_appdomain(friendly_name, nullptr);
  mono_domain_set(data->app_domain, true);

  data->core_assembly_path = filepath;
  data->core_assembly = LoadMonoAssembly(filepath, data->enable_debugging);
  if (data->core_assembly == nullptr)
    return false;

  data->core_assembly_image = mono_assembly_get_image(data->core_assembly);
  return true;
}

bool ScriptEngine::LoadAppAssembly(const fs::path& filepath) {
  data->app_assembly_path = filepath;
  data->app_assembly = LoadMonoAssembly(filepath, data->enable_debugging);
  if (data->app_assembly == nullptr)
    return false;

  data->app_assembly_image = mono_assembly_get_image(data->app_assembly);

  for (const auto& entry :
       fs::recursive_directory_iterator(Project::GetAssetDirectory())) {
    if (entry.is_directory()) {
      if (std::any_of(fs::directory_iterator(entry.path()),
                      fs::directory_iterator(), [](const auto& subentry) {
                        return subentry.path().extension() == ".cs";
                      })) {
        data->script_file_watchers.push_back(
            CreateScope<filewatch::FileWatch<std::string>>(
                entry.path().string(), OnScriptFileChanged));
        data->assembly_reload_pending = false;
      }
    }
  }

  return true;
}

void ScriptEngine::ReloadAssembly() {
  mono_domain_set(mono_get_root_domain(), false);

  mono_domain_unload(data->app_domain);

  LoadAssembly(data->core_assembly_path);
  LoadAppAssembly(data->app_assembly_path);
  LoadAssemblyClasses();

  ScriptGlue::RegisterComponents();

  // Retrieve and instantiate class
  data->entity_class = ScriptClass("EveEngine", "ScriptEntity", true);
}

bool ScriptEngine::EntityClassExists(const std::string& fullClassName) {
  return data->entity_classes.find(fullClassName) != data->entity_classes.end();
}

void ScriptEngine::OnCreateEntity(Entity entity) {
  const auto& sc = entity.GetComponent<ScriptComponent>();
  if (ScriptEngine::EntityClassExists(sc.class_name)) {
    GUUID entity_id = entity.GetUUID();

    Ref<ScriptInstance> instance =
        CreateRef<ScriptInstance>(data->entity_classes[sc.class_name], entity);
    data->entity_instances[entity_id] = instance;

    // Copy field values
    if (data->entity_script_fields.find(entity_id) !=
        data->entity_script_fields.end()) {
      const ScriptFieldMap& field_map =
          data->entity_script_fields.at(entity_id);
      for (const auto& [name, field_instance] : field_map)
        instance->SetFieldValueInternal(name, field_instance.buffer_);
    }

    instance->InvokeOnCreate();
  }
}

void ScriptEngine::OnUpdateEntity(Entity entity, float ds) {
  GUUID entity_uuid = entity.GetUUID();
  if (data->entity_instances.find(entity_uuid) !=
      data->entity_instances.end()) {
    Ref<ScriptInstance> instance = data->entity_instances[entity_uuid];
    instance->InvokeOnUpdate(ds);
  } else {
    LOG_ERROR("Could not find ScriptInstance for entity {}",
              (uint64_t)entity_uuid);
  }
}

Scene* ScriptEngine::GetSceneContext() {
  return data->scene_context;
}

Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(GUUID entity_id) {
  auto it = data->entity_instances.find(entity_id);
  if (it == data->entity_instances.end()) {
    return nullptr;
  }

  return it->second;
}

ScriptClass ScriptEngine::GetEntityClass() {
  return data->entity_class;
}

Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name) {
  if (data->entity_classes.find(name) == data->entity_classes.end())
    return nullptr;

  return data->entity_classes.at(name);
}

void ScriptEngine::OnRuntimeStart(Scene* scene) {
  data->scene_context = scene;
}

void ScriptEngine::OnRuntimeStop() {
  data->scene_context = nullptr;

  data->entity_instances.clear();
}

std::unordered_map<std::string, Ref<ScriptClass>>
ScriptEngine::GetEntityClasses() {
  return data->entity_classes;
}

ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity) {
  ASSERT(entity);

  GUUID entity_id = entity.GetUUID();
  return data->entity_script_fields[entity_id];
}

void ScriptEngine::LoadAssemblyClasses() {
  data->entity_classes.clear();

  const MonoTableInfo* type_definitions_table =
      mono_image_get_table_info(data->app_assembly_image, MONO_TABLE_TYPEDEF);
  int num_types = mono_table_info_get_rows(type_definitions_table);
  MonoClass* entity_class = mono_class_from_name(data->core_assembly_image,
                                                 "EveEngine", "ScriptEntity");

  for (int32_t i = 0; i < num_types; i++) {
    uint32_t cols[MONO_TYPEDEF_SIZE];
    mono_metadata_decode_row(type_definitions_table, i, cols,
                             MONO_TYPEDEF_SIZE);

    const char* class_namespace = mono_metadata_string_heap(
        data->app_assembly_image, cols[MONO_TYPEDEF_NAMESPACE]);
    const char* class_name = mono_metadata_string_heap(data->app_assembly_image,
                                                       cols[MONO_TYPEDEF_NAME]);
    std::string full_name;
    if (strlen(class_namespace) != 0) {
      full_name = std::format("{}.{}", class_namespace, class_name);
    } else {
      full_name = class_name;
    }

    MonoClass* mono_class = mono_class_from_name(data->app_assembly_image,
                                                 class_namespace, class_name);

    if (mono_class == entity_class) {
      continue;
    }

    bool is_entity = mono_class_is_subclass_of(mono_class, entity_class, false);
    if (!is_entity) {
      continue;
    }

    Ref<ScriptClass> script_class =
        CreateRef<ScriptClass>(class_namespace, class_name);
    data->entity_classes[full_name] = script_class;

    // This routine is an iterator routine for retrieving the fields in a class.
    // You must pass a gpointer that points to zero and is treated as an opaque handle
    // to iterate over all of the elements. When no more values are available, the return value is NULL.

    int fieldCount = mono_class_num_fields(mono_class);
    LOG_WARNING("{} has {} fields:", class_name, fieldCount);
    void* iterator = nullptr;
    while (MonoClassField* field =
               mono_class_get_fields(mono_class, &iterator)) {
      const char* field_name = mono_field_get_name(field);
      uint32_t flags = mono_field_get_flags(field);
      if (flags & FIELD_ATTRIBUTE_PUBLIC) {
        MonoType* type = mono_field_get_type(field);
        ScriptFieldType field_type = MonoTypeToScriptFieldType(type);
        LOG_WARNING("  {} ({})", field_name,
                    ScriptFieldTypeToString(field_type));

        script_class->fields_[field_name] = {field_type, field_name, field};
      }
    }
  }
}

MonoImage* ScriptEngine::GetCoreAssemblyImage() {
  return data->core_assembly_image;
}

MonoImage* ScriptEngine::GetAppAssemblyImage() {
  return data->app_assembly_image;
}

MonoObject* ScriptEngine::GetManagedInstance(GUUID uuid) {
  ASSERT(data->entity_instances.find(uuid) != data->entity_instances.end());
  return data->entity_instances.at(uuid)->GetManagedObject();
}

MonoString* ScriptEngine::CreateString(const char* string) {
  return mono_string_new(data->app_domain, string);
}

MonoObject* ScriptEngine::InstantiateClass(MonoClass* mono_class) {
  MonoObject* instance = mono_object_new(data->app_domain, mono_class);
  mono_runtime_object_init(instance);
  return instance;
}

void CreateProjectFiles() {
  std::ofstream csproj_file(Project::GetProjectDirectory() /
                            (Project::GetProjectName() + ".csproj"));

  if (!csproj_file.is_open()) {
    LOG_ERROR("Error: Unable to create .csproj file");
    return;
  }

  const std::string project_dir = Project::GetProjectDirectory().string();
  csproj_file << std::format(R"(<?xml version="1.0" encoding="utf-8"?>
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Library</OutputType>
    <TargetFramework>net8.0</TargetFramework>
    <OutputPath>{}\out</OutputPath>
    <AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
  </PropertyGroup>
  <ItemGroup>
)",
                             project_dir);

  for (const auto& entry :
       fs::recursive_directory_iterator(Project::GetAssetDirectory())) {
    if (entry.is_regular_file() && entry.path().extension() == ".cs") {
      csproj_file << "       <Compile Include=\"" << entry.path().string()
                  << "\" />";
    }
  }

  csproj_file << R"(</ItemGroup>
  <ItemGroup>
    <Reference Include="script-core">
      <HintPath>)"
              << fs::current_path().string() << R"(\script-core.dll</HintPath>
    </Reference>
  </ItemGroup>
</Project>)";

  LOG_TRACE("{}.csproj file generated successfully.",
            Project::GetProjectName());
}

void BuildScripts() {
  fs::path csproj_path =
      Project::GetProjectDirectory() / (Project::GetProjectName() + ".csproj");
  if (!fs::exists(csproj_path)) {
    LOG_WARNING("Unable to find project files creating...");
    CreateProjectFiles();
  }

  int result =
      std::system(std::format("dotnet build {}", csproj_path.string()).c_str());
  if (result == 0) {
    LOG_INFO("Scripts built successfully!");
  } else {
    LOG_ERROR("Script building process failed!");
  }
}

const char* ScriptFieldTypeToString(ScriptFieldType field_type) {
  switch (field_type) {
    case ScriptFieldType::kNone:
      return "None";
    case ScriptFieldType::kFloat:
      return "Float";
    case ScriptFieldType::kDouble:
      return "Double";
    case ScriptFieldType::kBool:
      return "Bool";
    case ScriptFieldType::kChar:
      return "Char";
    case ScriptFieldType::kByte:
      return "Byte";
    case ScriptFieldType::kShort:
      return "Short";
    case ScriptFieldType::kInt:
      return "Int";
    case ScriptFieldType::kLong:
      return "Long";
    case ScriptFieldType::kUByte:
      return "UByte";
    case ScriptFieldType::kUShort:
      return "UShort";
    case ScriptFieldType::kUInt:
      return "UInt";
    case ScriptFieldType::kULong:
      return "ULong";
    case ScriptFieldType::kVector2:
      return "Vector2";
    case ScriptFieldType::kVector3:
      return "Vector3";
    case ScriptFieldType::kVector4:
      return "Vector4";
    case ScriptFieldType::kEntity:
      return "ScriptEntity";
  }
  ASSERT(false, "Unknown ScriptFieldType");
  return "None";
}

ScriptFieldType ScriptFieldTypeFromString(std::string_view field_type) {
  if (field_type == "None")
    return ScriptFieldType::kNone;
  if (field_type == "Float")
    return ScriptFieldType::kFloat;
  if (field_type == "Double")
    return ScriptFieldType::kDouble;
  if (field_type == "Bool")
    return ScriptFieldType::kBool;
  if (field_type == "Char")
    return ScriptFieldType::kChar;
  if (field_type == "Byte")
    return ScriptFieldType::kByte;
  if (field_type == "Short")
    return ScriptFieldType::kShort;
  if (field_type == "Int")
    return ScriptFieldType::kInt;
  if (field_type == "Long")
    return ScriptFieldType::kLong;
  if (field_type == "UByte")
    return ScriptFieldType::kUByte;
  if (field_type == "UShort")
    return ScriptFieldType::kUShort;
  if (field_type == "UInt")
    return ScriptFieldType::kUInt;
  if (field_type == "ULong")
    return ScriptFieldType::kULong;
  if (field_type == "Vector2")
    return ScriptFieldType::kVector2;
  if (field_type == "Vector3")
    return ScriptFieldType::kVector3;
  if (field_type == "Vector4")
    return ScriptFieldType::kVector4;
  if (field_type == "ScriptEntity")
    return ScriptFieldType::kEntity;

  ASSERT(false, "Unknown ScriptFieldType");
  return ScriptFieldType::kNone;
}
