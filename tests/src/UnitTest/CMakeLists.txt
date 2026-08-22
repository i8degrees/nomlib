# nomlib-unit-test module tests

add_executable(UnitTestFrameworkTest
  #WIN32 # Link SDL2main over WINMAIN for console apps
  UnitTestFrameworkTest.cpp
)

add_dependencies(UnitTestFrameworkTest gtest)

target_include_directories(UnitTestFrameworkTest
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
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
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/UnitTestFrameworkTest
                  "" # args
                  "UnitTestFrameworkTest.cpp" )

