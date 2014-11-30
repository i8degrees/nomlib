# nomlib-visual-unit-test module tests

add_executable( VisualUnitTestFrameworkTest "VisualUnitTestFrameworkTest.cpp" )

target_link_libraries(  VisualUnitTestFrameworkTest
                        nomlib-visual-unit-test )

add_test(   # test name
            VisualUnitTestFrameworkTestGenRefImages,
            # executable name
            ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
            # args
            "-r" )

add_test( # test name
          VisualUnitTestFrameworkTestCompareImages,
          # executable name
          ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
          # args
          "" )

add_executable( VisualUnitTestFrameworkNonDefaultCtorTest
                "VisualUnitTestFrameworkNonDefaultCtorTest.cpp" )

target_link_libraries(  VisualUnitTestFrameworkNonDefaultCtorTest
                        nomlib-visual-unit-test )

add_test( # test name
          VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages,
          # executable name
          ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
          # args
          "-r" )

add_test( # test name
          VisualUnitTestFrameworkNonDefaultCtorTestCompareImages,
          # executable name
          ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
          # args
          "" )

# FIXME
# add_executable( ImageTestSetTest "ImageTestSetTest.cpp" )

# target_link_libraries( ImageTestSetTest nomlib-visual-unit-test )

# GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageTestSetTest
#                   "" # args
#                   "ImageTestSetTest.cpp" )

# FIXME
# add_executable( ImageDiffTest "ImageDiffTest.cpp" )

# target_link_libraries( ImageDiffTest nomlib-visual-unit-test )

# GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageDiffTest
#                   "" # args
#                   "ImageDiffTest.cpp" )

# install(  DIRECTORY
#           "${NOM_TESTS_RESOURCES_DIR}/ImageDiffTest"
#           DESTINATION
#           "${TESTS_INSTALL_DIR}/Resources"
#           PATTERN ".*" EXCLUDE
#         )
