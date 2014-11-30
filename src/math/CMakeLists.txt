# nomlib-math library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_MATH_LIBRARY "nomlib-math" )

set(  NOM_MATH_SOURCE
      ${INC_DIR}/math.hpp

      ${SRC_DIR}/math/Color4.cpp
      ${INC_DIR}/math/Color4.hpp

      ${SRC_DIR}/math/Point2.cpp
      ${INC_DIR}/math/Point2.hpp

      ${SRC_DIR}/math/Point3.cpp
      ${INC_DIR}/math/Point3.hpp

      ${SRC_DIR}/math/Rect.cpp
      ${INC_DIR}/math/Rect.hpp

      ${SRC_DIR}/math/Size2.cpp
      ${INC_DIR}/math/Size2.hpp

      ${SRC_DIR}/math/math_helpers.cpp
      ${INC_DIR}/math/math_helpers.hpp
)

# Platform-specific implementations & dependencies

# Common on all platforms
set( NOM_MATH_DEPS nomlib-core )

# Add and link the library
nom_add_library(  ${NOM_MATH_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_MATH_SOURCE}" ""
                  "${NOM_MATH_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_MATH_LIBRARY "${NOM_MATH_LIBRARY}" PARENT_SCOPE )
