/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/core/err.hpp"

// Private headers
// #include <string>

// OpenAL Error Handling

// namespace nom {
// namespace audio {

// uint32 err()
// {
//   return 0;
// }

// const char* err_str(uint32 errno)
// {
//   const char* err_string = "ERR_NO_ERROR";

//   switch(errno) {

//     default:
//     case ERR_NO_ERROR: {
//       // ...
//     } break;

//     case ERR_SYSTEM_CALL_FAILURE: {
//       err_string = "ERR_SYSTEM_CALL_FAILURE";
//     } break;

//     case ERR_OUT_OF_MEMORY: {
//       err_string = "ERR_OUT_OF_MEMORY";
//     } break;

//     case ERR_INVALID_OPERATION: {
//       err_string = "ERR_INVALID_OPERATION";
//     } break;
//   }

//   return err_string;
// }

// void set_err(uint32 errno)
// {
//   return;
// }

// void set_err(err_t* err)
// {
//   return;
// }

// } // namespace audio
// } // namespace nom

namespace nom {
namespace priv {

void
al_err(const std::string& func, const std::string& file, uint32 line)
{
  ALenum error_code = alGetError();

  if(error_code != AL_NO_ERROR) {
    const char* err_cstr = nullptr;

    switch(error_code) {
      default: {
        err_cstr = "Unknown err";
      } break;

      case AL_NO_ERROR: {
        err_cstr = "No error";
      } break;

      case AL_INVALID_NAME: {
        err_cstr = "AL_INVALID_NAME; Invalid name (identifier).";
      } break;

      case AL_INVALID_ENUM: {
        err_cstr = "AL_INVALID_ENUM; Invalid enumeration (attribute)";
      } break;

      case AL_INVALID_VALUE: {
        err_cstr = "AL_INVALID_VALUE; Invalid value.";
      } break;

      case AL_INVALID_OPERATION: {
        err_cstr =
          "AL_INVALID_OPERATION; Requested operation is not valid.";
      } break;

      case AL_OUT_OF_MEMORY: {
        err_cstr =
          "AL_OUT_OF_MEMORY; Out of memory.";
      } break;
    } // end switch

    NOM_LOG_ERR(NOM_LOG_CATEGORY_AUDIO, err_cstr, "at", file, ":", line,
                "in", func);
    nom::set_error(err_cstr);
  } // end if AL_NO_ERROR
}

} // namespace priv
} // namespace nom
