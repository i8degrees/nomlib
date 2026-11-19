# nomlib-math module tests

add_executable(MathTest
  #WIN32 # Link SDL2main over WINMAIN for console apps
  MathTest.cpp
)

add_dependencies(MathTest gtest)

target_include_directories(MathTest
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

# SDL2main for console apps
if(TARGET SDL2::SDL2main)
  target_link_libraries(MathTest
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(MathTest
  PUBLIC
    nomlib-core
    nomlib-math
  PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/MathTest
                  "" # args
                  "MathTest.cpp" )

