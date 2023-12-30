cmake_minimum_required(VERSION 3.25)

project(script-core VERSION 0.1.0 LANGUAGES CSharp)

include(${CMAKE_SOURCE_DIR}/CMakeSettings.cmake)

set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Camera.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Components.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/ScriptEntity.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Input.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/InternalCalls.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/KeyCode.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Mathf.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/MouseCode.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/SceneManager.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Transform.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Vector2.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Vector3.cs
    ${CMAKE_CURRENT_LIST_DIR}/EveEngine/Vector4.cs
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(script-core SHARED ${SOURCE_FILES})

# Set the target properties
set_property(TARGET script-core PROPERTY VS_CONFIGURATION_TYPE ClassLibrary)
set_property(TARGET script-core PROPERTY VS_DOTNET_REFERENCES
    "System"
    "System.Core"
)

set_property(TARGET script-core PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")
set_property(TARGET script-core PROPERTY DOTNET_TARGET_LANGUAGE_VERSION "latest")
