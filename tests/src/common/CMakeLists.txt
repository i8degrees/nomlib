if( NOM_BUILD_COMMON_TESTS )

    add_executable  (
                      UnitTestFrameworkTest
                      # ${NOM_TESTS_COMMON_SOURCE}  # nom::UnitTest
                      ${NOM_TESTS_VTEST_SOURCE}   # nom::VisualUnitTest
                      ${NOM_TESTS_SOURCE_DIR}/common/UnitTestFrameworkTest.cpp
                    )

    target_link_libraries (
                            UnitTestFrameworkTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      UnitTestFrameworkTest
                      "UnitTestFrameworkTest"
                      # ${NOM_TESTS_COMMON_SOURCE}  # nom::UnitTest
                      ${NOM_TESTS_VTEST_SOURCE}   # nom::VisualUnitTest
                      ${NOM_TESTS_SOURCE_DIR}/common/UnitTestFrameworkTest.cpp
                    )

    add_executable  (
                      VisualUnitTestFrameworkTest
                      ${NOM_TESTS_VTEST_SOURCE}   # nom::VisualUnitTest
                      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestFrameworkTest.cpp
                    )

    target_link_libraries (
                            VisualUnitTestFrameworkTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

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
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestFrameworkNonDefaultCtorTest.cpp
                    )

    target_link_libraries (
                            VisualUnitTestFrameworkNonDefaultCtorTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

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
                      VisualSetTest
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/common/VisualTestSetTest.cpp
                    )

    target_link_libraries (
                            VisualSetTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      VisualSetTest
                      "VisualSetTest"
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/common/VisualTestSetTest.cpp
                    )

    add_executable  (
                      ImageDiffTest
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiffTest.cpp
                    )

    target_link_libraries (
                            ImageDiffTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      ImageDiffTest
                      "ImageDiffTest"
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiffTest.cpp
                    )

  install (
            DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/ImageDiffTest"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_COMMON_TESTS )
