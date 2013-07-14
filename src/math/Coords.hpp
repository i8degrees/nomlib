/******************************************************************************

    Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_COORDS_HEADERS
#define NOMLIB_COORDS_HEADERS

#include "config.hpp"

namespace nom {

class Coords
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Coords ( void );
    /// Constructor variant for setting coords by x, y, width, height values
    Coords ( int32_t x, int32_t y, int32_t width = 0, int32_t height = 0 );
    /// Copy constructor
    Coords ( const Coords& coords );
    ~Coords ( void );

    Coords getPosition ( void ) const;
    void setPosition ( int32_t x, int32_t y );

    Coords getSize ( void ) const;
    void setSize ( int32_t width, int32_t height );

    /// \todo TEST ME
    bool contains ( int32_t x, int32_t y ) const;

    /// \todo TEST ME
    bool contains ( const Coords& pos ) const;

    /// Checks to see if our rectangle overlaps with another
    /// \todo TEST ME
    bool intersects ( Coords& rectangle ) const;

    /// Copy assignment constructor
    Coords& operator = ( const Coords& other );

  public:
    signed int x;
    signed int y;
    signed int z; // reserved
    signed int width;
    signed int height;

}; // class Coords

// FIXME: not sure why these cannot be put into the class file without
// linking errs?
inline bool operator == ( const Coords& left, const Coords& right )
{
  return (left.x == right.x ) &&
         (left.y == right.y ) &&
         (left.width == right.width ) &&
         (left.height == right.height );
}

inline bool operator != ( const Coords& left, const Coords& right )
{
  return ! ( left == right );
}

inline Coords operator + ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32_t> ( left.x + right.x ),
                    static_cast<int32_t> ( left.y + right.y ),
                    static_cast<int32_t> ( left.width + right.width ),
                    static_cast<int32_t> ( left.height + right.height )
                  );
}

inline Coords operator ++ ( Coords& left )
{
  return Coords  (  static_cast<int32_t> ( left.x ++ ),
                    static_cast<int32_t> ( left.y ++ ),
                    static_cast<int32_t> ( left.width ++ ),
                    static_cast<int32_t> ( left.height ++ )
                  );
}

inline Coords operator - ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32_t> ( left.x - right.x ),
                    static_cast<int32_t> ( left.y - right.y ),
                    static_cast<int32_t> ( left.width - right.width ),
                    static_cast<int32_t> ( left.height - right.height )
                  );
}

inline Coords operator -- ( Coords& left )
{
  return Coords  (  static_cast<int32_t> ( left.x -- ),
                    static_cast<int32_t> ( left.y -- ),
                    static_cast<int32_t> ( left.width -- ),
                    static_cast<int32_t> ( left.height -- )
                  );
}

inline Coords operator * ( const Coords& left, const Coords& right)
{
  return Coords  ( static_cast<int32_t> ( left.x * right.x ),
                        static_cast<int32_t> ( left.y * right.y ),
                        static_cast<int32_t> ( left.width * right.width ),
                        static_cast<int32_t> ( left.height * right.height )
                      );
}

inline Coords operator / ( const Coords& left, const Coords& right)
{
  return Coords  (  static_cast<int32_t> ( left.x / right.x ),
                    static_cast<int32_t> ( left.y / right.y ),
                    static_cast<int32_t> ( left.width / right.width ),
                    static_cast<int32_t> ( left.height / right.height )
                  );
}

inline Coords& operator += ( Coords& left, const Coords& right)
{
  return left = left + right;
}

inline Coords& operator -= ( Coords& left, const Coords& right )
{
  return left = left - right;
}

inline Coords& operator *= ( Coords& left, const Coords& right)
{
  return left = left * right;
}


} // namespace nom

#endif // NOMLIB_COORDS_HEADERS defined
