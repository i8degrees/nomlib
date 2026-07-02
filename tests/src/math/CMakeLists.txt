# nomlib-math module tests

add_executable(MathTest
  WIN32
  MathTest.cpp
)

#if( PLATFORM_WINDOWS )
#list( APPEND NOM_MATH_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
#endif( PLATFORM_WINDOWS )

add_dependencies(MathTest gtest)

target_include_directories(MathTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

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
