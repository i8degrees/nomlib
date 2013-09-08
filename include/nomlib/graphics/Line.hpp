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
#ifndef NOMLIB_SDL_LINE_HEADERS
#define NOMLIB_SDL_LINE_HEADERS

#include <cmath>
#include <vector>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Pixel.hpp"
#include "nomlib/system/make_unique.hpp"

namespace nom {

/// \brief Bresenham's line algorithm
class Line:
            public IDrawable
{
  public:
    Line ( void );
    Line ( const Coords& coords, const Color& color );
    Line ( int32 x, int32 y, int32 width, int32 height, const Color& color );
    virtual ~Line ( void );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    /// Vector of pointers to Pixel objects
    std::vector<std::unique_ptr<Pixel>> pixels;

    Coords coords;
    Color color;

    /// Track object logic changes for updating its rendering
    bool updated;
};


} // namespace nom

#endif // NOMLIB_SDL_LINE_HEADERS defined
