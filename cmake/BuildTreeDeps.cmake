cmake_minimum_required( VERSION 2.6 )

# Set the required build unit dependencies based on the chosen CMake options

if( NOM_BUILD_MATH_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
endif( NOM_BUILD_MATH_UNIT )

if( NOM_BUILD_AUDIO_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
  # FIXME: I *think* we may only need clock and perhaps other minor utils from
  # the System build unit...
  set( NOM_BUILD_SYSTEM_UNIT TRUE )
endif( NOM_BUILD_AUDIO_UNIT )

if( NOM_BUILD_GRAPHICS_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
endif( NOM_BUILD_GRAPHICS_UNIT )

if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
  set( NOM_BUILD_GRAPHICS_UNIT TRUE )
endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

if( NOM_BUILD_SYSTEM_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_GRAPHICS_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
endif( NOM_BUILD_SYSTEM_UNIT )

if( NOM_BUILD_PTREE_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
endif( NOM_BUILD_PTREE_UNIT )

if( NOM_BUILD_SERIALIZERS_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
  set( NOM_BUILD_PTREE_UNIT TRUE )
endif( NOM_BUILD_SERIALIZERS_UNIT )

if( NOM_BUILD_JSON_UNIT )
  # set( NOM_BUILD_JSONCPP TRUE ) # TODO
  set( NOM_BUILD_SERIALIZERS_UNIT TRUE )
  set( NOM_BUILD_PTREE_UNIT TRUE )
endif( NOM_BUILD_JSON_UNIT )

if( NOM_BUILD_XML_UNIT )
  # set( NOM_BUILD_RAPIDXML TRUE )  # TODO
  set( NOM_BUILD_SERIALIZERS_UNIT TRUE )
  set( NOM_BUILD_PTREE_UNIT TRUE )
endif( NOM_BUILD_XML_UNIT )

if( NOM_BUILD_GUI_UNIT )
  set( NOM_BUILD_CORE_UNIT TRUE )
  set( NOM_BUILD_MATH_UNIT TRUE )
  set( NOM_BUILD_GRAPHICS_UNIT TRUE )
  # FIXME?
  set( NOM_BUILD_SERIALIZERS_UNIT TRUE )
  # FIXME?
  set( NOM_BUILD_JSON_UNIT TRUE )
  set( NOM_BUILD_SYSTEM_UNIT TRUE )
endif( NOM_BUILD_GUI_UNIT )

# Stub option; not yet implemented ... experimental use has been implemented in
# examples/device_info.cpp.
set( NOM_USE_SDL2_IMAGE TRUE )

# Stub option; not yet implemented ... experimental use has been implemented in
# examples/device_info.cpp.
set( NOM_USE_SDL2_TTF TRUE )

# Stub option; not yet implemented ... experimental use has been implemented in
# examples/device_info.cpp.
if( NOM_BUILD_AUDIO_UNIT )
  set( NOM_USE_OPENAL TRUE )
  set( NOM_USE_LIBSNDFILE TRUE )
endif( NOM_BUILD_AUDIO_UNIT )

# See also: src/graphics/Texture.cpp
if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  set( NOM_USE_SCALEX TRUE )
  set( NOM_USE_HQX TRUE )
endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
