/******************************************************************************
    SDL_Gradient.cpp

  SDL Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Gradient.hpp"

nom::SDL_Gradient::SDL_Gradient ( void )
{
  #ifdef DEBUG_SDL_GRADIENT_OBJ
    std::cout << "nom::SDL_Gradient::SDL_Gradient (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->gradient[0] = nom::Color ( 66, 66, 66 ); // starting color defaults
  this->gradient[1] = nom::Color ( 99, 99, 99 ); // ending color defaults

  this->coords = nom::Coords ( 0, 0, 0, 0 ); // zero out dem blitting coords

  this->x_margin = 0;
  this->y_margin = 0;
  this->direction = 0;
}

void nom::SDL_Gradient::Init (  const nom::Color& starting_color, const nom::Color& ending_color,
                          unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int direction, unsigned int x_margin,
                          unsigned int y_margin )
{
  this->gradient[0] = starting_color;
  this->gradient[1] = ending_color;

  this->coords = nom::Coords ( x, y, width, height ); // initialize dem coords

  this->x_margin = x_margin; // offsets += 3 unless initialized in args list or not calling Init
  this->y_margin = y_margin; // offsets += 4 unless initialized in args list or not calling Init
  this->direction = 0; // direction in which gradient starts from; one (1) reverses direction
}

nom::SDL_Gradient::~SDL_Gradient ( void )
{
  #ifdef DEBUG_SDL_GRADIENT_OBJ
    std::cout << "nom::SDL_Gradient::~SDL_Gradient (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void nom::SDL_Gradient::Draw (  void* video_buffer, unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height, unsigned int direction
                        )
{
  unsigned int rows = 0; // iterator
  uint32_t x_offset = x + width;
  //unsigned int x_offset = x + width;
  //unsigned int y_offset = y + height;

  // width should always be one (1) with this particular blit algorithm
  this->coords = nom::Coords ( x, y, 1, height - this->y_margin );
  //this->coords = nom::Coords ( this->getX(), this->getY(), 1, ( this->getHeight() - this->y_margin ) );

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( width - this->x_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( width - this->x_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( width - this->x_margin );

  for ( rows = this->coords.x; rows < x_offset - this->x_margin; rows++ )
  {
    this->coords.x = rows;

    nom::Rectangle rect ( this->coords, nom::Color ( currentR, currentG, currentB ) );
    rect.Draw ( video_buffer );

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
