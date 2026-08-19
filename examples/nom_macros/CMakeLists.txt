add_executable(nom_macros
  WIN32 # Link SDL2main over WinMain
  macros.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_macros "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_macros "@origin/../lib")
endif()

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_macros
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_macros
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
)

target_link_libraries(nom_macros
  PUBLIC
    nomlib-core
  PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

install(TARGETS nom_macros
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For development:
add_custom_command(TARGET nom_macros POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL2::SDL2>
  $<TARGET_FILE_DIR:nom_macros>
)

# For distribution:
install(FILES $<TARGET_FILE:SDL2::SDL2>
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  RENAME nom_$<TARGET_FILE_NAME:SDL2::SDL2>
  COMPONENT Examples
)

copy_framework_dep(nom_macros
  nomlib-core
)

