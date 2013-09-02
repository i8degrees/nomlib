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

    hqx Algorithm
1. Copyright (C) 2003 Maxim Stepin ( maxst@hiend3d.com )
2. Copyright (C) 2010 Cameron Zemek ( grom@zeminvaders.net)

******************************************************************************/
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {
  namespace priv {

uint32 RGBtoYUV[16777216];
uint32 YUV1, YUV2;


  } // namespace priv

void hqxInit ( void )
{
  /* Initalize RGB to YUV lookup table */
  uint32 c, r, g, b, y, u, v;

  for (c = 0; c < 16777215; c++)
  {
    r = (c & 0xFF0000) >> 16;
    g = (c & 0x00FF00) >> 8;
    b = c & 0x0000FF;
    y = (uint32)(0.299*r + 0.587*g + 0.114*b);
    u = (uint32)(-0.169*r - 0.331*g + 0.5*b) + 128;
    v = (uint32)(0.5*r - 0.419*g - 0.081*b) + 128;
    priv::RGBtoYUV[c] = (y << 16) + (u << 8) + v;
  }
}


} // namespace nom
