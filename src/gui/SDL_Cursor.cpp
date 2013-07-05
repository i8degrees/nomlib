/******************************************************************************

    Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Cursor.hpp"

nom::SDL_Cursor::SDL_Cursor ( void )
{
NOMLIB_LOG_INFO;

  // Not yet implemented
}

nom::SDL_Cursor::SDL_Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
NOMLIB_LOG_INFO;

  this->cursor = nom::Sprite ( width, height );
  this->cursor.setPosition ( x, y );
  this->cursor.setState ( 0 ); // default state init
}

nom::SDL_Cursor::~SDL_Cursor ( void )
{
NOMLIB_LOG_INFO;

  // Clean up
}

void nom::SDL_Cursor::Load ( std::string filename, nom::Color colorkey ) // sheet_id ?
{
  this->cursor.Load ( filename, colorkey );
}

nom::int32 nom::SDL_Cursor::getX ( void )
{
  nom::Coords x_value = this->cursor.getPosition();
  return x_value.x;
}

nom::int32 nom::SDL_Cursor::getY ( void )
{
  nom::Coords y_value = this->cursor.getPosition();
  return y_value.y;
}

void nom::SDL_Cursor::setPosition ( unsigned int x, unsigned int y )
{
  this->cursor.setPosition ( x, y );
}

void nom::SDL_Cursor::move ( unsigned int x, unsigned int y )
{
  this->cursor.move ( x, y );
}

signed int nom::SDL_Cursor::getSheetID ( void )
{
  return this->cursor.getSheetID();
}

void nom::SDL_Cursor::setSheetID ( signed int sheet_id )
{
  this->cursor.setSheetID ( sheet_id );
}

void nom::SDL_Cursor::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->cursor.setSheetDimensions ( sheet_width, sheet_height, spacing, padding );
}

unsigned int nom::SDL_Cursor::getState ( void )
{
  return this->cursor.getState();
}

void nom::SDL_Cursor::setState ( unsigned int state )
{
  this->cursor.setState ( state );
}

void nom::SDL_Cursor::Update ( void )
{
  this->cursor.Update();
}

void nom::SDL_Cursor::Draw ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}
