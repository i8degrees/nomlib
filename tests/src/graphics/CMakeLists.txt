if( NOM_BUILD_RENDERWINDOW_TESTS )

  if( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      render_window
                      ${NOM_TESTS_SOURCE_DIR}/graphics/RenderWindowTest.cpp
                    )

    target_link_libraries (
                            render_window
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      render_window
                      "RenderWindowTest"
                      ${NOM_TESTS_SOURCE_DIR}/graphics/RenderWindowTest.cpp
                    )

    # Copy nomlib's window icon resource.
    install (
              FILES
              "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/icon.png"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )

  endif( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_RENDERWINDOW_TESTS )

if( NOM_BUILD_GRADIENT_TESTS )

    add_executable  (
                      GradientTest
                      ${NOM_TESTS_SOURCE_DIR}/graphics/GradientTest.cpp
                      # VisualUnitTest
                      ${NOM_TESTS_COMMON_SOURCE}
                      ${NOM_TESTS_VTEST_SOURCE}
                    )

    target_link_libraries (
                            GradientTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      GradientTest
                      ""
                      ${NOM_TESTS_SOURCE_DIR}/graphics/GradientTest.cpp
                    )
endif( NOM_BUILD_GRADIENT_TESTS )
