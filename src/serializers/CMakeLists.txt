# nomlib-serializers library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_SERIALIZERS_LIBRARY "nomlib-serializers" )

set(  NOM_SERIALIZERS_SOURCE
      ${INC_DIR}/serializers.hpp

      ${SRC_DIR}/serializers/IConfigFile.cpp
      ${INC_DIR}/serializers/IConfigFile.hpp

      ${INC_DIR}/serializers/IValueDeserializer.hpp
      ${INC_DIR}/serializers/IValueSerializer.hpp

      ${SRC_DIR}/serializers/JsonConfigFile.cpp
      ${INC_DIR}/serializers/JsonConfigFile.hpp

      ${SRC_DIR}/serializers/JsonCppDeserializer.cpp
      ${INC_DIR}/serializers/JsonCppDeserializer.hpp

      ${SRC_DIR}/serializers/JsonCppSerializer.cpp
      ${INC_DIR}/serializers/JsonCppSerializer.hpp

      ${SRC_DIR}/serializers/MiniHTML.cpp
      ${INC_DIR}/serializers/MiniHTML.hpp

      ${SRC_DIR}/serializers/RapidXmlDeserializer.cpp
      ${INC_DIR}/serializers/RapidXmlDeserializer.hpp

      ${SRC_DIR}/serializers/RapidXmlSerializer.cpp
      ${INC_DIR}/serializers/RapidXmlSerializer.hpp

      # TODO: Relocate SearchPath from system to serializers?
      ${SRC_DIR}/system/SearchPath.cpp
      ${INC_DIR}/system/SearchPath.hpp

      ${INC_DIR}/serializers/serializers_config.hpp
)

# Platform-specific implementations & dependencies

# Absolute path to project's third-party 'common' directory
#
# See also: nomlib's third-party/README.md
set( NOM_THIRD_PARTY_COMMON_DIR "${PROJECT_SOURCE_DIR}/third-party/common" )

set( JSONCPP_ROOT_DIR "${NOM_THIRD_PARTY_COMMON_DIR}/jsoncpp" )
set( RAPIDXML_INCLUDE_DIR "${NOM_THIRD_PARTY_COMMON_DIR}/rapidxml" )

if( NOT EXISTS ${JSONCPP_ROOT_DIR} )
  message(  FATAL_ERROR
            "Could not find source tree for JsonCpp at ${JSONCPP_ROOT_DIR}; "
            "see nomlib's third-party/README.md for help obtaining this "
            "required dependency."
  )
else( EXISTS ${JSONCPP_ROOT_DIR} )
  message( STATUS "Found JsonCpp: ${JSONCPP_ROOT_DIR}" )
endif( NOT EXISTS ${JSONCPP_ROOT_DIR} )

if( NOT EXISTS ${RAPIDXML_INCLUDE_DIR} )
  message(  FATAL_ERROR
            "Could not find source tree for RapidXml at ${RAPIDXML_INCLUDE_DIR}; "
            "see nomlib's third-party/README.md for help obtaining this "
            "required dependency."
  )
else( EXISTS ${RAPIDXML_INCLUDE_DIR} )
  message( STATUS "Found RapidXml: ${RAPIDXML_INCLUDE_DIR}" )
endif( NOT EXISTS ${RAPIDXML_INCLUDE_DIR} )

set( JSONCPP_SOURCE
      ${JSONCPP_ROOT_DIR}/jsoncpp.cpp
      ${JSONCPP_ROOT_DIR}/json.h
)

# RapidXml is headers-only
set(  RAPIDXML_SOURCE
      ${RAPIDXML_INCLUDE_DIR}/rapidxml.hpp
      ${RAPIDXML_INCLUDE_DIR}/rapidxml_iterators.hpp
      ${RAPIDXML_INCLUDE_DIR}/rapidxml_print.hpp
      ${RAPIDXML_INCLUDE_DIR}/rapidxml_utils.hpp
)

# JsonCpp is built as part of our library source tree
list( APPEND NOM_SERIALIZERS_SOURCE ${JSONCPP_SOURCE} ${RAPIDXML_SOURCE} )

# third-party headers inclusion for JsonCpp and RapidXml
include_directories("${NOM_THIRD_PARTY_COMMON_DIR}")

# Separate third-party sources for IDE project files
source_group( "third-party\\JsonCpp" FILES ${JSONCPP_SOURCE} )
source_group( "third-party\\RapidXml" FILES ${RAPIDXML_SOURCE} )

# Common on all platforms
set( NOM_SERIALIZERS_DEPS nomlib-core nomlib-file nomlib-ptree )

# Add and link the library
nom_add_library(  ${NOM_SERIALIZERS_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_SERIALIZERS_SOURCE}" ""
                  "${NOM_SERIALIZERS_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_SERIALIZERS_LIBRARY ${NOM_SERIALIZERS_LIBRARY} PARENT_SCOPE )
