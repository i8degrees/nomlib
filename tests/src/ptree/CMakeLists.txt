# nomlib-ptree module tests

add_executable(ptree
  #WIN32 # Link SDL2main over WINMAIN for console apps
  ptree.cpp
)

add_dependencies(ptree gtest)

target_include_directories(ptree
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(ptree
  PUBLIC
    nomlib-core
    nomlib-ptree
  PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
)

# console app
if(TARGET SDL2::SDL2main)
  target_link_libraries(ptree
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ptree
                  "" # args
                  "ptree.cpp" )

