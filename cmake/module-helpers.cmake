function(add_module name)
    add_library(eve_${name} STATIC ${ARGN})
    add_library(eve::${name} ALIAS eve_${name})
endfunction()

function(add_module_executable name)
    add_executable(eve_${name} ${ARGN})
endfunction()

function(module_include_directories name visibility) 
    target_include_directories(eve_${name} ${visibility} ${ARGN})
endfunction()

function(module_link_libraries name visibility) 
    target_link_libraries(eve_${name} ${visibility} ${ARGN})
endfunction()

function(module_compile_definitions name visibility) 
    target_compile_definitions(eve_${name} ${visibility} ${ARGN})
endfunction()

function(module_precompile_headers name visibility) 
    target_precompile_headers(eve_${name} ${visibility} ${ARGN})
endfunction()
