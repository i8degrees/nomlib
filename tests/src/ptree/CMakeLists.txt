# nomlib-ptree module tests

set( NOM_PTREE_TESTS_DEPS ${GTEST_LIBRARY} nomlib-ptree )

if( PLATFORM_WINDOWS )
  list( APPEND NOM_PTREE_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
endif( PLATFORM_WINDOWS )

add_executable( ptree "ptree.cpp" )

target_link_libraries( ptree ${NOM_PTREE_TESTS_DEPS} )

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ptree
                  "" # args
                  "ptree.cpp" )
