# nomlib-unit-test module tests

add_executable( UnitTestFrameworkTest "UnitTestFrameworkTest.cpp" )

target_link_libraries( UnitTestFrameworkTest nomlib-unit-test )

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/UnitTestFrameworkTest
                  "" # args
                  "UnitTestFrameworkTest.cpp" )
