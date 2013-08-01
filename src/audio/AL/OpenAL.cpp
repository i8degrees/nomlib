/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Portions Copyright (c) 2004-2013 Lazy Foo' Productions [1]
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

1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#include "nomlib/audio/AL/OpenAL.hpp"

// OpenAL Error Handling

namespace nom {
  namespace priv {

void al_err ( const std::string& file, uint32 line )
{
  ALenum error_code = alGetError();

  if ( error_code != AL_NO_ERROR )
  {
    std::string error, description;

    switch ( error_code )
    {
      default:
      {
        error = "Unknown";
        description = "An unknown error has occurred.";
      }
      break;

      case AL_INVALID_NAME:
      {
        error = "AL_INVALID_NAME";
        description = "Invalid name (identifier).";
      }
      break;

      case AL_INVALID_ENUM:
      {
        error = "AL_INVALID_ENUM";
        description = "Invalid enumeration (attribute).";
      }
      break;

      case AL_INVALID_VALUE:
      {
        error = "AL_INVALID_VALUE";
        description = "Invalid value.";
      }
      break;

      case AL_INVALID_OPERATION:
      {
        error = "AL_INVALID_OPERATION";
        description = "Requested operation is not valid.";
      }
      break;

      case AL_OUT_OF_MEMORY:
      {
        error = "AL_OUT_OF_MEMORY";
        description = "Requested operation resulted in OpenAL running out of memory";
      }
      break;
    } // end switch

  std::cout << "NOM_LOG_ERR at " << getCurrentTime() << "In file " << file << ":" << line << std::endl << "Error: " << error << ", " << description << std::endl << std::endl;

  } // end if AL_NO_ERROR
}


  } // namespace priv
} // namespace nom
