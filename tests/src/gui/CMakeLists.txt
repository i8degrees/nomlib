if( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

  add_executable  (
                    UIItemContainerTest
                    ${NOM_TESTS_COMMON_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/UIItemContainerTest.cpp
                  )

  target_link_libraries ( UIItemContainerTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIItemContainerTest
                      "UIItemContainerTest"
                      ${NOM_TESTS_COMMON_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/gui/UIItemContainerTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

if( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS )

  add_executable  (
                    UIEventDispatcherTest
                    ${NOM_TESTS_COMMON_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/UIEventDispatcherTest.cpp
                  )

  target_link_libraries (
                          UIEventDispatcherTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIEventDispatcherTest
                      "UIEventDispatcherTest"
                      ${NOM_TESTS_COMMON_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/gui/UIEventDispatcherTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS )

if( NOM_BUILD_GUI_UI_WIDGET_TESTS )

  add_executable  (
                    UIWidgetTest
                    ${NOM_TESTS_VTEST_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/UIWidgetTest.cpp
                  )

  target_link_libraries (
                          UIWidgetTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIWidgetTest
                      # Disable visual image comparison feature
                      "-d"
                      ${NOM_TESTS_VTEST_SOURCE}
                      ${NOM_TESTS_SOURCE_DIR}/gui/UIWidgetTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_WIDGET_TESTS )

if( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS )

  add_executable  (
                    listbox_layout_test
                    ${NOM_TESTS_VTEST_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/ListBoxLayoutTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/common.cpp
                  )

  target_link_libraries (
                          listbox_layout_test
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  add_test  (
              # test name
              listbox_layout_test_gen_ref_images
              # executable name
              listbox_layout_test
              # args
              "-r"
            )

  add_test  (
              # test name
              listbox_layout_test_compare_images
              # executable name
              listbox_layout_test
              # args
              ""
            )

endif( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS )

if( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS )

  add_executable  (
                    button_layout_test
                    ${NOM_TESTS_VTEST_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/ButtonLayoutTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/common.cpp
                  )

  target_link_libraries (
                          button_layout_test
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  add_test  (
              # test name
              button_layout_test_gen_ref_images
              # executable name
              button_layout_test
              # args
              "-r"
            )

  add_test  (
              # test name
              button_layout_test_compare_images
              # executable name
              button_layout_test
              # args
              ""
            )

endif( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS )

if( NOM_BUILD_GUI_DATAVIEWLIST_TESTS )

  add_executable  (
                    DataViewListTest
                    ${NOM_TESTS_VTEST_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/common.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/DataViewListTest.cpp
                  )

  target_link_libraries (
                          DataViewListTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  add_test  (
              # test name
              dataviewlist_test_gen_ref_images
              # executable name
              DataViewListTest
              # args
              "-r"
            )

  add_test  (
              # test name
              dataviewlist_test_compare_images
              # executable name
              DataViewListTest
              # args
              ""
            )

endif( NOM_BUILD_GUI_DATAVIEWLIST_TESTS )

# TODO: Move to tests/librocket
if( NOM_BUILD_GUI_LIBROCKET_TESTS )

  add_executable  (
                    libRocketTest
                    ${NOM_TESTS_COMMON_SOURCE}
                    ${NOM_TESTS_VTEST_SOURCE}
                    ${NOM_TESTS_SOURCE_DIR}/gui/libRocketTest.cpp
                  )

  target_link_libraries (
                          libRocketTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      libRocketTest
                      ""
                      ${NOM_TESTS_SOURCE_DIR}/gui/libRocketTest.cpp
                    )

    # Search paths to use in finding resources path
    install (
              FILES
              "${NOM_TESTS_RESOURCES_DIR}/gui/librocket/libRocketTest.json"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )

endif( NOM_BUILD_GUI_LIBROCKET_TESTS )

# Resource dependencies
if( NOM_BUILD_GUI_UNIT )

  install (
            DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/gui"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_GUI_UNIT )

# Install binary dependencies for the unit tests
if ( PLATFORM_WINDOWS )

  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            "${MSVCPP_LIBRARY_REDIST}"
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            "${GLEW_REDIST_DIR}"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )

  # TODO: Move to tests/librocket?
  if( NOM_BUILD_LIBROCKET_UNIT )

    install_dependencies  (
                            "${LIBROCKET_REDIST_DIRS}"
                            "${TESTS_DEPS_INSTALL_PEFIX}"
                            "*.dll"
                          )

  endif( NOM_BUILD_LIBROCKET_UNIT )

endif ( PLATFORM_WINDOWS )

