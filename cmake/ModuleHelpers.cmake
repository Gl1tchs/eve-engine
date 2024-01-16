function(add_module name)
  add_library(eve_${name} STATIC ${ARGN})
  add_library(eve::${name} ALIAS eve_${name})
endfunction()

function(add_module_executable name)
  add_executable(eve_${name} ${ARGN})
endfunction()

function(module_include_directories name) 
  target_include_directories(eve_${name} ${ARGN})
endfunction()

function(module_link_libraries name) 
  target_link_libraries(eve_${name} ${ARGN})
endfunction()

function(module_link_directories name) 
  target_link_directories(eve_${name} ${ARGN})
endfunction()

function(module_compile_definitions name) 
  target_compile_definitions(eve_${name} ${ARGN})
endfunction()

function(module_precompile_headers name) 
  target_precompile_headers(eve_${name} ${ARGN})
endfunction()

function(module_add_tests name)
  add_executable(eve_${name}-tests ${ARGN})

  target_link_libraries(eve_${name}-tests PRIVATE
    Catch2::Catch2WithMain
    eve::all
  )
  
  target_include_directories(eve_${name}-tests PRIVATE
    ${VENDOR_DIR}/Catch2/src
    ${ENGINE_DIR}
  )

  catch_discover_tests(eve_${name}-tests)
endfunction(module_add_tests)
