/******************************************************************************
    SDLGradient.cpp

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDLGradient.h"

SDLGradient::SDLGradient ( void )
{
  #ifdef DEBUG_GRADIENT_OBJ
    std::cout << "SDLGradient::SDLGradient (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->gradient[0].setColor ( 0, 0, 0 ); // starting color defaults
  this->gradient[1].setColor ( 255, 255, 255 ); // ending color defaults

  this->coords.setCoords ( 0, 0, 0, 0 ); // zero out dem blitting coords

  this->x_margin = 0;
  this->y_margin = 0;
  this->direction = 0;
}

void SDLGradient::Init (  GColor starting_color, GColor ending_color,
                          unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int direction, unsigned int x_margin,
                          unsigned int y_margin )
{
  #ifdef DEBUG_GRADIENT_OBJ
    std::cout << "SDLGradient::SDLGradient (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->gradient[0] = starting_color;
  this->gradient[1] = ending_color;

  this->coords.setCoords ( x, y, width, height ); // initialize dem coords

  this->x_margin = x_margin; // offsets += 3 unless initialized in args list or not calling Init
  this->y_margin = y_margin; // offsets += 4 unless initialized in args list or not calling Init
  this->direction = 0; // direction in which gradient starts from; one (1) reverses direction
}

SDLGradient::~SDLGradient ( void )
{
  #ifdef DEBUG_GRADIENT_OBJ
    std::cout << "SDLGradient::~SDLGradient (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void SDLGradient::Draw (  SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height, unsigned int direction
                        )
{
  unsigned int x_offset = x + width;
  //unsigned int y_offset = y + height;

  // width should always be one (1) with this particular blit algorithm
  this->coords.setCoords ( x, y, 1, height - this->y_margin );

  #ifdef DEBUG_GRADIENT
    std::cout << "SDLGradient::LinearGradientFill (): " << "Variable Dump" << std::endl << std::endl;

    for ( int i = 0; i < 2; i++ )
    {
      std::cout << "\nRed[" << i << "]: " << this->gradient[i].getRed() << "\n";
      std::cout << "\nGreen[" << i << "]: " << this->gradient[i].getGreen() << "\n";
      std::cout << "\nBlue[" << i << "]: " << this->gradient[i].getBlue() << "\n";
    }
  #endif

  float currentR = (float) gradient[0].getRed();
  float currentG = (float) gradient[0].getGreen();
  float currentB = (float) gradient[0].getBlue();

  float destR = (float) ( gradient[1].getRed() - gradient[0].getRed() )      / ( float ) ( width - this->x_margin );
  float destG = (float) ( gradient[1].getGreen() - gradient[0].getGreen() )  / ( float ) ( width - this->x_margin );
  float destB = (float) ( gradient[1].getBlue() - gradient[0].getBlue() )    / ( float ) ( width - this->x_margin );

  unsigned int rows = this->coords.getX();

  for ( rows = x; rows < x_offset - this->x_margin; rows++ )
  {
    this->coords.setX ( rows );
    Gfx::drawRect ( video_buffer, this->coords, GColor ( currentR, currentG, currentB ) );

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
