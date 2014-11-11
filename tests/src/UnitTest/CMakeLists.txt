if( NOM_BUILD_UNIT_TEST_TESTS )

  add_executable( UnitTestFrameworkTest
                  ${NOM_TESTS_SOURCE_DIR}/UnitTest/UnitTestFrameworkTest.cpp )

  target_link_libraries( UnitTestFrameworkTest UnitTest )

  GTEST_ADD_TESTS(  UnitTestFrameworkTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/UnitTest/UnitTestFrameworkTest.cpp )

endif( NOM_BUILD_UNIT_TEST_TESTS )
