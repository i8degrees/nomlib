/******************************************************************************
    SDLWindowFrame.cpp

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDLWindowFrame.h"

SDLWindowFrame::SDLWindowFrame ( void )
{
  #ifdef DEBUG_WINDOWFRAME_OBJ
    std::cout << "SDLWindowFrame::SDLWindowFrame (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}


SDLWindowFrame::~SDLWindowFrame ( void )
{
  #ifdef DEBUG_WINDOWFRAME_OBJ
    std::cout << "SDLWindowFrame::~SDLWindowFrame (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}
