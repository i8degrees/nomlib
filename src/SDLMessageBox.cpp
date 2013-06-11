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

void SDLMessageBox::disable ( void )
{
 this->enabled = false;
}

void SDLMessageBox::enable ( void )
{
  this->enabled = true;
}

void SDLMessageBox::setBorder ( nom::Color &border_colors )
{
  this->window_borders.push_back ( border_colors );

  #ifdef DEBUG_MESSAGEBOX
    std::cout << "window_borders: " << this->window_borders.getRed() << "\n\n";
    std::cout << "window_borders: " << this->window_borders.getGreen() << "\n\n";
    std::cout << "window_borders: " << this->window_borders.getBlue() << "\n\n";
  #endif
}

void SDLMessageBox::setBackground ( nom::SDL_Gradient *gradient )
{
  this->background = gradient;
}

void SDLMessageBox::Update ( void )
{
  //
}

void SDLMessageBox::Draw ( void* video_buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
  unsigned int padding = 1;
  unsigned int x_offset = x + width; //unsigned int x_offset = ( x + width ) - padding;
  unsigned int y_offset = y + height; //unsigned int y_offset = ( y + height ) - padding;

  if ( this->background != NULL )
  {
    nom::Color starting ( 66, 66, 66 );
    nom::Color ending ( 99, 99, 99 );
    background->Init ( starting, ending, x, y, width, height, 0, 0, 0 );
    background->Draw ( video_buffer, x, y, width, height, 0 );
  }

  if ( box.mustLock ( video_buffer ) == true )
  {
    box.lockCanvas ( video_buffer );

    line.setPosition ( x, y, x_offset - padding, y ); // top0
    line.setColor ( this->window_borders[0].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x, y + 1, x_offset - padding, y + 1 ); // top1
    line.setColor ( this->window_borders[1].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x, y + 1, x, y_offset - padding ); // left0
    line.setColor ( this->window_borders[2].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x + 1, y + 2, x + 1, y_offset - padding ); // left1
    line.setColor ( this->window_borders[3].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x, y_offset - padding, x_offset - padding, y_offset - padding ); //bottom0
    line.setColor ( this->window_borders[4].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x, y_offset, x_offset + padding, y_offset ); // bottom1
    line.setColor ( this->window_borders[5].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x_offset - padding, y, x_offset - padding, y_offset + padding ); // right0
    line.setColor ( this->window_borders[6].getColor() );
    line.Draw ( video_buffer );

    line.setPosition ( x_offset, y, x_offset, y_offset + padding ); // right1
    line.setColor ( this->window_borders[7].getColor() );
    line.Draw ( video_buffer );
  }

  box.unlockCanvas ( video_buffer );
}
