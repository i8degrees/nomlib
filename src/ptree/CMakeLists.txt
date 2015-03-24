# nomlib-ptree library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_PTREE_LIBRARY "nomlib-ptree" )

set(  NOM_PTREE_SOURCE
      ${INC_DIR}/ptree.hpp

      ${INC_DIR}/ptree/Object.hpp

      ${SRC_DIR}/ptree/VString.cpp
      ${INC_DIR}/ptree/VString.hpp

      ${SRC_DIR}/ptree/Value.cpp
      ${INC_DIR}/ptree/Value.hpp

      ${SRC_DIR}/ptree/ValueConstIterator.cpp
      ${INC_DIR}/ptree/ValueConstIterator.hpp

      ${SRC_DIR}/ptree/ValueIterator.cpp
      ${INC_DIR}/ptree/ValueIterator.hpp

      ${SRC_DIR}/ptree/ValueIteratorBase.cpp
      ${INC_DIR}/ptree/ValueIteratorBase.hpp

      ${INC_DIR}/ptree/ptree_config.hpp
      ${INC_DIR}/ptree/ptree_forwards.hpp
      ${INC_DIR}/ptree/ptree_types.hpp
)

# Platform-specific implementations & dependencies

# Common on all platforms
set( NOM_PTREE_DEPS nomlib-core )

# Add and link the library
nom_add_library(  ${NOM_PTREE_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_PTREE_SOURCE}" ""
                  "${NOM_PTREE_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_PTREE_LIBRARY ${NOM_PTREE_LIBRARY} PARENT_SCOPE )
