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

  this->window_borders.clear();

  for ( int32_t i = 0; i < this->window_borders.size(); i++ )
    this->window_borders[i].setColor ( nom::Color::Black );

  this->coords.setCoords ( 0, 0, 0, 0 );
}


SDLMessageBox::~SDLMessageBox ( void )
{
  drawable_t::const_iterator it; // iterator for our drawables vector

  #ifdef DEBUG_MESSAGEBOX_OBJ
    std::cout << "SDLMessageBox::~SDLMessageBox (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->enabled = false;

  // Goodbye cruel drawables!
  for ( it = this->lines.begin(); it != this->lines.end(); it++ )
  {
    nom::SDL_Drawable* obj = *it;
    delete obj;
  }

  this->lines.clear();

  this->background = NULL; // SDL_Gradient
}

// setBorder must be called first for the border colors to be set properly
void SDLMessageBox::Init ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  unsigned int padding = 1;
  unsigned int x_offset = x + width; //unsigned int x_offset = ( x + width ) - padding;
  unsigned int y_offset = y + height; //unsigned int y_offset = ( y + height ) - padding;

  // init geometry coords w/ arguments list
  this->coords.setCoords ( x, y, width, height );

  if ( this->background != NULL )
  {
    nom::Color starting ( 66, 66, 66 );
    nom::Color ending ( 99, 99, 99 );
    this->background->Init ( starting, ending, x, y, width, height, 0, 0, 0 );
  }

  this->lines.push_back ( new nom::Line ( x, y, x_offset - padding, y, this->window_borders[0].getColor() ) ); // top0
  this->lines.push_back ( new nom::Line ( x, y + 1, x_offset - padding, y + 1, this->window_borders[1].getColor() ) ); // top1
  this->lines.push_back ( new nom::Line ( x, y + 1, x, y_offset - padding, this->window_borders[2].getColor() ) ); // left0
  this->lines.push_back ( new nom::Line ( x + 1, y + 2, x + 1, y_offset - padding, this->window_borders[3].getColor() ) ); // left1
  this->lines.push_back ( new nom::Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, this->window_borders[4].getColor() ) ); //bottom0
  this->lines.push_back ( new nom::Line ( x, y_offset, x_offset + padding, y_offset, this->window_borders[5].getColor() ) ); // bottom1
  this->lines.push_back ( new nom::Line ( x_offset - padding, y, x_offset - padding, y_offset + padding, this->window_borders[6].getColor() ) ); // right0
  this->lines.push_back ( new nom::Line ( x_offset, y, x_offset, y_offset + padding, this->window_borders[7].getColor() ) ); // right1
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

void SDLMessageBox::setBorder ( const nom::Color& border_colors )
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
  drawable_t::const_iterator it; // iterator for our drawables vector

  for ( it = this->lines.begin(); it != this->lines.end(); it++ )
  {
    nom::SDL_Drawable* obj = *it;
    obj->Update();
  }
}

void SDLMessageBox::Draw ( void* video_buffer )
{
  drawable_t::const_iterator it; // iterator for our drawables vector

  if ( this->background != NULL )
  {
    background->Draw ( video_buffer, this->coords.getX(), this->coords.getY(), this->coords.getWidth(), this->coords.getHeight(), 0 );
  }

  if ( this->box.mustLock ( video_buffer ) == true )
  {
    // Not sure if this is the proper placement of the check, but seems to do
    // alright for now!
    while ( this->box.lockCanvas ( video_buffer ) == false )
      SDL_Delay ( 10 );

    for ( it = this->lines.begin(); it != this->lines.end(); it++ )
    {
      nom::SDL_Drawable* obj = *it;
      obj->Draw ( video_buffer );
    }

    this->box.unlockCanvas ( video_buffer );
  }
}
