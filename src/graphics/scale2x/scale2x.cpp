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

bool scale2x  ( const Pixels source_buffer, Pixels destination_buffer,
                const int32 source_width, const int32 source_height,
                const int32 depth,
                const int16 source_pitch, int16 destination_pitch
              )
{
  // Save a temporary copy of the *existing* width & height for scaling
  // calculation.
  int32 width = source_width;
  int32 height = source_height;

  // The existing video surface pitch (width) is used for scaling calculations.
  uint16 srcpitch = source_pitch;

  // We must use the new video surface configuration for computing the pitch as
  // this is dependent upon width & height parameters.
  uint16 dstpitch = destination_pitch;

  // Existing & resulting pixel arrays
  uint8* srcpix = static_cast<uint8*> ( source_buffer );
  uint8* dstpix = static_cast<uint8*> ( destination_buffer );

  // Use the existing video surface BPP for choosing scaling algorithm.
  switch ( depth )
  {
    default: // Err, we could not determine a valid color depth!
    {
      return false;
    break;
    } // end unsupported color depth

    case 8:
    {
      uint8 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint8*)(srcpix + (std::max ( 0, looph - 1 ) * srcpitch ) + ( 1 * loopw ) );
          D = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::max ( 0,loopw - 1 ) ) );
          E = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * loopw ) );
          F = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::min ( width - 1,loopw + 1 ) ) );
          H = *(uint8*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 1 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint8*)(dstpix + looph*2*dstpitch + loopw*2*1) = E0;
          *(uint8*)(dstpix + looph*2*dstpitch + (loopw*2+1)*1) = E1;
          *(uint8*)(dstpix + (looph*2+1)*dstpitch + loopw*2*1) = E2;
          *(uint8*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*1) = E3;
        } // for width loop
      } // for height loop
    break;
    } // end case 8

    case 16:
    {
      uint16 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint16*)(srcpix + ( std::max ( 0, looph - 1 ) * srcpitch ) + ( 2 * loopw ) );
          D = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::max ( 0, loopw - 1 ) ) );
          E = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * loopw ) );
          F = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::min ( width - 1, loopw + 1 ) ) );
          H = *(uint16*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 2 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint16*)(dstpix + looph*2*dstpitch + loopw*2*2) = E0;
          *(uint16*)(dstpix + looph*2*dstpitch + (loopw*2+1)*2) = E1;
          *(uint16*)(dstpix + (looph*2+1)*dstpitch + loopw*2*2) = E2;
          *(uint16*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*2) = E3;
        } // for width loop
      } // for height loop
    break;
    } // end case 16

    case 24:
    {
      int32 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = SCALE2x_READINT24(srcpix + (std::max(0,looph-1)*srcpitch) + (3*loopw));
          D = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::max(0,loopw-1)));
          E = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*loopw));
          F = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::min(width-1,loopw+1)));
          H = SCALE2x_READINT24(srcpix + (std::min(height-1,looph+1)*srcpitch) + (3*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + loopw*2*3), E0);
          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + (loopw*2+1)*3), E1);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + loopw*2*3), E2);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*3), E3);
        } // for width loop
      } // for height loop
    break;
    } // end case 24

    case 32:
    {
      uint32 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint32*)(srcpix + (std::max(0,looph-1)*srcpitch) + (4*loopw));
          D = *(uint32*)(srcpix + (looph*srcpitch) + (4*std::max(0,loopw-1)));
          E = *(uint32*)(srcpix + (looph*srcpitch) + (4*loopw));
          F = *(uint32*)(srcpix + (looph*srcpitch) + (4*std::min(width-1,loopw+1)));
          H = *(uint32*)(srcpix + (std::min(height-1,looph+1)*srcpitch) + (4*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint32*)(dstpix + looph*2*dstpitch + loopw*2*4) = E0;
          *(uint32*)(dstpix + looph*2*dstpitch + (loopw*2+1)*4) = E1;
          *(uint32*)(dstpix + (looph*2+1)*dstpitch + loopw*2*4) = E2;
          *(uint32*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*4) = E3;
        } // for width loop
      } // for height loop
    } // end case 32
    break;
  } // end switch (BytesPerPixel)

  return true;
}


  } // namespace priv
} // namespace nom
