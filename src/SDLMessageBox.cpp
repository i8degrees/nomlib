/******************************************************************************
    SDLMessageBox.cpp

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDLMessageBox.h"

SDLMessageBox::SDLMessageBox ( void )
{
  #ifdef DEBUG_MESSAGEBOX_OBJ
    std::cout << "SDLMessageBox::SDLMessageBox (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->enabled = true;
}


SDLMessageBox::~SDLMessageBox ( void )
{
  #ifdef DEBUG_MESSAGEBOX_OBJ
    std::cout << "SDLMessageBox::~SDLMessageBox (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->enabled = false;
}

SDLMessageBox::SDLMessageBox ( GCoords coords, GColor window_top[], GColor window_bottom[], GColor window_sides[] )
{
  //
}

void SDLMessageBox::Update ( void )
{
  //
}

void SDLMessageBox::Draw ( void )
{
  //
}
