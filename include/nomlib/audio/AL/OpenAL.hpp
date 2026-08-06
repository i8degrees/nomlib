/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_OPENAL_HEADERS
#define NOMLIB_OPENAL_HEADERS

#include "nomlib/config.hpp"

#if defined(NOM_USE_CREATIVE_OPENAL) || defined(NOM_USE_OPENAL_SOFT)
  // Use OpenAL-soft distributed SDK headers
  #include <AL/al.h>
  #include <AL/alc.h>
  #include <AL/alext.h>
#elif defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
  // Use Apple's distributed OpenAL SDK headers
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
  #include <OpenAL/MacOSX_OALExtensions.h>
#else // As per the header inclusion path that FindOpenAL.cmake gives us
  #include <al.h>
  #include <alc.h>
  #include <alext.h>
#endif

// OpenAL error checking macro for all but the audio context error handling
// The OpenAL function call is wrapped inside of this macro that needs
// checking.
#define AL_CHECK_ERR(Function) \
  ( (Function), nom::priv::al_err(NOM_FUNC, __FILE__, __LINE__) )

// OpenAL error checking macro for audio context error handling
// The OpenAL function call is wrapped inside of this macro that needs
// checking. This macro additionally requires the OpenAL device handle to be
// given.
#define ALC_CHECK_ERR(Function, device) \
  ( (Function), nom::priv::alc_err(NOM_FUNC, __FILE__, __LINE__, device) )

// Clear the error state of OpenAL -- this must be done right before a
// AL_CHECK_ERR macro is used.
#define AL_CLEAR_ERR() alGetError();

// Clear the error state of OpenAL (context specific) -- this must be done
// right before a ALC_CHECK_ERR macro is used.
#define ALC_CLEAR_ERR(device) alcGetError(device);

namespace nom {
namespace priv {

NOM_EXPORT
void al_err(const std::string& func, const std::string& file, uint32 line);
NOM_EXPORT
void alc_err(const std::string& func, const std::string& file, uint32 line,
  ALCdevice* dev = nullptr);

} // namespace priv
} // namespace nom

namespace nom {
namespace audio {

// enum AudioError
// {
//   ERR_NO_ERROR = 0,
//   ERR_SYSTEM_CALL_FAILURE,
//   ERR_OUT_OF_MEMORY,
//   ERR_INVALID_OPERATION,
// };

// struct err_t
// {
//   uint32 error_code = 0;
// };

// // uint32 err(err_t* err);
// uint32 err();
// const char* err_str(uint32 errno);

// void set_err(uint32 errno);
// void set_err(err_t* err);

} // namespace audio
} // namespace nom

#endif // NOMLIB_OPENAL_HEADERS defined

/// \class nom::OpenAL
/// \ingroup audio
///
/// \see http://ffainelli.github.io/openal-example/
