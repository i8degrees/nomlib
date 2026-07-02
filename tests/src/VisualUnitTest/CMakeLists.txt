# nomlib-visual-unit-test module tests

# exe VisualUnitTestFrameworkTest
# exe VisualUnitTestFrameworkNonDefaultCtorTest
# exe ImageTestSetTest
# exe ImageDiffTest

add_executable(VisualUnitTestFrameworkTest
  WIN32
  VisualUnitTestFrameworkTest.cpp
)

add_dependencies(VisualUnitTestFrameworkTest gtest)

target_include_directories(VisualUnitTestFrameworkTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(VisualUnitTestFrameworkTest
  PUBLIC
  nomlib-core
  nomlib-graphics
  nomlib-visual-unit-test
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

nom_add_visual_test(  VisualUnitTestFrameworkTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "" )

add_executable(VisualUnitTestFrameworkNonDefaultCtorTest
  WIN32
  VisualUnitTestFrameworkNonDefaultCtorTest.cpp
)

add_dependencies(VisualUnitTestFrameworkNonDefaultCtorTest gtest)

target_include_directories(VisualUnitTestFrameworkNonDefaultCtorTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(VisualUnitTestFrameworkNonDefaultCtorTest
  PUBLIC
  nomlib-core
  nomlib-visual-unit-test
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
                      "" )

add_executable(ImageTestSetTest
  WIN32
  ImageTestSetTest.cpp
)

add_dependencies(ImageTestSetTest gtest)

target_include_directories(ImageTestSetTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(ImageTestSetTest
  PUBLIC
  nomlib-core
  nomlib-visual-unit-test
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageTestSetTest
                  "" # args
                  "ImageTestSetTest.cpp" )

add_executable(ImageDiffTest
  WIN32
  ImageDiffTest.cpp
)

add_dependencies(ImageDiffTest gtest)

target_include_directories(ImageDiffTest
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(ImageDiffTest
  PUBLIC
  nomlib-core
  nomlib-visual-unit-test
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageDiffTest
                  "" # args
                  "ImageDiffTest.cpp" )
