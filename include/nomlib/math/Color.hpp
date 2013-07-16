/******************************************************************************

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_COLORS_HEADERS
#define NOMLIB_COLORS_HEADERS

#include <algorithm>

#include "nomlib/config.hpp"

namespace nom {

class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void );
    /// Constructor variant for setting a color using RGBA values
    Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha = -1 ); // SDL_ALPHA_OPAQUE (255)
    /// Copy constructor
    Color ( const Color& color );
    ~Color ( void );

    /// Convenience getter helper for obtaining a color by object
    const Color getColor ( void ) const;

    /// Copy assignment constructor
    Color& operator = ( const Color& other );

    /// Predefined color constants for convenience sake
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;

    /// Additional predefined color constants
    static const Color Gray;

  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    int32_t alpha;

}; // class Color

// FIXME: not sure why these cannot be put into the class file without
// linking errs?
inline bool operator == ( const Color& left, const Color& right )
{
  return (left.red == right.red ) &&
         (left.green == right.green ) &&
         (left.blue == right.blue ) &&
         (left.alpha == right.alpha );
}

inline bool operator != ( const Color& left, const Color& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
inline Color operator + ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8_t> ( std::min ( left.red + right.red, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.green + right.green, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.blue + right.blue, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.alpha + right.alpha, 255 ) )
                );
}

/// Values that exceed 255 are clamped to 255
inline Color operator ++ ( Color& left )
{
  return Color  ( static_cast<uint8_t> ( left.red-- ),
                  static_cast<uint8_t> ( left.green-- ),
                  static_cast<uint8_t> ( left.blue-- ),
                  static_cast<uint8_t> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
inline Color operator - ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8_t> ( std::min ( left.red - right.red, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.green - right.green, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.blue - right.blue, 255 ) ),
                  static_cast<uint8_t> ( std::min ( left.alpha - right.alpha, 255 ) )
                );
}

/// Values that exceed 255 are clamped to 255
inline Color operator -- ( Color& left )
{
  return Color  ( static_cast<uint8_t> ( left.red-- ),
                  static_cast<uint8_t> ( left.green-- ),
                  static_cast<uint8_t> ( left.blue-- ),
                  static_cast<uint8_t> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
inline Color operator * ( const Color& left, const Color& right)
{
  return Color  ( static_cast<uint8_t> ( left.red * right.red / 255 ),
                  static_cast<uint8_t> ( left.green * right.green / 255 ),
                  static_cast<uint8_t> ( left.blue * right.blue / 255 ),
                  static_cast<uint8_t> ( left.alpha * right.alpha / 255 )
                );
}

inline Color& operator += ( Color& left, const Color& right)
{
  return left = left + right;
}

inline Color& operator -= ( Color& left, const Color& right )
{
  return left = left - right;
}

inline Color& operator *= ( Color& left, const Color& right)
{
  return left = left * right;
}


} // namespace nom

#endif // NOMLIB_COLORS_HEADERS defined
