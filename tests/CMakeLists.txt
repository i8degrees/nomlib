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

include( "${NOM_TESTS_DIR}/graphics/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/math/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/system/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/gui/CMakeLists.txt" )

include( "${NOM_TESTS_DIR}/serializers/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/ptree/CMakeLists.txt" )

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

if( NOM_BUILD_TEMPLATES_UNIT )

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
