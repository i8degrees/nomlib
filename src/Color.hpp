/******************************************************************************
    Color.h

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_COLORS_HEADERS
#define NOMLIB_COLORS_HEADERS

#include <iostream>
#include <string>
#include <algorithm>

#include "SDL.h"

#include "gamelib.h"

namespace nom
{
  class Color
  {
    public:
      Color ( void );
      Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha = -1 ); // SDL_ALPHA_OPAQUE (255)
      Color ( const nom::Color& color );
      Color ( const SDL_Color& color );
      ~Color ( void );

      const nom::Color getColor ( void ) const;

      /// SDL compatibility wrappers
      SDL_Color getSDL_Color ( void ) const;
      uint32_t getColorAsInt ( void* pixel_format ) const;
      uint32_t getAlphaColorAsInt ( void* pixel_format ) const;

      /// Predefined color constants for convenience sake
      static const nom::Color Black;
      static const nom::Color White;
      static const nom::Color Red;
      static const nom::Color Green;
      static const nom::Color Blue;
      static const nom::Color Yellow;
      static const nom::Color Magenta;
      static const nom::Color Cyan;
      static const nom::Color Transparent;

      uint8_t red;
      uint8_t green;
      uint8_t blue;
      int32_t alpha;

    private:
      // ...
  };
  // FIXME: not sure why these cannot be put into the class file without
  // linking errs?
  inline bool operator == ( const nom::Color& left, const nom::Color& right )
  {
    return (left.red == right.red) &&
           (left.green == right.green) &&
           (left.blue == right.blue) &&
           (left.alpha == right.alpha);
  }

  inline bool operator != ( const nom::Color& left, const nom::Color& right )
  {
    return ! ( left == right );
  }

  /// Values that exceed 255 are clamped to 255
  inline nom::Color operator + ( const nom::Color& left, const nom::Color& right )
  {
    return nom::Color ( static_cast<uint8_t> ( std::min ( left.red + right.red, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.green + right.green, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.blue + right.blue, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.alpha + right.alpha, 255 ) )
                      );
  }

  /// Values that exceed 255 are clamped to 255
  inline nom::Color operator ++ ( nom::Color& left )
  {
    return nom::Color ( static_cast<uint8_t> ( left.red--, 255 ),
                        static_cast<uint8_t> ( left.green--, 255 ),
                        static_cast<uint8_t> ( left.blue-- , 255 ),
                        static_cast<uint8_t> ( left.alpha--, 255 )
                      );
  }

  /// Values that exceed 255 are clamped to 255
  inline nom::Color operator - ( const nom::Color& left, const nom::Color& right )
  {
    return nom::Color ( static_cast<uint8_t> ( std::min ( left.red - right.red, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.green - right.green, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.blue - right.blue, 255 ) ),
                        static_cast<uint8_t> ( std::min ( left.alpha - right.alpha, 255 ) )
                      );
  }

  /// Values that exceed 255 are clamped to 255
  inline nom::Color operator -- ( nom::Color& left )
  {
    return nom::Color ( static_cast<uint8_t> ( left.red--, 255 ),
                        static_cast<uint8_t> ( left.green--, 255 ),
                        static_cast<uint8_t> ( left.blue--, 255 ),
                        static_cast<uint8_t> ( left.alpha--, 255 )
                      );
  }

  /// Values that exceed 255 are clamped to 255
  inline nom::Color operator * ( const nom::Color& left, const nom::Color& right)
  {
    return nom::Color ( static_cast<uint8_t> ( left.red * right.red / 255 ),
                        static_cast<uint8_t> ( left.green * right.green / 255 ),
                        static_cast<uint8_t> ( left.blue * right.blue / 255 ),
                        static_cast<uint8_t> ( left.alpha * right.alpha / 255 )
                      );
  }

  inline nom::Color& operator += ( nom::Color& left, const nom::Color& right)
  {
    return left = left + right;
  }

  inline nom::Color& operator -= ( nom::Color& left, const nom::Color& right )
  {
    return left = left - right;
  }

  inline nom::Color& operator *= ( nom::Color& left, const nom::Color& right)
  {
    return left = left * right;
  }
}

#endif // NOMLIB_COLORS_HEADERS defined
