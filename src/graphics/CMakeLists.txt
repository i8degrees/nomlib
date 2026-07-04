# nomlib-graphics library

set(GRAPHICS_HDR
  ${HDR_BASE}/graphics.hpp
  ${HDR_BASE}/graphics/Cursor.hpp
  ${HDR_BASE}/graphics/Gradient.hpp
  ${HDR_BASE}/graphics/IDrawable.hpp
  ${HDR_BASE}/graphics/Image.hpp
  ${HDR_BASE}/graphics/RenderWindow.hpp
  ${HDR_BASE}/graphics/Renderer.hpp
  ${HDR_BASE}/graphics/RendererInfo.hpp
  ${HDR_BASE}/graphics/Text.hpp
  ${HDR_BASE}/graphics/Texture.hpp
  ${HDR_BASE}/graphics/DisplayMode.hpp
  ${HDR_BASE}/graphics/fonts/BMFont.hpp
  ${HDR_BASE}/graphics/fonts/BitmapFont.hpp
  ${HDR_BASE}/graphics/fonts/Font.hpp
  ${HDR_BASE}/graphics/fonts/FontMetrics.hpp
  ${HDR_BASE}/graphics/fonts/FontPage.hpp
  ${HDR_BASE}/graphics/fonts/FontRow.hpp
  ${HDR_BASE}/graphics/fonts/Glyph.hpp
  ${HDR_BASE}/graphics/fonts/IFont.hpp
  ${HDR_BASE}/graphics/fonts/TrueTypeFont.hpp
  ${HDR_BASE}/graphics/graphics_helpers.hpp
  ${HDR_BASE}/graphics/shapes/Shape.hpp
  ${HDR_BASE}/graphics/shapes/Point.hpp
  ${HDR_BASE}/graphics/shapes/Line.hpp
  ${HDR_BASE}/graphics/shapes/Rectangle.hpp
  ${HDR_BASE}/graphics/sprite/Sprite.hpp
  ${HDR_BASE}/graphics/sprite/SpriteBatch.hpp
  ${HDR_BASE}/graphics/sprite/SpriteSheet.hpp
  # FIXME: relocate to math unit
  ${HDR_BASE}/math/Transformable.hpp
)

set(GRAPHICS_SOURCE
  ${SRC_DIR}/graphics/Cursor.cpp
  ${SRC_DIR}/graphics/Gradient.cpp
  ${SRC_DIR}/graphics/Image.cpp
  ${SRC_DIR}/graphics/RenderWindow.cpp
  ${SRC_DIR}/graphics/Renderer.cpp
  ${SRC_DIR}/graphics/RendererInfo.cpp
  ${SRC_DIR}/graphics/Text.cpp
  ${SRC_DIR}/graphics/Texture.cpp
  ${SRC_DIR}/graphics/DisplayMode.cpp
  ${SRC_DIR}/graphics/fonts/BMFont.cpp
  ${SRC_DIR}/graphics/fonts/BitmapFont.cpp
  ${SRC_DIR}/graphics/fonts/Font.cpp
  ${SRC_DIR}/graphics/fonts/FontPage.cpp
  ${SRC_DIR}/graphics/fonts/Glyph.cpp
  ${SRC_DIR}/graphics/fonts/TrueTypeFont.cpp
  ${SRC_DIR}/graphics/graphics_helpers.cpp
  ${SRC_DIR}/graphics/shapes/Shape.cpp
  ${SRC_DIR}/graphics/shapes/Point.cpp
  ${SRC_DIR}/graphics/shapes/Line.cpp
  ${SRC_DIR}/graphics/shapes/Rectangle.cpp
  ${SRC_DIR}/graphics/sprite/Sprite.cpp
  ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
  # !! This depend on nomlib-serializers and should be relocated to said unit
  ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
  # TODO: Relocate to graphics
  ${SRC_DIR}/math/Transformable.cpp
)

# Add and link the library
add_library(nomlib-graphics
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-graphics
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

# Base unit files
target_sources(nomlib-graphics
  PRIVATE
  ${GRAPHICS_SOURCE}
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${GRAPHICS_HDR}
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  # hqx algorithm
  set(  NOM_GRAPHICS_HQX_SOURCE
        ${SRC_DIR}/graphics/hqx/hqx.cpp
        ${SRC_DIR}/graphics/hqx/hq2x.cpp
        ${SRC_DIR}/graphics/hqx/hq3x.cpp
        ${SRC_DIR}/graphics/hqx/hq4x.cpp
  )

  # scale2x algorithm
  set(  NOM_GRAPHICS_SCALE2X_SOURCE
        ${SRC_DIR}/graphics/scale2x/scale2x.cpp
        ${SRC_DIR}/graphics/scale2x/scale3x.cpp
        ${SRC_DIR}/graphics/scale2x/scale4x.cpp
  )

  target_sources(nomlib-graphics
    PRIVATE
    ${NOM_GRAPHICS_HQX_SOURCE}
    ${NOM_GRAPHICS_SCALE2X_SOURCE}
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/graphics/hqx/hqx.hpp
      ${HDR_BASE}/graphics/scale2x/scale2x.hpp
  )

endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

# This is embedded as the SONAME in the library's ELF header and determinnes
# which file the dynamic linker searches for at runtime, i.e.:
# `libnomlib-actions.so.1 -> `libnomlib-actions.so.0.11`
# The Application Binary Interface (ABI) version; PATCH level versions are
# intended **not** to break the ABI version.
set_target_properties(nomlib-graphics
  PROPERTIES
    VERSION "${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSION_REQUIRED OFF
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-graphics
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-graphics
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_include_directories(nomlib-graphics
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
)

# Common dependencies on all platforms
target_link_libraries(nomlib-graphics
  PUBLIC
  nomlib-core
  #nomlib-file
  nomlib-math
  nomlib-ptree
  # !! We must relocate SpriteSheet before we can remove this dependency
  nomlib-serializers
  nomlib-system
  PRIVATE
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

if(PLATFORM_OSX)
  # MacOSX Framework bundle support
  set_target_properties(nomlib-graphics
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-graphics
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-graphics"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
  )

  install(TARGETS nomlib-graphics
    EXPORT libframework
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-graphics.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-graphics.framework/Headers/graphics"
      COMPONENT Development
  )
endif(PLATFORM_OSX)

install(TARGETS nomlib-graphics
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/graphics"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)


