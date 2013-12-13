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

    scale2x Algorithm
1. Copyright (c) Andrea Mazzoleni

******************************************************************************/
#ifndef NOMLIB_SDL2_SCALE2X_HPP
#define NOMLIB_SDL2_SCALE2X_HPP

#include <algorithm>

#include "nomlib/config.hpp"

/// Uses the AdvanceMAME bitmap scaling algorithm known as scale2x to scale
/// a surface while maintaining the quality pixel art feel of the original
/// art. The algorithm is designed to be fast enough to process 256x256
/// bitmaps in real-time.
///
/// This method re-implements the function scale2x found in the contrib/sdl
/// directory of the scale2x distribution.
///
/// See http://scale2x.sourceforge.net/
///
/// \todo Test the implementation of 8-bit, 16-bit & 24-bit video scaling
/// functions.

// C Macros used in scale2x functions
#define SCALE2x_READINT24(x) \
  ((x)[0]<<16 | (x)[1]<<8 | (x)[2])

#define SCALE2x_WRITEINT24(x, i) \
  {(x)[0]=i>>16; (x)[1]=(i>>8)&0xff; x[2]=i&0xff; }

namespace nom {
  namespace priv {

/// Note that we expect the *source* width & buffer here
/// depth is the color depth in bits -- this is commonly referred to values
/// like: 8, 16, 24, 32.
///
/// \todo IMPLEMENT ME
bool scale2x  ( void* source_buffer, void* destination_buffer,
                const int32 source_width, const int32 source_height,
                const int32 bits_per_pixel,
                const int16 source_pitch, int16 destination_pitch
              );

/// Note that we expect the *source* width & buffer here
/// depth is the color depth in bits -- this is commonly referred to values
/// like: 8, 16, 24, 32.
///
/// \todo IMPLEMENT ME
bool scale3x  ( void* source_buffer, void* destination_buffer,
                const int32 source_width, const int32 source_height,
                const int32 bits_per_pixel,
                const int16 source_pitch, int16 destination_pitch
              );

/// Note that we expect the *source* width & buffer here
/// depth is the color depth in bits -- this is commonly referred to values
/// like: 8, 16, 24, 32.
///
/// \todo IMPLEMENT ME
bool scale4x  ( void* source_buffer, void* destination_buffer,
                const int32 source_width, const int32 source_height,
                const int32 bits_per_pixel,
                const int16 source_pitch, int16 destination_pitch
              );


  } // namespace priv
} // namespace nom

#endif // include guard defined
