#pragma once

#include <string>
#include <cstdint>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

//#define __FILE__ "__FILE__"
//#define __FILE__
#define NOM_FUNC \
  __PRETTY_FUNCTION__

// OpenAL error checking macro for all but the audio context error handling
// The OpenAL function call is wrapped inside of this macro that needs
// checking.
#define AL_CHECK_ERR(Function) \
  ( Function, al_err(NOM_FUNC, __FILE__, __LINE__) )

// OpenAL error checking macro for audio context error handling
// The OpenAL function call is wrapped inside of this macro that needs
// checking. This macro additionally requires the OpenAL device handle to be
// given.
#define ALC_CHECK_ERR(Function, device) \
  ( Function, alc_err(NOM_FUNC, __FILE__, __LINE__, device) )

// Clear the error state of OpenAL -- this must be done right before a
// AL_CHECK_ERR macro is used.
#define AL_CLEAR_ERR() alGetError();

// Clear the error state of OpenAL (context specific) -- this must be done
// right before a ALC_CHECK_ERR macro is used.
#define ALC_CLEAR_ERR(device) alcGetError(device);

namespace nom::priv {
  ALenum al_err(const std::string& func, const std::string& file, uint32_t line);
  ALenum alc_err(const std::string& func, const std::string& file, uint32_t line,
    ALCdevice* dev = nullptr);
} // namespace nom::priv

//typedef struct ALCcontext_struct ALCcontext;
//typedef struct ALCdevice_struct ALCdevice;
