function(expose_headers TARGET SOURCE_DIR)
    # Get headers to expose
    file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/*.inl")

    # Set destination directory
    get_filename_component(DIRECTORY_NAME ${SOURCE_DIR} NAME)
    set(DESTINATION_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include/${DIRECTORY_NAME}")

    # Copy headers maintaining directory structure
    foreach(HEADER_FILE ${HEADERS})
        # Get the path relative to the source directory
        file(RELATIVE_PATH REL_PATH ${SOURCE_DIR} ${HEADER_FILE})
        get_filename_component(DIR ${REL_PATH} DIRECTORY)

        # Create the directory structure in the build directory
        file(MAKE_DIRECTORY ${DESTINATION_DIR}/${DIR})

        # Copy each header file to the corresponding directory in the build/bin/include
        add_custom_command(TARGET ${TARGET} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${HEADER_FILE} ${DESTINATION_DIR}/${REL_PATH}
            COMMENT "Copying ${HEADER_FILE} to ${DESTINATION_DIR}/${REL_PATH}"
        )
    endforeach()
endfunction()
