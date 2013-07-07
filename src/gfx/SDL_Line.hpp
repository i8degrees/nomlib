/******************************************************************************

    SDL Line Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_LINE_HEADERS
#define NOMLIB_SDL_LINE_HEADERS

#include <cmath>
#include <vector>
#include <memory>

#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "gfx/SDL_Pixel.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  class Line: public nom::SDL_Drawable,   // "is a" relationship
              public nom::Transformable   // "has a" relationship
  {
    public:
      Line ( void );
      virtual ~Line ( void );
      Line ( const nom::Coords& coords, const nom::Color& color );
      Line ( int32_t x, int32_t y, int32_t width, int32_t height, const nom::Color& color );

      void Update ( void );
      void Draw ( void* video_buffer ) const;
    private:
      std::vector<std::unique_ptr<nom::Pixel>> pixels;
      bool updated;
  };
}

#endif // NOMLIB_SDL_LINE_HEADERS defined
