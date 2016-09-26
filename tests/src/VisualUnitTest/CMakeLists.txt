# nomlib-visual-unit-test module tests

add_executable( VisualUnitTestFrameworkTest "VisualUnitTestFrameworkTest.cpp" )

target_link_libraries(VisualUnitTestFrameworkTest nomlib-visual-unit-test
                      nomlib-graphics)

nom_add_visual_test(  VisualUnitTestFrameworkTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "" )

add_executable( VisualUnitTestFrameworkNonDefaultCtorTest
                "VisualUnitTestFrameworkNonDefaultCtorTest.cpp" )

target_link_libraries(  VisualUnitTestFrameworkNonDefaultCtorTest
                        nomlib-visual-unit-test )

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
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
