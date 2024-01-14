set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(SOURCE_FILES
  Camera.cs
  Color.cs
  Components.cs
  Debug.cs
  Entity.cs
  Input.cs
  InternalCalls.cs
  KeyCode.cs
  Mathf.cs
  MouseCode.cs
  Rigidbody.cs
  SceneManager.cs
  Transform.cs
  Vector2.cs
  Vector3.cs
  Vector4.cs
)

add_library(script_core SHARED ${SOURCE_FILES})

# Set the target properties
set_property(TARGET script_core PROPERTY VS_CONFIGURATION_TYPE ClassLibrary)
set_property(TARGET script_core PROPERTY VS_DOTNET_REFERENCES
  "System"
  "System.Core"
)

set_property(TARGET script_core PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")
set_property(TARGET script_core PROPERTY DOTNET_TARGET_LANGUAGE_VERSION "latest")
