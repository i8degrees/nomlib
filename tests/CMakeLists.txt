# Unit testing with Google Test
#
# NOTE: Ensure that when you use the GTEST_ADD_TESTS macro, that you supply the
# necessary arguments -- name of test(s) to run -- otherwise you may notice
# CTest skipping the tests all together!
#
# NOTE: You may need to issue a 'make rebuild_cache' after changing test
# fixture names, in order to get 'make test' to recognize the modified test
# code.
#
# NOTE: To display a unit test's full debugging output (NOM_LOG_ERR, NOM_DUMP
# and friends), run the test's executable directly. This debugging output will
# not be shown when using the 'test' target -- i.e.: make target.
#
# FIXME: Under WindowsOS, I get strange errors anytime a unit test fails only
# when running 'msbuild /t:build build\RUN_TESTS.vcxproj' from the command line:
#
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: The command "setlocal\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: "C:\Program Files (x86)\CMake 2.8\bin\ctest.exe" --force-new-ctest-process -C Debug\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: if %errorlevel% neq 0 goto :cmEnd\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: :cmEnd\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: endlocal & call :cmErrorLevel %errorlevel% & goto :cmDone\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: :cmErrorLevel\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: exit /b %1\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: :cmDone\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: if %errorlevel% neq 0 goto :VCEnd\r [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
# C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(132,5): error MSB3073: :VCEnd" exited with code 8. [C:\Users\jeff\Projects\nomlib.git\build\RUN_TESTS.vcxproj]
#
# It appears to work as expected when RUN_TESTS.vcxproj is built from within
# MS Visual Studio Express.
#
# The current "workaround" is to execute the test's executable directly.
#
# References:
# 1. https://code.google.com/p/urho3d/issues/detail?id=30

set( NOM_BUILD_PTREE_TESTS off )
set( NOM_BUILD_PTREE_JSON_TESTS off )
set( NOM_BUILD_PTREE_XML_TESTS off )

set( NOM_BUILD_RENDERWINDOW_TESTS off )
set( NOM_BUILD_MATH_TESTS off )
set( NOM_BUILD_FILE_TESTS off )

set( NOM_BUILD_FONT_CACHE_TESTS on )
set( NOM_BUILD_COLOR_DB_TESTS on )

set( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS on )
set( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS on )

# TODO: Remove
set( NOM_BUILD_GUI_VBOX_LAYOUT_TESTS off )

set( NOM_BUILD_GUI_CUSTOM_UI_LAYOUT_TESTS off )

set( NOM_BUILD_JSON_CONFIG_FILE_TESTS off )

if( PLATFORM_WINDOWS )

  # Path under Windows should resolve to: build/tests/Debug
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
      )

else( NOT PLATFORM_WINDOWS )

  # Path should resolve to a directory: build/tests
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}"
      )

endif( PLATFORM_WINDOWS )

# NOTE: we set GTEST_ROOT in the dependencies section of our build script.
find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )

if( NOM_BUILD_PTREE_TESTS )

  add_executable  (
                    ptree
                    ${NOM_TESTS_DIR}/ptree/ptree.cpp
                  )

  target_link_libraries ( ptree
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      ptree
                      "values"
                      ${NOM_TESTS_DIR}/ptree/ptree.cpp
                    )

endif( NOM_BUILD_PTREE_TESTS )

# Additional Header file dependencies
include_directories( "${NOM_TESTS_DIR}/serializers" )

if( NOM_BUILD_PTREE_JSON_TESTS )

  add_executable  (
                    json_serializer
                    ${NOM_TESTS_DIR}/serializers/JsonCppSerializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries ( json_serializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    json_serializer
                    "JsonCppSerializerTest"
                    ${NOM_TESTS_DIR}/serializers/JsonCppSerializerTest.cpp
                  )

  add_executable  (
                    json_deserializer
                    ${NOM_TESTS_DIR}/serializers/JsonCppDeserializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )


  target_link_libraries (
                          json_deserializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      json_deserializer
                      "JsonCppDeserializerTest"
                      ${NOM_TESTS_DIR}/serializers/JsonCppDeserializerTest.cpp
                    )

endif( NOM_BUILD_PTREE_JSON_TESTS )

if( NOM_BUILD_PTREE_XML_TESTS )

  add_executable  (
                    xml_serializer
                    ${NOM_TESTS_DIR}/serializers/RapidXmlSerializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries ( xml_serializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    xml_serializer
                    "RapidXmlSerializer"
                    ${NOM_TESTS_DIR}/serializers/RapidXmlSerializerTest.cpp
                  )

  add_executable  (
                    xml_deserializer
                    ${NOM_TESTS_DIR}/serializers/RapidXmlDeserializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries ( xml_deserializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    xml_deserializer
                    "RapidXmlDeserializer"
                    ${NOM_TESTS_DIR}/serializers/RapidXmlDeserializerTest.cpp
                  )

endif( NOM_BUILD_PTREE_XML_TESTS )

# Copy JSON resources for unit tests only if necessary (built).
if( NOM_BUILD_PTREE_JSON_TESTS )
  install ( DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/json"
            DESTINATION
            # "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif( NOM_BUILD_PTREE_JSON_TESTS )

# Copy XML resources for unit tests only if necessary (built).
if( NOM_BUILD_PTREE_XML_TESTS )
  install ( DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/xml"
            DESTINATION
            # "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif( NOM_BUILD_PTREE_XML_TESTS )

if( NOM_BUILD_FONT_CACHE_TESTS )
  if( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      font_cache
                      ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                      )

    target_link_libraries (
                            font_cache
                            ${GTEST_BOTH_LIBRARIES}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      font_cache
                      "FontCacheTest"
                      ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                      )

    # FIXME:
    if( PLATFORM_WINDOWS )
      install (
                DIRECTORY
                "${NOMLIB_FONTS}" # Resources/fonts
                DESTINATION
                "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
                PATTERN ".*" EXCLUDE # Just say no to dotfiles
              )
    endif( PLATFORM_WINDOWS )

  endif( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
endif( NOM_BUILD_FONT_CACHE_TESTS )

if( NOM_BUILD_COLOR_DB_TESTS )
  if( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable  (
                      color_database
                      ${NOM_TESTS_DIR}/system/ColorDatabaseTest.cpp
                    )

    target_link_libraries (
                            color_database
                            ${GTEST_BOTH_LIBRARIES}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      color_database
                      "ColorDatabaseTest"
                      ${NOM_TESTS_DIR}/system/ColorDatabaseTest.cpp
                    )
  endif( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
endif( NOM_BUILD_COLOR_DB_TESTS )

if( NOM_BUILD_FILE_TESTS )
  if( NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      file_interface
                      ${NOM_TESTS_DIR}/system/FileInterfaceTest.cpp
                    )

    target_link_libraries (
                            file_interface
                            ${GTEST_BOTH_LIBRARIES}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      file_interface
                      "FileInterfaceTest"
                      ${NOM_TESTS_DIR}/system/FileInterfaceTest.cpp
                    )
  endif( NOM_BUILD_SYSTEM_UNIT )
endif( NOM_BUILD_FILE_TESTS )

if( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

  # JsonConfigFile

  add_executable  (
                    json_config_file
                    ${NOM_TESTS_DIR}/serializers/JsonConfigFileTest.cpp
                    # ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries ( json_config_file
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

    GTEST_ADD_TESTS (
                      json_config_file
                      "JsonConfigFileTest"
                      ${NOM_TESTS_DIR}/serializers/JsonConfigFileTest.cpp
                    )

endif( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

if( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS )

  # UIItemContainerTests

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

  # UIEventDispatcherTests

  add_executable  (
                    UIEventDispatcherTest
                    ${NOM_TESTS_DIR}/gui/UIEventDispatcherTest.cpp
                  )

  target_link_libraries ( UIEventDispatcherTest
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

  # VBoxLayoutTest

  add_executable  (
                    VBoxLayoutTest
                    ${NOM_TESTS_DIR}/gui/VBoxLayoutTest.cpp
                  )

  target_link_libraries ( VBoxLayoutTest
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    VBoxLayoutTest
                    "VBoxLayoutTest"
                    ${NOM_TESTS_DIR}/gui/VBoxLayoutTest.cpp
                  )

  # install ( DIRECTORY
  #           "${NOM_TESTS_RESOURCES_DIR}/gui"
  #           DESTINATION
  #           "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
  #           PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
  #         )

endif( NOM_BUILD_GUI_VBOX_LAYOUT_TESTS )

if( NOM_BUILD_GUI_CUSTOM_UI_LAYOUT_TESTS )

  # CustomUILayoutTest

  add_executable  (
                    CustomUILayoutTest
                    ${NOM_TESTS_DIR}/gui/CustomUILayout.cpp
                    ${NOM_TESTS_DIR}/gui/CustomUILayoutTest.cpp
                  )

  target_link_libraries ( CustomUILayoutTest
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

if( NOM_BUILD_TEMPLATES_UNIT )

  # Resources/SharedSupport/UnitTestTemplate.cpp

  add_executable  (
                    Template
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

  target_link_libraries ( Template
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    Template
                    "Template"
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

endif( NOM_BUILD_TEMPLATES_UNIT )

# JsonConfigFileTest

if( NOM_BUILD_JSON_CONFIG_FILE_TESTS )
  install ( DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/json"
            DESTINATION
            # "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

if( NOM_BUILD_RENDERWINDOW_TESTS )

  if( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable  (
                      render_window
                      ${NOM_TESTS_DIR}/graphics/RenderWindowTest.cpp
                    )

    target_link_libraries (
                            render_window
                            ${GTEST_BOTH_LIBRARIES}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      render_window
                      "RenderWindowTest"
                      ${NOM_TESTS_DIR}/graphics/RenderWindowTest.cpp
                    )

    # Copy nomlib's window icon resource.
    install ( FILES
              "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/icon.png"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )

  endif( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_RENDERWINDOW_TESTS )

if( NOM_BUILD_MATH_TESTS )

  if( NOM_BUILD_MATH_UNIT )
    add_executable  (
                      math
                      ${NOM_TESTS_DIR}/math/MathTest.cpp
                    )

    target_link_libraries (
                            math
                            ${GTEST_BOTH_LIBRARIES}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      math
                      "MathTest"
                      ${NOM_TESTS_DIR}/math/MathTest.cpp
                    )
  endif( NOM_BUILD_MATH_UNIT )

endif( NOM_BUILD_MATH_TESTS )

# Install library dependencies into tests output directory so we can always
# execute the binaries with the proper dependency versions.
if ( PLATFORM_WINDOWS )
  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            "${MSVCPP_LIBRARY_REDIST}"
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )
endif ( PLATFORM_WINDOWS )
