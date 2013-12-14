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
#ifndef NOMLIB_SDL2_RENDERER_INFO_HPP
#define NOMLIB_SDL2_RENDERER_INFO_HPP

#include <iostream>
#include <string>
#include <vector>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

/// \brief Container class for the current renderer driver capabilities
class RendererInfo
{
  public:
    /// Raw pointer type
    typedef RendererInfo* RawPtr;

    /// Default constructor
    RendererInfo ( void );

    /// Destructor
    ~RendererInfo ( void );

    /// Obtain the most optimal texture (pixel) format available
    const uint32 optimal_texture_format ( void ) const;

  public:
    /// Renderer designation
    std::string name;

    /// Mask of supported renderer flags
    uint32 flags;

    /// Available texture (pixel) formats
    ///
    /// \note The most optimal pixel format will always be the first element
    std::vector<uint32> texture_formats;

    /// Maximum texture width (in pixels) supported
    int texture_width;

    /// Maximum texture height (in pixels) supported
    int texture_height;
};

/// Pretty print the available device capabilities
std::ostream& operator << ( std::ostream& os, const RendererInfo& info );

} // namespace nom

#endif // include guard defined
