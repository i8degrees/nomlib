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
#ifndef NOMLIB_HQX_HPP
#define NOMLIB_HQX_HPP

/// Use the hqx bitmap algorithm to scale a source buffer. hqx is a
/// fast, high-quality magnification filter designed for pixel art. Compared
/// to scale2x, you can generally expect similar results but with additional
/// anti-aliasing applied. This makes the algorithm likely to be preferable
/// for vastly increased resolution support (think: Apple Retina displays).
///
/// The algorithm is designed to be fast enough to process 256x256 bitmaps
/// in real-time.
///
/// See https://code.google.com/p/hqx/wiki/ReadMe
///
/// \todo Test the implementation of 8-bit, 16-bit & 24-bit video scaling
/// functions.
///
/// \todo FIXME; due to some bizarre linking issue resulting in unresolved
/// symbols upon trying to use any of the function calls (such as hqxInit),
/// so we have had to resort to forking a copy of the original source to get
/// this working.

#include "nomlib/config.hpp"

#define MASK_2     0x0000FF00
#define MASK_13    0x00FF00FF
#define MASK_RGB   0x00FFFFFF
#define MASK_ALPHA 0xFF000000

#define Ymask 0x00FF0000
#define Umask 0x0000FF00
#define Vmask 0x000000FF
#define trY   0x00300000
#define trU   0x00000700
#define trV   0x00000006

namespace nom {
  namespace priv {

/* RGB to YUV lookup table */
extern uint32 RGBtoYUV[16777216];

static inline uint32 rgb_to_yuv(uint32 c)
{
    // Mask against MASK_RGB to discard the alpha channel
    return RGBtoYUV[MASK_RGB & c];
}

/* Test if there is difference in color */
static inline int32 yuv_diff(uint32 yuv1, uint32 yuv2) {
    return (( abs((yuv1 & Ymask) - (yuv2 & Ymask)) > trY ) ||
            ( abs((yuv1 & Umask) - (yuv2 & Umask)) > trU ) ||
            ( abs((yuv1 & Vmask) - (yuv2 & Vmask)) > trV ) );
}

static inline int32 Diff(uint32 c1, uint32 c2)
{
    return yuv_diff(rgb_to_yuv(c1), rgb_to_yuv(c2));
}

/* Interpolate functions */
static inline uint32 Interpolate_2(uint32 c1, int w1, uint32 c2, int32 w2, int32 s)
{
    if (c1 == c2) {
        return c1;
    }
    return
        (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2) << (24-s)) & MASK_ALPHA) +
        ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2) >> s) & MASK_2) +
        ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2) >> s) & MASK_13);
}

static inline uint32 Interpolate_3(uint32 c1, int32 w1, uint32 c2, int32 w2, uint32 c3, int32 w3, int32 s)
{
    return
        (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2 + ((c3 & MASK_ALPHA) >> 24) * w3) << (24-s)) & MASK_ALPHA) +
        ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2 + (c3 & MASK_2) * w3) >> s) & MASK_2) +
        ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2 + (c3 & MASK_13) * w3) >> s) & MASK_13);
}

static inline uint32 Interp1(uint32 c1, uint32 c2)
{
    //(c1*3+c2) >> 2;
    return Interpolate_2(c1, 3, c2, 1, 2);
}

static inline uint32 Interp2(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*2+c2+c3) >> 2;
    return Interpolate_3(c1, 2, c2, 1, c3, 1, 2);
}

static inline uint32 Interp3(uint32 c1, uint32 c2)
{
    //(c1*7+c2)/8;
    return Interpolate_2(c1, 7, c2, 1, 3);
}

static inline uint32 Interp4(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*2+(c2+c3)*7)/16;
    return Interpolate_3(c1, 2, c2, 7, c3, 7, 4);
}

static inline uint32 Interp5(uint32 c1, uint32 c2)
{
    //(c1+c2) >> 1;
    return Interpolate_2(c1, 1, c2, 1, 1);
}

static inline uint32 Interp6(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*5+c2*2+c3)/8;
    return Interpolate_3(c1, 5, c2, 2, c3, 1, 3);
}

static inline uint32 Interp7(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*6+c2+c3)/8;
    return Interpolate_3(c1, 6, c2, 1, c3, 1, 3);
}

static inline uint32 Interp8(uint32 c1, uint32 c2)
{
    //(c1*5+c2*3)/8;
    return Interpolate_2(c1, 5, c2, 3, 3);
}

static inline uint32 Interp9(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*2+(c2+c3)*3)/8;
    return Interpolate_3(c1, 2, c2, 3, c3, 3, 3);
}

static inline uint32 Interp10(uint32 c1, uint32 c2, uint32 c3)
{
    //(c1*14+c2+c3)/16;
    return Interpolate_3(c1, 14, c2, 1, c3, 1, 4);
}

/// Internal function for rescaling using hq2x algorithm
void hq2x_32_rb( uint32* sp, uint32 srb, uint32* dp, uint32 drb, int32 Xres, int32 Yres );

/// Internal function for rescaling using hq3x algorithm
void hq3x_32_rb( uint32* sp, uint32 srb, uint32* dp, uint32 drb, int32 Xres, int32 Yres );

/// Internal function for rescaling using hq4x algorithm
void hq4x_32_rb( uint32* sp, uint32 srb, uint32* dp, uint32 drb, int32 Xres, int32 Yres );

/// Public interface for initialization of hqx algorithm
void hqxInit ( void );

/// Public interface for scaling a video surface with the hq2x algorithm
///
/// Note that we expect a *source* width & height.
void hq2x_32 ( uint32* src, uint32* dest, int32 width, int32 height );

/// Public interface for scaling a video surface with the hq3x algorithm
///
/// Note that we expect a *source* width & height.
void hq3x_32 ( uint32* src, uint32* dest, int32 width, int32 height );

/// Public interface for scaling a video surface with the hq4x algorithm
///
/// Note that we expect a *source* width & height.
void hq4x_32 ( uint32* src, uint32* dest, int32 width, int32 height );


  } // namespace priv
} // namespace nom

#endif // include guard defined
