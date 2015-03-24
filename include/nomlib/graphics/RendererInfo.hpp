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
#ifndef NOMLIB_SDL2_RENDERER_INFO_HPP
#define NOMLIB_SDL2_RENDERER_INFO_HPP

#include <iostream>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

/// \brief Container class for the current renderer driver capabilities
struct RendererInfo
{
  public:
    /// Raw pointer type
    typedef RendererInfo* RawPtr;

    /// Default constructor
    RendererInfo ( void );

    /// Destructor
    ~RendererInfo ( void );

    /// \brief Query driver's name.
    const std::string& name( void ) const;

    /// \brief    Obtain the most optimal texture format available.
    ///
    /// \remarks  Value returned is graphics hardware dependent.
    uint32 optimal_texture_format ( void ) const;

    /// \brief    Query support for setting a nom::Texture as a
    ///           nom::Renderer target.
    bool target_texture ( void ) const;

    /// \brief Query driver's support of hardware video acceleration.
    bool accelerated( void ) const;

    /// \brief Query driver's support of VSYNC.
    bool vsync( void ) const;

    /// \brief Get the maximum texture width supported by the driver.
    int texture_width( void ) const;

    /// \brief Get the maximum texture height supported by the driver.
    int texture_height( void ) const;

    /// Renderer designation
    std::string name_;

    /// Mask of supported renderer flags
    uint32 flags_;

    /// Available texture (pixel) formats
    ///
    /// \note The most optimal pixel format will always be the first element
    std::vector<uint32> texture_formats;

    /// Maximum texture width (in pixels) supported
    int texture_width_;

    /// Maximum texture height (in pixels) supported
    int texture_height_;
};

/// Pretty print the available device capabilities
std::ostream& operator << ( std::ostream& os, const RendererInfo& info );

} // namespace nom

#endif // include guard defined
