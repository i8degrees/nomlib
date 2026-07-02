add_executable(nom_macros
  WIN32 # Link SDL2main over WinMain
  macros.cpp
)

set_target_properties(nom_macros
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

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
