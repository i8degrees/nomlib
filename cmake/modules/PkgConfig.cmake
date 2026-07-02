# cmake/pkgconfig.cmake:jeff
#
# TODO(JEFF): Verify this is functional before adding to our install targets
# pkg-config compatible script at $PREFIX/lib/pkgconfig
#

#set(CMAKE_INSTALL_DOCS "share/doc/nomlib/v0.13.1")
set(PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_INCLUDEDIR "include/nomlib")
set(PROJECT_DESC "C++11 game engine")
set(PROJECT_URL "https://github.com/i8degrees/nomlib")
set(PROJECT_DEPS "")
# TODO(JEFF): The project dependency list should be modified to only reflect the
# build units that are available, i.e.: without the audio unit, we do not
# need the openal or (likely) sndfile libraries.
set(PROJECT_DEPS "sdl2 >=2.0.4 SDL2_image >=2.0.0 sdl2_ttf >= 2.0.12 openal >= 1.14 libRocket >=1.3.0 sndfile >=1.0.24")
set(PROJECT_DEPS_PRIVATE "")
#set(PROJECT_DEPS_PRIVATE "jsoncpp >= 1.x rapidxml >=1.x gtest >=1.8.x")
set(LIBS "")
# Release builds will have no suffix.
set(LIB_SUFFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(LIB_SUFFIX "-d")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# DEPRECATED(JEFF): This "option" will be removed in a future release.
# Favor the use of `CMAKE_BUILD_TYPE` instead.
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(LIB_SUFFIX "-d")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

if(NOM_BUILD_ACTIONS_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-actions${LIB_SUFFIX}")
endif()

if(NOM_BUILD_AUDIO_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-audio${LIB_SUFFIX}")
endif()

if(NOM_BUILD_CORE_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-core${LIB_SUFFIX}")
endif()

if(NOM_BUILD_FILE_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-file${LIB_SUFFIX}")
endif()

if(NOM_BUILD_GRAPHICS_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-graphics${LIB_SUFFIX}")
endif()

if(NOM_BUILD_GUI_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-gui${LIB_SUFFIX}")
endif()

if(NOM_BUILD_MATH_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-math${LIB_SUFFIX}")
endif()

if(NOM_BUILD_PTREE_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-ptree${LIB_SUFFIX}")
endif()

if(NOM_BUILD_SERIALIZERS_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-serializers${LIB_SUFFIX}")
endif()

if(NOM_BUILD_SYSTEM_UNIT)
  set(LIBS "${LIBS} -l${PROJECT_NAME}-system${LIB_SUFFIX}")
endif()

configure_file(# nomlib.pc
  "${CMAKE_TEMPLATE_PATH}/nomlib.pc.in" # template (input)
  "${CMAKE_BINARY_DIR}/nomlib.pc" # output (output)
  @ONLY # options
)
