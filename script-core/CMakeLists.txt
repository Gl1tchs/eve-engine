cmake_minimum_required(VERSION 3.25)

project(script-core VERSION 0.1.0 LANGUAGES CSharp)

include(${CMAKE_SOURCE_DIR}/CMakeSettings.cmake)

set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/Camera.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Color.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Components.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Debug.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Entity.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Input.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/InternalCalls.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/KeyCode.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Mathf.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/MouseCode.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Rigidbody.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/SceneManager.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Transform.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Vector2.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Vector3.cs
    ${CMAKE_CURRENT_LIST_DIR}/src/Vector4.cs
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
