# nomlib-actions library

if(NOM_BUILD_ACTIONS_UNIT)
  # Library name
  set(NOM_ACTIONS_LIBRARY "nomlib-actions")

  # Library headers inclusion path
  include_directories("${INC_ROOT_DIR}")

  set(NOM_ACTIONS_SOURCE
      ${INC_DIR}/actions.hpp
      ${SRC_DIR}/actions/ActionTimingCurves.cpp
      ${INC_DIR}/actions/ActionTimingCurves.hpp
      ${SRC_DIR}/actions/IActionObject.cpp
      ${INC_DIR}/actions/IActionObject.hpp
      ${SRC_DIR}/actions/ActionPlayer.cpp
      ${INC_DIR}/actions/ActionPlayer.hpp
      ${SRC_DIR}/actions/CallbackAction.cpp
      ${INC_DIR}/actions/CallbackAction.hpp
      ${SRC_DIR}/actions/DispatchQueue.cpp
      ${INC_DIR}/actions/DispatchQueue.hpp
      ${SRC_DIR}/actions/GroupAction.cpp
      ${INC_DIR}/actions/GroupAction.hpp
      ${SRC_DIR}/actions/RemoveAction.cpp
      ${INC_DIR}/actions/RemoveAction.hpp
      ${SRC_DIR}/actions/RepeatForAction.cpp
      ${INC_DIR}/actions/RepeatForAction.hpp
      ${SRC_DIR}/actions/RepeatForeverAction.cpp
      ${INC_DIR}/actions/RepeatForeverAction.hpp
      ${SRC_DIR}/actions/ReversedAction.cpp
      ${INC_DIR}/actions/ReversedAction.hpp
      ${SRC_DIR}/actions/SequenceAction.cpp
      ${INC_DIR}/actions/SequenceAction.hpp
      ${SRC_DIR}/actions/WaitForDurationAction.cpp
      ${INC_DIR}/actions/WaitForDurationAction.hpp)

    # Common internal dependencies for actions
    set(NOM_ACTIONS_DEPS nomlib-core nomlib-math nomlib-system)

  if(NOM_BUILD_GRAPHICS_UNIT)
    list(APPEND NOM_ACTIONS_SOURCE
         ${SRC_DIR}/actions/AnimateTexturesAction.cpp
         ${INC_DIR}/actions/AnimateTexturesAction.hpp
         ${SRC_DIR}/actions/FadeInAction.cpp
         ${INC_DIR}/actions/FadeInAction.hpp
         ${SRC_DIR}/actions/FadeOutAction.cpp
         ${INC_DIR}/actions/FadeOutAction.hpp
         ${SRC_DIR}/actions/FadeAlphaByAction.cpp
         ${INC_DIR}/actions/FadeAlphaByAction.hpp
         ${SRC_DIR}/actions/MoveByAction.cpp
         ${INC_DIR}/actions/MoveByAction.hpp
         ${SRC_DIR}/actions/ScaleByAction.cpp
         ${INC_DIR}/actions/ScaleByAction.hpp
         ${SRC_DIR}/actions/SpriteBatchAction.cpp
         ${INC_DIR}/actions/SpriteBatchAction.hpp)

      # Internal dependencies for this action subgroup
      list(APPEND NOM_ACTIONS_DEPS nomlib-graphics)
  endif(NOM_BUILD_GRAPHICS_UNIT)

  if(NOM_BUILD_AUDIO_UNIT)
    list(APPEND NOM_ACTIONS_SOURCE
         ${SRC_DIR}/actions/FadeAudioGainBy.cpp
         ${INC_DIR}/actions/FadeAudioGainBy.hpp
         ${SRC_DIR}/actions/PlayAudioSource.cpp
         ${INC_DIR}/actions/PlayAudioSource.hpp)

    # Internal dependencies for this action subgroup
    list(APPEND NOM_ACTIONS_DEPS nomlib-audio)
  endif(NOM_BUILD_AUDIO_UNIT)

  # Add and link the library
  nom_add_library(  ${NOM_ACTIONS_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                    "${NOM_ACTIONS_SOURCE}" ""
                    "${NOM_ACTIONS_DEPS}" )

  # Re-declare globally, so other build units that depend on us can refer to
  # this variable
  # set(NOM_ACTIONS_LIBRARY ${NOM_ACTIONS_LIBRARY} PARENT_SCOPE)
endif(NOM_BUILD_ACTIONS_UNIT)
