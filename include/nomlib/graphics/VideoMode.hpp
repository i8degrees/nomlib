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
#ifndef NOMLIB_VIDEO_MODE_HPP
#define NOMLIB_VIDEO_MODE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "nomlib/config.hpp"

namespace nom {

class VideoMode
{
  public:
    /// Default constructor; initialize instance members to zero (0)
    VideoMode ( void );

    /// Initialize this instance with provided provided data
    VideoMode ( int32 mode_width, int32 mode_height, uint8 mode_bpp );

    /// Destructor
    ~VideoMode ( void );

    /// Pretty print the mode using <width>x<height>x<bpp> as the format string
    /// in the following form:
    ///
    ///     1440x900x32
    ///
    ///
    /// (Yes, that is one newline preceeding the mode followed by two more
    /// newlines).
    ///
    /// Outputs to the console
    void pp ( void );

  public:
    /// Horizontal number of pixels
    int32 width;

    /// Vertical number of pixels
    int32 height;

    /// Bits per pixel
    uint8 bpp;
};

/// Compare two video modes for equality
bool operator == ( const VideoMode& lhs, const VideoMode& rhs );

/// Compare two video modes for inequality
bool operator != ( const VideoMode& lhs, const VideoMode& rhs );

/// Compare two video modes for lesser than
bool operator < ( const VideoMode& lhs, const VideoMode& rhs );

/// Compare two video modes for greater than
bool operator > ( const VideoMode& lhs, const VideoMode& rhs );

/// Compare two video modes for lesser than or equal to
bool operator <= ( const VideoMode& lhs, const VideoMode& rhs );

/// Compare two video modes for greater than or equal to
bool operator >= ( const VideoMode& lhs, const VideoMode& rhs );

/// Convenience typedef
typedef std::vector<VideoMode> VideoModeList;


} // namespace nom

#endif // include guard defined

/*
  VideoMode Class Usage Example

  // Public headers interface; this includes all of nomlib's header files for
  // the graphics subsystem supported by your platform.
  #include <nomlib/graphics.hpp>

  // ...or just the header file we need here, if you the minimal route!
  #include <nomlib/graphics/VideoMode.hpp>

  nom::Display context; // Primary video surface

  // This should be safe to call before the initialization of the video display;
  // setting of the video resolution beforehand is not necessary.
  nom::VideoModeList modes = context.getVideoModes();

  // Output to the console the available modes we have
  for ( nom::uint32 idx = 0; idx != modes.size(); idx++ )
  {
    modes[idx].pp();
  }

*/
