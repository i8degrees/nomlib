/******************************************************************************

    Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TRANSFORMABLE_HEADERS
#define NOMLIB_TRANSFORMABLE_HEADERS

#include "config.hpp"
#include "math/Coords.hpp"
#include "math/Color.hpp"

namespace nom {

class Transformable
{
  public:
    Transformable ( void );
    Transformable ( const Coords& coords, const Color& color );
    virtual ~Transformable ( void );

    const Coords getPosition ( void ) const;
    const Coords getSize ( void ) const;
    void setPosition ( int32_t x, int32_t y );
    void setPosition ( int32_t x, int32_t y, int32_t width, int32_t height );
    void setPosition ( const Coords& coords );

    void setSize ( int32_t width, int32_t height );
    void setSize ( const Coords& size );

    void move ( uint32_t x = 0, uint32_t y = 0 );
    void move ( const Coords& offsets = Coords ( 0, 0 ) );

    const Color& getColor ( void ) const;
    void setColor ( const Color& color );
    void setColor ( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 );

  protected:
    Coords coords;
    Color color;
};


} // namespace nom

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
