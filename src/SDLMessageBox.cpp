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

  window_borders.clear();

  for ( int i = 0; i < window_borders.size(); i++ )
    window_borders[i].setColor ( 0, 0, 0 );

  this->geometry.setCoords ( 0, 0, 0, 0 );
}


SDLMessageBox::~SDLMessageBox ( void )
{
  #ifdef DEBUG_MESSAGEBOX_OBJ
    std::cout << "SDLMessageBox::~SDLMessageBox (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->enabled = false;
}

void SDLMessageBox::Init ( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
  // init geometry coords w/ arguments list
  this->geometry.setCoords ( x, y, width, height );
}

bool SDLMessageBox::isEnabled ( void )
{
  if ( this->enabled == true )
    return true;
  else
    return false;
}

void SDLMessageBox::disable ( bool state )
{
 this->enabled = false;
}

void SDLMessageBox::enable ( bool state )
{
  this->enabled = true;
}

}

void SDLMessageBox::Update ( void )
{
  //
}

void SDLMessageBox::Draw ( void )
{
  //
}
