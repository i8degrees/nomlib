# nomlib-ptree module tests

#if( PLATFORM_WINDOWS )
#list( APPEND NOM_PTREE_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
#endif( PLATFORM_WINDOWS )

add_executable(ptree
  WIN32
  ptree.cpp
)

add_dependencies(ptree gtest)

target_include_directories(ptree
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${PROJECT_SOURCE_DIR}/tests/include
  PRIVATE
  ${GTEST_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(ptree
  PUBLIC
  nomlib-core
  nomlib-ptree
  PRIVATE
  Threads::Threads
  ${GTEST_LIBRARY}
)

GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ptree
                  "" # args
                  "ptree.cpp" )
