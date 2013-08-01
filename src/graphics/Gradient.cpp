/******************************************************************************

    Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/graphics/Gradient.hpp"

namespace nom {

// Default constructor; initializes with respective defaults
Gradient::Gradient ( void )  : gradient { Color::Gray,
                                                  Color::Gray
                                                },
                                        x_margin ( 0 ), y_margin ( 0 ),
                                        direction ( 0 )
{
NOM_LOG_CLASSINFO;

  this->Update();
}

// Constructor variant
Gradient::Gradient  ( const Color& starting_color,
                      const Color& ending_color,
                      int32 x, int32 y, int32 width, int32 height,
                      uint32 direction, uint32 x_margin, uint32 y_margin
                    )
{
  this->gradient[0] = starting_color;
  this->gradient[1] = ending_color;

  this->coords = Coords ( x, y, width, height );

  this->x_margin = x_margin;
  this->y_margin = y_margin;
  this->direction = 0;

  this->Update();
}

Gradient::~Gradient ( void )
{
NOM_LOG_CLASSINFO;
}

Color Gradient::getStartColor ( void ) const
{
  return this->gradient[0];
}

Color Gradient::getEndColor ( void ) const
{
  return this->gradient[1];
}

void Gradient::setStartColor ( const Color& starting_color )
{
  this->gradient[0] = starting_color;

  this->Update();
}

void Gradient::setEndColor ( const Color& ending_color )
{
  this->gradient[1] = ending_color;

  this->Update();
}

uint32 Gradient::getFillDirection ( void ) const
{
  return this->direction;
}

void Gradient::setFillDirection ( const uint32 direction )
{
  this->direction = direction;

  this->Update();
}

void Gradient::Update ( void )
{
  this->rectangles.clear();

  uint32 x_offset = this->coords.x + this->coords.width;

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( this->coords.width - this->x_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( this->coords.width - this->x_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( this->coords.width - this->x_margin );

  for ( uint32 rows = this->coords.x; rows < x_offset - this->x_margin; rows++ )
  {
    this->rectangles.push_back ( std::shared_ptr<Rectangle> ( new Rectangle ( Coords ( rows, this->coords.y, 1, this->coords.height - this->y_margin ), Color ( currentR, currentG, currentB ) ) ) );

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

void Gradient::Draw ( void* video_buffer ) const
{
  for ( auto it = this->rectangles.begin(); it != this->rectangles.end(); ++it )
  {
    std::shared_ptr<IDrawable> obj = *it;
    obj->Draw ( video_buffer );
  }
}


} // namespace nom
