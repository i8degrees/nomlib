set( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS on )
set( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS on )

# TODO: Remove
set( NOM_BUILD_GUI_VBOX_LAYOUT_TESTS on )

# TODO: Remove?
set( NOM_BUILD_GUI_CUSTOM_UI_LAYOUT_TESTS off )

if( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

  add_executable  (
                    UIItemContainerTest
                    ${NOM_TESTS_DIR}/gui/UIItemContainerTest.cpp
                  )

  target_link_libraries ( UIItemContainerTest
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIItemContainerTest
                      "UIItemContainerTest"
                      ${NOM_TESTS_DIR}/gui/UIItemContainerTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

if( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS )

  add_executable  (
                    UIEventDispatcherTest
                    ${NOM_TESTS_DIR}/gui/UIEventDispatcherTest.cpp
                  )

  target_link_libraries (
                          UIEventDispatcherTest
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIEventDispatcherTest
                      "UIEventDispatcherTest"
                      ${NOM_TESTS_DIR}/gui/UIEventDispatcherTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS )

if( NOM_BUILD_GUI_VBOX_LAYOUT_TESTS )

  add_executable  (
                    VBoxLayoutTest
                    ${NOM_TESTS_DIR}/gui/VBoxLayoutTest.cpp
                  )

  target_link_libraries (
                          VBoxLayoutTest
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    VBoxLayoutTest
                    "VBoxLayoutTest"
                    ${NOM_TESTS_DIR}/gui/VBoxLayoutTest.cpp
                  )

  # TODO: Remove (we should be using paths resolved from the SystemFonts and/or
  # PlatformSettings APIs.
  install (
            DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/gui"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_GUI_VBOX_LAYOUT_TESTS )

if( NOM_BUILD_GUI_CUSTOM_UI_LAYOUT_TESTS )

  add_executable  (
                    CustomUILayoutTest
                    ${NOM_TESTS_DIR}/gui/CustomUILayout.cpp
                    ${NOM_TESTS_DIR}/gui/CustomUILayoutTest.cpp
                  )

  target_link_libraries (
                          CustomUILayoutTest
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    CustomUILayoutTest
                    "CustomUILayoutTest"
                    ${NOM_TESTS_DIR}/gui/CustomUILayout.cpp
                    ${NOM_TESTS_DIR}/gui/CustomUILayoutTest.cpp
                  )

endif( NOM_BUILD_GUI_CUSTOM_UI_LAYOUT_TESTS )
