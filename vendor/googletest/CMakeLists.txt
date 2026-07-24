##
# GoogleTest settings
##

## 1. Force static libraries for GoogleTest
set(gtest_force_shared_crt OFF CACHE INTERNAL "prevent cache pollution")
if(WIN32)
  set(gtest_force_shared_crt OFF CACHE INTERNAL "prevent cache pollution")
endif()
## ! 2.
# FIXME(JEFF): This should be OFF so that we can statically link GTest in
#set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "prevent cache pollution")
set(BUILD_SHARED_LIBS OFF)
## Only build GTest and do not install artifacts (header files, library files)
set(BUILD_GMOCK OFF CACHE INTERNAL "prevent cache pollution")
set(BUILD_GTEST ON CACHE INTERNAL "prevent cache pollution")
set(INSTALL_GTEST OFF CACHE INTERNAL "prevent cache pollution")
#set(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "prevent cache pollution")

if(NOT BUILD_SHARED_LIBS)
  if(NOT CMAKE_CXX_FLAGS MATCHES "/-fPIC/")
    string(APPEND CMAKE_CXX_FLAGS "-fPIC")
  endif()
endif()

FetchContent_MakeAvailable(googletest)

set(LIB_SUFFIX "")
set(GTEST_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include" CACHE PATH "GoogleTest headers dir")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(LIB_SUFFIX "d")
  message(STATUS "[googletest] LIB_SUFFIX is ${LIB_SUFFIX}")
endif()

if(BUILD_SHARED_LIBS)
  if(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    # Xcode projects output binaries to lib/Debug
    set(GTEST_LIBRARY "${CMAKE_BINARY_DIR}/lib/Debug/libgtest${LIB_SUFFIX}.so" CACHE FILEPATH "GTest::gtest shared library")
    set(GTEST_MAIN_LIBRARY "${CMAKE_BINARY_DIR}/lib/Debug/libgtest_main${LIB_SUFFIX}.so" CACHE FILEPATH "GTest::gtest_main shared library")
  else() # Other generators, i.e.: Makefiles output binaries to lib
    set(GTEST_LIBRARY "${CMAKE_BINARY_DIR}/lib/libgtest${LIB_SUFFIX}.so" CACHE FILEPATH "GTest::gtest shared library")
    set(GTEST_MAIN_LIBRARY "${CMAKE_BINARY_DIR}/lib/libgtest_main${LIB_SUFFIX}.so" CACHE FILEPATH "GTest::gtest_main shared library")
  endif()
else()
  if(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    # Xcode projects output binaries to lib/Debug
    set(GTEST_LIBRARY "${CMAKE_BINARY_DIR}/lib/Debug/libgtest${LIB_SUFFIX}.a" CACHE FILEPATH "GTest::gtest static library")
    set(GTEST_MAIN_LIBRARY "${CMAKE_BINARY_DIR}/lib/Debug/libgtest_main${LIB_SUFFIX}.a" CACHE FILEPATH "GTest::gtest_main static library")
  else() # Other generators, i.e.: Makefiles output binaries to lib
    set(GTEST_LIBRARY "${CMAKE_BINARY_DIR}/lib/libgtest${LIB_SUFFIX}.a" CACHE FILEPATH "GTest::gtest static library")
    set(GTEST_MAIN_LIBRARY "${CMAKE_BINARY_DIR}/lib/libgtest_main${LIB_SUFFIX}.a" CACHE FILEPATH "GTest::gtest_main static library")
  endif()
endif()

set(GTEST_LIBRARIES "${GTEST_LIBRARY};${GTEST_MAIN_LIBRARY}" CACHE FILEPATH "GoogleTest libraries for linking")
