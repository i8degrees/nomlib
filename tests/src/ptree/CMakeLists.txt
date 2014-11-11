if( NOM_BUILD_PTREE_TESTS )

  add_executable  (
                    ptree
                    ${NOM_TESTS_SOURCE_DIR}/ptree/ptree.cpp
                  )

  target_link_libraries( ptree UnitTest )

  GTEST_ADD_TESTS (
                    ptree
                    "values"
                    ${NOM_TESTS_SOURCE_DIR}/ptree/ptree.cpp
                  )

endif( NOM_BUILD_PTREE_TESTS )
