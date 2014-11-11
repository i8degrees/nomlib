if( NOM_BUILD_PTREE_TESTS )

  add_executable  (
                    ptree
                    ${NOM_TESTS_SOURCE_DIR}/ptree/ptree.cpp
                  )

  target_link_libraries( ptree ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    ptree
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/ptree/ptree.cpp
                  )

endif( NOM_BUILD_PTREE_TESTS )
