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
#include "nomlib/graphics/RendererInfo.hpp"

namespace nom {

RendererInfo::RendererInfo ( void ) {}
RendererInfo::~RendererInfo ( void ) {}

const uint32 RendererInfo::optimal_texture_format ( void ) const
{
  return this->texture_formats.front();
}

std::ostream& operator << ( std::ostream& os, const RendererInfo& info )
{
  os << "Renderer Name: " << info.name
  << std::endl << std::endl
  << "Device Capabilities: "
  << std::endl << std::endl;

  if ( info.flags & SDL_RENDERER_TARGETTEXTURE )
  {
    os << "SDL_RENDERER_TARGETTEXTURE"
    << std::endl;
  }

  if ( info.flags & SDL_RENDERER_ACCELERATED )
  {
    os << "SDL_RENDERER_ACCELERATED"
    << std::endl;
  }

  if ( info.flags & SDL_RENDERER_PRESENTVSYNC )
  {
    os << "SDL_RENDERER_PRESENTVSYNC"
    << std::endl;
  }

  os << std::endl
  << "Texture Formats: "
  << std::endl << std::endl;
  for ( nom::uint32 idx = 0; idx < info.texture_formats.size(); ++idx )
  {
    os << PIXEL_FORMAT_NAME( info.texture_formats[idx] )
    << std::endl;
  }
  os << "Texture Width: " << info.texture_width
  << std::endl;
  os << "Texture Height: " << info.texture_height
  << std::endl;

  return os;
}

} // namespace nom