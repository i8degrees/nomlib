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
set(PROJECT_DEPS "SDL2 >=2.0.4 librocket >=1.3.0 libsndfile >=1.0.24 SDL2_image >=2.0.0 SDL2_ttf >= 2.0.12 openal >= 1.14")
set(PROJECT_DEPS_PRIVATE "jsoncpp >= 1.x rapidxml >=1.x gtest >=1.8.x")
set(PROJECT_LIBS "")

if(NOM_BUILD_ACTIONS_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-actions)
endif()

if(NOM_BUILD_AUDIO_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-audio)
endif()

if(NOM_BUILD_CORE_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-core)
endif()

if(NOM_BUILD_FILE_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-file)
endif()

if(NOM_BUILD_GRAPHICS_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-graphics)
endif()

if(NOM_BUILD_GUI_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-gui)
endif()

if(NOM_BUILD_MATH_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-math)
endif()

if(NOM_BUILD_PTREE_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-ptree)
endif()

if(NOM_BUILD_SERIALIZERS_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-serializers)
endif()

if(NOM_BUILD_SYSTEM_UNIT)
  list( APPEND PROJECT_LIBS -l@PROJECT_NAME@-system)
endif()

configure_file  ( "${CMAKE_TEMPLATE_PATH}/nomlib.pc.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
                  #"${CMAKE_SOURCE_DIR}/Resources/pkgconfig/${PROJECT_NAME}.pc"
                  @ONLY
                )
