# nomlib-visual-unit-test module tests

# exe VisualUnitTestFrameworkTest
# exe VisualUnitTestFrameworkNonDefaultCtorTest
# exe ImageTestSetTest
# exe ImageDiffTest

add_executable(VisualUnitTestFrameworkTest
  #WIN32 # Link SDL2main over WinMain for console apps
  VisualUnitTestFrameworkTest.cpp
)

add_dependencies(VisualUnitTestFrameworkTest gtest)

target_include_directories(VisualUnitTestFrameworkTest
  PUBLIC
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
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

nom_add_visual_test(  VisualUnitTestFrameworkTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkTest
                      "" )

add_executable(VisualUnitTestFrameworkNonDefaultCtorTest
  #WIN32 # Link SDL2main over WinMain for console apps
  VisualUnitTestFrameworkNonDefaultCtorTest.cpp
)

add_dependencies(VisualUnitTestFrameworkNonDefaultCtorTest gtest)

target_include_directories(VisualUnitTestFrameworkNonDefaultCtorTest
  PUBLIC
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
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestGenRefImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
                      "-r" )

nom_add_visual_test(  VisualUnitTestFrameworkNonDefaultCtorTestCompareImages
                      ${TESTS_INSTALL_DIR}/VisualUnitTestFrameworkNonDefaultCtorTest
                      "" )

add_executable(ImageTestSetTest
  #WIN32 # Link SDL2main over WinMain for console apps
  ImageTestSetTest.cpp
)

add_dependencies(ImageTestSetTest gtest)

target_include_directories(ImageTestSetTest
  PUBLIC
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
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageTestSetTest
                  "" # args
                  "ImageTestSetTest.cpp" )

# For development:
add_custom_command(TARGET ImageTestSetTest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${NOM_TESTS_RESOURCES_DIR}"
  $<TARGET_FILE_DIR:ImageTestSetTest>
)

add_executable(ImageDiffTest
  #WIN32 # Link SDL2main over WinMain for console apps
  ImageDiffTest.cpp
)

add_dependencies(ImageDiffTest gtest)

target_include_directories(ImageDiffTest
  PUBLIC
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
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ImageDiffTest
                  "" # args
                  "ImageDiffTest.cpp" )
# For development:
add_custom_command(TARGET ImageDiffTest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${NOM_TESTS_RESOURCES_DIR}"
  $<TARGET_FILE_DIR:ImageDiffTest>
)

