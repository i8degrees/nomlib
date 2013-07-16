/******************************************************************************

    Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gui/SDL_Cursor.hpp"

namespace nom {

SDL_Cursor::SDL_Cursor ( void )
{
NOMLIB_LOG_INFO;

  // Not yet implemented
}

SDL_Cursor::SDL_Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
NOMLIB_LOG_INFO;

  this->cursor = Sprite ( width, height );
  this->cursor.setPosition ( x, y );
  this->cursor.setState ( 0 ); // default state init
}

SDL_Cursor::~SDL_Cursor ( void )
{
NOMLIB_LOG_INFO;

  // Clean up
}

void SDL_Cursor::Load ( std::string filename, Color colorkey, bool use_cache ) // sheet_id ?
{
  this->cursor.Load ( filename, colorkey, use_cache );
}

int32 SDL_Cursor::getX ( void )
{
  Coords x_value = this->cursor.getPosition();
  return x_value.x;
}

int32 SDL_Cursor::getY ( void )
{
  Coords y_value = this->cursor.getPosition();
  return y_value.y;
}

void SDL_Cursor::setPosition ( unsigned int x, unsigned int y )
{
  this->cursor.setPosition ( x, y );
}

void SDL_Cursor::move ( unsigned int x, unsigned int y )
{
  this->cursor.move ( x, y );
}

signed int SDL_Cursor::getSheetID ( void )
{
  return this->cursor.getSheetID();
}

void SDL_Cursor::setSheetID ( signed int sheet_id )
{
  this->cursor.setSheetID ( sheet_id );
}

void SDL_Cursor::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->cursor.setSheetDimensions ( sheet_width, sheet_height, spacing, padding );
}

unsigned int SDL_Cursor::getState ( void )
{
  return this->cursor.getState();
}

void SDL_Cursor::setState ( unsigned int state )
{
  this->cursor.setState ( state );
}

void SDL_Cursor::Update ( void )
{
  this->cursor.Update();
}

void SDL_Cursor::Draw ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}


} // namespace nom
