find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

function(compile_shader TARGET)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "INCLUDE_DIR" "SOURCES")
    foreach(SOURCE ${arg_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)

        set(DEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME})

        add_custom_command(
            OUTPUT ${DEST_PATH}.spirv
            DEPENDS ${SOURCE}
            DEPFILE ${DEST_PATH}.d
            COMMAND
                ${glslc_executable}
                -I ${arg_INCLUDE_DIR}
                -MD -MF ${DEST_PATH}.d
                -o ${DEST_PATH}.spirv
                ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}
        )
        target_sources(${TARGET} PRIVATE ${DEST_PATH}.spirv)
    endforeach()
endfunction()
