set( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS off )
set( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS off )

set( NOM_BUILD_GUI_UI_WIDGET_TESTS on )

set( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS on )
set( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS on )

if( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

  add_executable  (
                    UIItemContainerTest
                    ${NOM_TESTS_DIR}/gui/UIItemContainerTest.cpp
                  )

  target_link_libraries ( UIItemContainerTest
                          ${GTEST_LIBRARY}
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
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIEventDispatcherTest
                      "UIEventDispatcherTest"
                      ${NOM_TESTS_DIR}/gui/UIEventDispatcherTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS )

if( NOM_BUILD_GUI_UI_WIDGET_TESTS )

  add_executable  (
                    UIWidgetTest
                    ${NOM_TESTS_DIR}/gui/UIWidgetTest.cpp
                  )

  target_link_libraries (
                          UIWidgetTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      UIWidgetTest
                      "UIWidgetTest"
                      ${NOM_TESTS_DIR}/gui/UIWidgetTest.cpp
                    )

endif( NOM_BUILD_GUI_UI_WIDGET_TESTS )

if( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS )

  add_executable  (
                    listbox_layout_test
                    ${NOM_TESTS_DIR}/gui/ListBoxLayoutTest.cpp
                    ${NOM_TESTS_DIR}/gui/common.cpp
                  )

  target_link_libraries (
                          listbox_layout_test
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    listbox_layout_test
                    "ListBoxLayoutTest"
                    ${NOM_TESTS_DIR}/gui/ListBoxLayoutTest.cpp
                    ${NOM_TESTS_DIR}/gui/common.cpp
                  )

endif( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS )

if( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS )

  add_executable  (
                    button_layout_test
                    ${NOM_TESTS_DIR}/gui/ButtonLayoutTest.cpp
                    ${NOM_TESTS_DIR}/gui/common.cpp
                  )

  target_link_libraries (
                          button_layout_test
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    button_layout_test
                    "ButtonLayoutTest"
                    ${NOM_TESTS_DIR}/gui/ButtonLayoutTest.cpp
                    ${NOM_TESTS_DIR}/gui/common.cpp
                  )

  install (
            DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/gui"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS )
