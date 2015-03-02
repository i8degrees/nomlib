# nomlib-graphics library

if( NOM_BUILD_GRAPHICS_UNIT )

  # Library headers inclusion path
  include_directories( "${INC_ROOT_DIR}" )

  # Library name
  set( NOM_GRAPHICS_LIBRARY "nomlib-graphics" )

  set(  NOM_GRAPHICS_SOURCE
        ${INC_DIR}/graphics.hpp

        ${SRC_DIR}/graphics/Cursor.cpp
        ${INC_DIR}/graphics/Cursor.hpp

        ${SRC_DIR}/graphics/Gradient.cpp
        ${INC_DIR}/graphics/Gradient.hpp

        ${INC_DIR}/graphics/IDrawable.hpp

        ${SRC_DIR}/graphics/Image.cpp
        ${INC_DIR}/graphics/Image.hpp

        ${SRC_DIR}/graphics/RenderWindow.cpp
        ${INC_DIR}/graphics/RenderWindow.hpp

        ${SRC_DIR}/graphics/Renderer.cpp
        ${INC_DIR}/graphics/Renderer.hpp

        ${SRC_DIR}/graphics/RendererInfo.cpp
        ${INC_DIR}/graphics/RendererInfo.hpp

        ${SRC_DIR}/graphics/Text.cpp
        ${INC_DIR}/graphics/Text.hpp

        ${SRC_DIR}/graphics/Texture.cpp
        ${INC_DIR}/graphics/Texture.hpp

        ${SRC_DIR}/graphics/DisplayMode.cpp
        ${INC_DIR}/graphics/DisplayMode.hpp

        ${SRC_DIR}/graphics/fonts/BMFont.cpp
        ${INC_DIR}/graphics/fonts/BMFont.hpp

        ${SRC_DIR}/graphics/fonts/BitmapFont.cpp
        ${INC_DIR}/graphics/fonts/BitmapFont.hpp

        ${SRC_DIR}/graphics/fonts/Font.cpp
        ${INC_DIR}/graphics/fonts/Font.hpp

        ${INC_DIR}/graphics/fonts/FontMetrics.hpp

        ${SRC_DIR}/graphics/fonts/FontPage.cpp
        ${INC_DIR}/graphics/fonts/FontPage.hpp

        ${INC_DIR}/graphics/fonts/FontRow.hpp

        ${SRC_DIR}/graphics/fonts/Glyph.cpp
        ${INC_DIR}/graphics/fonts/Glyph.hpp

        ${INC_DIR}/graphics/fonts/IFont.hpp

        ${SRC_DIR}/graphics/fonts/TrueTypeFont.cpp
        ${INC_DIR}/graphics/fonts/TrueTypeFont.hpp

        ${SRC_DIR}/graphics/graphics_helpers.cpp
        ${INC_DIR}/graphics/graphics_helpers.hpp

        ${SRC_DIR}/graphics/shapes/Shape.cpp
        ${INC_DIR}/graphics/shapes/Shape.hpp

        ${SRC_DIR}/graphics/shapes/Point.cpp
        ${INC_DIR}/graphics/shapes/Point.hpp

        ${SRC_DIR}/graphics/shapes/Line.cpp
        ${INC_DIR}/graphics/shapes/Line.hpp

        ${SRC_DIR}/graphics/shapes/Rectangle.cpp
        ${INC_DIR}/graphics/shapes/Rectangle.hpp

        ${SRC_DIR}/graphics/sprite/Sprite.cpp
        ${INC_DIR}/graphics/sprite/Sprite.hpp

        ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
        ${INC_DIR}/graphics/sprite/SpriteBatch.hpp

        # FIXME: This depend on serializers
        ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
        ${INC_DIR}/graphics/sprite/SpriteSheet.hpp

        # TODO: Relocate to graphics
        ${SRC_DIR}/math/Transformable.cpp
        ${INC_DIR}/math/Transformable.hpp
  )

  # TODO: Consider breaking the actions engine off into its own library? It
  # appears to be a strong candidate for doing so. I'll see how future build
  # times are affected by leaving things as is for now -- because we are able
  # to forward declare all graphical components used here, we may well be OK.
  set(  NOM_ACTIONS_SOURCE
        ${INC_DIR}/actions.hpp
        ${SRC_DIR}/actions/ActionTimingCurves.cpp
        ${INC_DIR}/actions/ActionTimingCurves.hpp
        ${SRC_DIR}/actions/IActionObject.cpp
        ${INC_DIR}/actions/IActionObject.hpp
        ${SRC_DIR}/actions/SpriteTexturesAction.cpp
        ${INC_DIR}/actions/SpriteTexturesAction.hpp
        ${SRC_DIR}/actions/SpriteBatchAction.cpp
        ${INC_DIR}/actions/SpriteBatchAction.hpp
        ${SRC_DIR}/actions/FadeInAction.cpp
        ${INC_DIR}/actions/FadeInAction.hpp
        ${SRC_DIR}/actions/FadeOutAction.cpp
        ${INC_DIR}/actions/FadeOutAction.hpp
        ${SRC_DIR}/actions/FadeAlphaByAction.cpp
        ${INC_DIR}/actions/FadeAlphaByAction.hpp
        ${SRC_DIR}/actions/WaitForDurationAction.cpp
        ${INC_DIR}/actions/WaitForDurationAction.hpp
        ${SRC_DIR}/actions/MoveByAction.cpp
        ${INC_DIR}/actions/MoveByAction.hpp
        ${SRC_DIR}/actions/ScaleByAction.cpp
        ${INC_DIR}/actions/ScaleByAction.hpp
        ${SRC_DIR}/actions/ReversedAction.cpp
        ${INC_DIR}/actions/ReversedAction.hpp
        ${SRC_DIR}/actions/RepeatForAction.cpp
        ${INC_DIR}/actions/RepeatForAction.hpp
        ${SRC_DIR}/actions/RepeatForeverAction.cpp
        ${INC_DIR}/actions/RepeatForeverAction.hpp
        ${SRC_DIR}/actions/CallbackAction.cpp
        ${INC_DIR}/actions/CallbackAction.hpp
        ${SRC_DIR}/actions/RemoveAction.cpp
        ${INC_DIR}/actions/RemoveAction.hpp
        ${SRC_DIR}/actions/GroupAction.cpp
        ${INC_DIR}/actions/GroupAction.hpp
        ${SRC_DIR}/actions/SequenceAction.cpp
        ${INC_DIR}/actions/SequenceAction.hpp
        ${SRC_DIR}/actions/ColorizeAction.cpp
        ${INC_DIR}/actions/ColorizeAction.hpp
        ${SRC_DIR}/actions/DispatchQueue.cpp
        ${INC_DIR}/actions/DispatchQueue.hpp
        ${SRC_DIR}/actions/ActionPlayer.cpp
        ${INC_DIR}/actions/ActionPlayer.hpp )

  # Actions engine
  list( APPEND NOM_GRAPHICS_SOURCE ${NOM_ACTIONS_SOURCE} )

  # Common dependencies on all platforms
  set(  NOM_GRAPHICS_DEPS nomlib-core nomlib-math nomlib-file
        nomlib-serializers nomlib-system )
        # ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY}

  if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

    # hqx algorithm (third-party)
    set(  NOM_GRAPHICS_HQX_SOURCE
          ${INC_DIR}/graphics/hqx/hqx.hpp
          ${SRC_DIR}/graphics/hqx/hqx.cpp
          ${SRC_DIR}/graphics/hqx/hq2x.cpp
          ${SRC_DIR}/graphics/hqx/hq3x.cpp
          ${SRC_DIR}/graphics/hqx/hq4x.cpp
    )

    # scale2x algorithm (third-party)
    set(  NOM_GRAPHICS_SCALE2X_SOURCE
          ${INC_DIR}/graphics/scale2x/scale2x.hpp
          ${SRC_DIR}/graphics/scale2x/scale2x.cpp
          ${SRC_DIR}/graphics/scale2x/scale3x.cpp
          ${SRC_DIR}/graphics/scale2x/scale4x.cpp
    )

    list( APPEND NOM_GRAPHICS_SOURCE
          ${NOM_GRAPHICS_HQX_SOURCE} ${NOM_GRAPHICS_SCALE2X_SOURCE} )

  endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

  # Platform-specific implementations & dependencies

  find_package( SDL2_image REQUIRED )
  if( SDL2_IMAGE_FOUND )
    # Add development header files; SDL_image.h
    include_directories( ${SDL2_IMAGE_INCLUDE_DIR} )
  endif( SDL2_IMAGE_FOUND )

  find_package( SDL2_ttf REQUIRED )
  if( SDL2_TTF_FOUND )
    # Add development header files; SDL_ttf.h & friends
    include_directories( ${SDL2_TTF_INCLUDE_DIR} )
  endif( SDL2_TTF_FOUND )

  # Add and link the library
  nom_add_library(  ${NOM_GRAPHICS_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                    "${NOM_GRAPHICS_SOURCE}" ""
                    "${NOM_GRAPHICS_DEPS}" )

  # Re-declare globally, so other build units that depend on us can refer to
  # this variable
  # set( NOM_GRAPHICS_LIBRARY ${NOM_GRAPHICS_LIBRARY} PARENT_SCOPE )

endif( NOM_BUILD_GRAPHICS_UNIT )
