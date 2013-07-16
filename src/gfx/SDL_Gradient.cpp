/******************************************************************************

    SDL Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gfx/SDL_Gradient.hpp"

namespace nom {

// Default constructor; initializes with respective defaults
SDL_Gradient::SDL_Gradient ( void )  : gradient  { Color::Gray,
                                                        Color::Gray
                                                      },
                                            x_margin ( 0 ), y_margin ( 0 ),
                                            direction ( 0 )
{
NOMLIB_LOG_INFO;
}

// Constructor variant
SDL_Gradient::SDL_Gradient ( const Color& starting_color,
                                  const Color& ending_color,
                                  int32_t x, int32_t y,
                                  int32_t width, int32_t height,
                                  uint32_t direction, uint32_t x_margin,
                                  uint32_t y_margin
                                )
{
  this->gradient[0] = starting_color;
  this->gradient[1] = ending_color;

  this->coords = Coords ( x, y, width, height );

  this->x_margin = x_margin;
  this->y_margin = y_margin;
  this->direction = 0;
}

SDL_Gradient::~SDL_Gradient ( void )
{
NOMLIB_LOG_INFO;
}

Color SDL_Gradient::getStartColor ( void ) const
{
  return this->gradient[0];
}

Color SDL_Gradient::getEndColor ( void ) const
{
  return this->gradient[1];
}

void SDL_Gradient::setStartColor ( const Color& starting_color )
{
  this->gradient[0] = starting_color;
}

void SDL_Gradient::setEndColor ( const Color& ending_color )
{
  this->gradient[1] = ending_color;
}

uint32_t SDL_Gradient::getFillDirection ( void ) const
{
  return this->direction;
}

void SDL_Gradient::setFillDirection ( const uint32_t direction )
{
  this->direction = direction;
}

void SDL_Gradient::Update ( void )
{
  // TODO
}

void SDL_Gradient::Draw ( void* video_buffer ) /* const */
{
  uint32_t x_offset = this->coords.x + this->coords.width;

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( this->coords.width - this->x_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( this->coords.width - this->x_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( this->coords.width - this->x_margin );

  for ( uint32_t rows = this->coords.x; rows < x_offset - this->x_margin; rows++ )
  {
    this->rectangle = Rectangle ( Coords ( rows, this->coords.y, 1, this->coords.height - this->y_margin ), Color ( currentR, currentG, currentB ) );
    this->rectangle.Draw ( video_buffer );

    if ( this->direction == 0 )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
    else if ( this->direction == 1 ) // reversed
    {
      currentR -= destR;
      currentG -= destG;
      currentB -= destB;
    }
  } // end for coords blit loop
}


} // namespace nom
