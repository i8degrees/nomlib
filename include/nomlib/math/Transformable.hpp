/******************************************************************************

    Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TRANSFORMABLE_HEADERS
#define NOMLIB_TRANSFORMABLE_HEADERS

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"

namespace nom {

class Transformable
{
  public:
    Transformable ( void );
    Transformable ( const Coords& coords, const Color& color );
    virtual ~Transformable ( void );

    const Coords getPosition ( void ) const;
    const Coords getSize ( void ) const;
    void setPosition ( int32 x, int32 y );
    void setPosition ( int32 x, int32 y, int32 width, int32 height );
    void setPosition ( const Coords& coords );

    void setSize ( int32 width, int32 height );
    void setSize ( const Coords& size );

    void move ( uint32 x = 0, uint32 y = 0 );
    void move ( const Coords& offsets = Coords ( 0, 0 ) );

    const Color& getColor ( void ) const;
    void setColor ( uint8 red, uint8 green, uint8 blue, uint8 alpha = 255 );
    void setColor ( const Color& color );

  protected:
    Coords coords;
    Color color;
};


} // namespace nom

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
