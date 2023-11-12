find_program(shader_compiler_executable NAMES shader_compiler HINTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(NOT shader_compiler_executable)
    set(SHADER_COMPILER_FOUND FALSE)
else()
    set(SHADER_COMPILER_FOUND TRUE)
endif()

function(compile_shader TARGET)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "ENV;KIND;DEFINITIONS" "SOURCES")

    foreach(SOURCE ${arg_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)

        set(DEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME})

        add_custom_command(
            OUTPUT ${DEST_PATH}.spirv
            DEPENDS ${SOURCE}
            COMMAND
                ${shader_compiler_executable}
                -i ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}
                -o ${DEST_PATH}.spirv
                -e ${arg_ENV}
                -k ${arg_KIND}
                -d ${arg_DEFINITIONS}
        )

        target_sources(${TARGET} PRIVATE ${DEST_PATH}.spirv)
    endforeach()
endfunction()
