# nomlib-unit-test module tests

add_executable(UnitTestFrameworkTest
  WIN32
  UnitTestFrameworkTest.cpp
)

add_dependencies(UnitTestFrameworkTest gtest)

target_include_directories(UnitTestFrameworkTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(UnitTestFrameworkTest
  PUBLIC
  nomlib-unit-test
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/UnitTestFrameworkTest
                  "" # args
                  "UnitTestFrameworkTest.cpp" )

