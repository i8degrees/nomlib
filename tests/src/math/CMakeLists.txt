# nomlib-math module tests

add_executable( MathTest "MathTest.cpp" )

set( NOM_MATH_TESTS_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-math )

if( PLATFORM_WINDOWS )
  list( APPEND NOM_MATH_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
endif( PLATFORM_WINDOWS )

target_link_libraries( MathTest ${NOM_MATH_TESTS_DEPS} )

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/MathTest
                  "" # args
                  "MathTest.cpp" )
