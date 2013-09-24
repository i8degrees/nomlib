/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/graphics/scale2x/scale2x.hpp"

namespace nom {
  namespace priv {

bool scale4x  ( void* source_buffer, void* destination_buffer,
                const int32 source_width, const int32 source_height,
                const int32 depth,
                const int16 source_pitch, int16 destination_pitch
              )
{
  // Save a temporary copy of the *existing* width & height for scaling
  // calculation.
  //int32 width = source_width;
  //int32 height = source_height;

  // The existing video surface pitch (width) is used for scaling calculations.
  //uint16 srcpitch = source_pitch;

  // We must use the new video surface configuration for computing the pitch as
  // this is dependent upon width & height parameters.
  //uint16 dstpitch = destination_pitch;

  // Existing & resulting pixel arrays
  //uint8* srcpix = static_cast<uint8*> ( source_buffer );
  //uint8* dstpix = static_cast<uint8*> ( destination_buffer );

  // Use the existing video surface BPP for choosing scaling algorithm.
  switch ( depth )
  {
    default: // Err, we could not determine a valid color depth!
    {
      return false;
    } // end unsupported color depth
    break;

    case 8:
    {
      // Not implemented
    }
    break;

    case 16:
    {
      // Not implemented
    }
    break;

    case 24:
    {
      // Not implemented
    }
    break;

    case 32:
    {
      // Not implemented
    }
    break;
  } // end switch (BytesPerPixel)

  return false;
}


  } // namespace priv
} // namespace nom
