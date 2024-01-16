set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(SOURCE_FILES
  Components/BoxCollider.cs
  Components/CameraComponent.cs
  Components/Component.cs
  Components/Material.cs
  Components/Rigidbody.cs
  Components/ScriptComponent.cs
  Components/Transform.cs
  Application.cs
  Camera.cs
  Color.cs
  Debug.cs
  Entity.cs
  Exceptions.cs
  Input.cs
  Interop.cs
  KeyCode.cs
  Mathf.cs
  MouseCode.cs
  SceneManager.cs
  Vector2.cs
  Vector3.cs
  Vector4.cs
  Window.cs
)

add_library(script_core SHARED ${SOURCE_FILES})

# Set the target properties
set_property(TARGET script_core PROPERTY VS_CONFIGURATION_TYPE ClassLibrary)
set_property(TARGET script_core PROPERTY VS_DOTNET_REFERENCES
  "System"
  "System.Core"
  "System.Runtime.InteropServices"
)

set_property(TARGET script_core PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")
set_property(TARGET script_core PROPERTY DOTNET_TARGET_LANGUAGE_VERSION "latest")

set_property(TARGET script_core PROPERTY VS_DOTNET_DOCUMENTATION_FILE "${CMAKE_CURRENT_BINARY_DIR}/script_core.xml")
