# vendor/CMakeLists.txt:jeff
##
# IMPORTANT: Private dependencies that we link with nomlib; these we do not
# need include header files nor library files when redist our library
##

# NOTE: cmake version 3.7 allows for multiple URL definitions for sake of
# multiple tries before failing, including local filesystem paths.
# NOTE: CMake 3.24+ allows us to pass FIND_PACKAGE_ARGS directly inside
# `FetchContent_Declare` -- this forces CMake to look for a local
# system installation first using find_package. If it is not found, CMake
# will automatically download it.
cmake_minimum_required(VERSION 3.24)
# NOTE: cmake version 4.2+ introduces CONFIGURE_ENV && BUILD_ENV modification
# settings
#cmake_minimum_required(VERSION 4.2)

# DOWNLOAD_EXTRACT_TIMESTAMP behavior - CMake Policy 0135 (FetchContent)
#     cmake --help-policy CMP0135
#
# Setting this policy to the new behavior implies that the files extracted
# from a URL should match the time of extraction, else risk code not being
# regenerated if the URL changes.
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif(POLICY CMP0135)

set(PATCHES_DIR "${PROJECT_SOURCE_DIR}/vendor/patches")

include(FetchContent)

# Build flags

# >> GoogleTest v1.11.0 is our vendor package target due to compiler issues
# >> having been introduced since our inception (v1.8.x).
# (GoogleTest 1.8.x was the original vendor package used)
set(GTEST_TAG "release-1.11.0")
set(GTEST_URL "https://github.com/google/googletest/archive/refs/tags/${GTEST_TAG}.zip")
set(GTEST_MD5 "52943a59cefce0ae0491d4d2412c120b")
set(GTEST_SHA512 "20c2fad22a2ced6828955cc4fa0f540a61bbdcae74c0097529994a183c218589cbe6d2f9e57fc4d625ac01394849cba02e5c3fb6ba618775299f01d6b0737d26")
FetchContent_Declare(
  googletest
  URL               "${GTEST_URL}"
  URL_HASH          "SHA512=${GTEST_SHA512}"
  URL_MD5           "${GTEST_MD5}"
  #SOURCE_SUBDIR     "googletest"
  # FIND_PACKAGE_ARGS NAMES GTest
)
# >> jsoncpp v0.7.0 by ...
# >> we use the amalgamated source tree (one cpp and header) without the install phase
set(JSONCPP_TAG     "0.7.0")
set(JSONCPP_URL     "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/${JSONCPP_TAG}.zip")
set(JSONCPP_MD5     "48e1c0383d11bf894a1def195e3662ca")
set(JSONCPP_SHA512  "e99d00ac65620163679bbf5f14fff66657cc540f456e650dfb6780016a42869636f9b09dfcb819277901fff7844db6f75a024e3232b32229d487438981a0b0c8")
FetchContent_Declare(
  jsoncpp
  URL             "${JSONCPP_URL}"
  URL_MD5         "${JSONCPP_MD5}"
  URL_HASH        "SHA512=${JSONCPP_SHA512}"
  PATCH_COMMAND   python3 <SOURCE_DIR>/amalgamate.py
  COMMAND         patch <SOURCE_DIR>/src/lib_json/CMakeLists.txt "${PATCHES_DIR}/jsoncpp_no-install.patch"
)

# >> tclap v1.2.1 by ...
# >> headers only library
set(TCLAP_TAG "tclap-1-2-1-release-final")
set(TCLAP_URL "https://github.com/mirror/tclap/archive/refs/tags/${TCLAP_TAG}.zip")
set(TCLAP_MD5 "90c1cc9a6dceef8fe89cf83f8b9b8460")
set(TCLAP_SHA512 "9e771f4e4a0cd9426bc8754d77a26a2c84de0c1ee09b58bb3e9066fba81f19bfa113e5060d31f05999dfc0c47146d501658c4a35531f9a135152dc5b4275224b")
FetchContent_Declare(
  tclap
  URL             "${TCLAP_URL}"
  URL_MD5         "${TCLAP_MD5}"
  URL_HASH        "SHA512=${TCLAP_SHA512}"
)

# >> RapidXML v1.13 by discord
# >> headers-only library
set(RAPIDXML_TAG "master")
set(RAPIDXML_URL "https://github.com/discord/rapidxml/archive/refs/heads/${RAPIDXML_TAG}.zip")
set(RAPIDXML_MD5 "8918b1903277a552669aa59e66cb5bc9")
set(RAPIDXML_SHA512 "7a80a85349176c8b7b1b393b7a1512950067c7e7fd9495ade451878e0ee0adfa2c30843a371f2a4ce9edaa9da6f17ca310cdb98462fb9ab93fd2ef413fdd9966")
FetchContent_Declare(
  rapidxml
  URL             "${RAPIDXML_URL}"
  URL_MD5         "${RAPIDXML_MD5}"
  URL_HASH        "SHA512=${RAPIDXML_SHA512}"
  PATCH_COMMAND   patch -i "${PATCHES_DIR}/rapidxml_bug16.patch"
)

#
## build settings
#

# jsoncpp settings
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE INTERNAL "prevent cache pollution")
set(JSONCPP_LIB_BUILD_SHARED OFF CACHE INTERNAL "prevent cache pollution")
set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "prevent cache pollution")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "prevent cache pollution")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE INTERNAL "prevent cache pollution")
set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE INTERNAL "prevent cache pollution")

FetchContent_MakeAvailable(jsoncpp tclap rapidxml)

#
## Internal env
#
# These filesystem paths are resulting from the configuration executed above

set(JSONCPP_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/jsoncpp-src/dist" CACHE PATH "jsoncpp source tree")
set(JSONCPP_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/jsoncpp-src/dist" CACHE PATH "jsoncpp headers tree")
set(TCLAP_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/tclap-src/include" CACHE PATH "tclap headers tree")
set(RAPIDXML_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/rapidxml-src" CACHE PATH "rapidxml headers tree")

if(BUILD_TESTS)
  add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/googletest")
endif()
