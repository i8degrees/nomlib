if( NOM_BUILD_VTEST_TESTS )

  add_executable  (
                    VisualUnitTestFrameworkTest
                    ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/VisualUnitTestFrameworkTest.cpp
                  )

  target_link_libraries(  VisualUnitTestFrameworkTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  add_test  (
              # test name
              VisualUnitTestFrameworkTestGenRefImages,
              # executable name
              VisualUnitTestFrameworkTest
              "-r" # args
            )

  add_test  (
              # test name
              VisualUnitTestFrameworkTestCompareImages,
              # executable name
              VisualUnitTestFrameworkTest
              "" # args
            )

  add_executable  (
                    VisualUnitTestFrameworkNonDefaultCtorTest
                    ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/VisualUnitTestFrameworkNonDefaultCtorTest.cpp
                  )

  target_link_libraries(  VisualUnitTestFrameworkNonDefaultCtorTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  add_test  (
              # test name
              VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages,
              # executable name
              VisualUnitTestFrameworkNonDefaultCtorTest
              "-r" # args
            )

  add_test  (
              # test name
              VisualUnitTestFrameworkNonDefaultCtorTestCompareImages,
              # executable name
              VisualUnitTestFrameworkNonDefaultCtorTest
              "" # args
            )

  add_executable( ImageTestSetTest
                  ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/ImageTestSetTest.cpp )

  target_link_libraries(  ImageTestSetTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS(  ImageTestSetTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/ImageTestSetTest.cpp
                  )

  add_executable  (
                    ImageDiffTest
                    ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/ImageDiffTest.cpp )

  target_link_libraries(  ImageDiffTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS(  ImageDiffTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/VisualUnitTest/ImageDiffTest.cpp )

  install(  DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/ImageDiffTest"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_VTEST_TESTS )
