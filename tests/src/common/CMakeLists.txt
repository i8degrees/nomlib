if( NOM_BUILD_COMMON_TESTS )

    add_executable  (
                      UnitTestFrameworkTest
                      ${NOM_TESTS_SOURCE_DIR}/common/UnitTestFrameworkTest.cpp
                    )

    target_link_libraries( UnitTestFrameworkTest UnitTest )

    GTEST_ADD_TESTS (
                      UnitTestFrameworkTest
                      "UnitTestFrameworkTest"
                      ${NOM_TESTS_SOURCE_DIR}/common/UnitTestFrameworkTest.cpp
                    )

endif( NOM_BUILD_COMMON_TESTS )

if( NOM_BUILD_VTEST_TESTS )

  add_executable  (
                    VisualUnitTestFrameworkTest
                    ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestFrameworkTest.cpp
                  )

    target_link_libraries(  VisualUnitTestFrameworkTest
                            UnitTest
                            VisualUnitTest )

    add_test  (
                # test name
                VisualUnitTestFrameworkTestGenRefImages,
                # executable name
                VisualUnitTestFrameworkTest
                # args
                "-r"
              )

    add_test  (
                # test name
                VisualUnitTestFrameworkTestCompareImages,
                # executable name
                VisualUnitTestFrameworkTest
                # args
                ""
              )

    add_executable  (
                      VisualUnitTestFrameworkNonDefaultCtorTest
                      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestFrameworkNonDefaultCtorTest.cpp
                    )

    target_link_libraries(  VisualUnitTestFrameworkNonDefaultCtorTest
                            UnitTest
                            VisualUnitTest )

    add_test  (
                # test name
                VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages,
                # executable name
                VisualUnitTestFrameworkNonDefaultCtorTest
                # args
                "-r"
              )

    add_test  (
                # test name
                VisualUnitTestFrameworkNonDefaultCtorTestCompareImages,
                # executable name
                VisualUnitTestFrameworkNonDefaultCtorTest
                # args
                ""
              )

    add_executable  (
                      ImageTestSetTest
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageTestSetTest.cpp
                    )

    target_link_libraries( ImageTestSetTest UnitTest VisualUnitTest )

    GTEST_ADD_TESTS (
                      ImageTestSetTest
                      "ImageTestSetTest"
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageTestSetTest.cpp
                    )

    add_executable  (
                      ImageDiffTest
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiffTest.cpp
                    )

    target_link_libraries( ImageDiffTest UnitTest VisualUnitTest )

    GTEST_ADD_TESTS (
                      ImageDiffTest
                      "ImageDiffTest"
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiffTest.cpp
                    )

  install (
            DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/ImageDiffTest"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_VTEST_TESTS )
