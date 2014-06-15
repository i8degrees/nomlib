set( NOM_BUILD_PTREE_TESTS off )

if( NOM_BUILD_PTREE_TESTS )

  add_executable  (
                    ptree
                    ${NOM_TESTS_DIR}/ptree/ptree.cpp
                  )

  target_link_libraries (
                          ptree
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    ptree
                    "values"
                    ${NOM_TESTS_DIR}/ptree/ptree.cpp
                  )

endif( NOM_BUILD_PTREE_TESTS )
