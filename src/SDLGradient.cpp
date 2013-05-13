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
}

SDLGradient::~SDLGradient ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "SDLGradient::~SDLGradient (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}
