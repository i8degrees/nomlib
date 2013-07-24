/******************************************************************************

    Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gui/Cursor.hpp"

namespace nom {

Cursor::Cursor ( void )
{
NOM_LOG_CLASSINFO;

  // Not yet implemented
}

Cursor::Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
NOM_LOG_CLASSINFO;

  this->cursor = Sprite ( width, height );
  this->cursor.setPosition ( x, y );
  this->cursor.setState ( 0 ); // default state init
}

Cursor::~Cursor ( void )
{
NOM_LOG_CLASSINFO;

  // Clean up
}

int32 Cursor::getX ( void )
{
  Coords x_value = this->cursor.getPosition();
  return x_value.x;
}

int32 Cursor::getY ( void )
{
  Coords y_value = this->cursor.getPosition();
  return y_value.y;
}

void Cursor::setPosition ( unsigned int x, unsigned int y )
{
  this->cursor.setPosition ( x, y );
}

void Cursor::move ( unsigned int x, unsigned int y )
{
  this->cursor.move ( x, y );
}

signed int Cursor::getSheetID ( void )
{
  return this->cursor.getSheetID();
}

void Cursor::setSheetID ( signed int sheet_id )
{
  this->cursor.setSheetID ( sheet_id );
}

void Cursor::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->cursor.setSheetDimensions ( sheet_width, sheet_height, spacing, padding );
}

unsigned int Cursor::getState ( void )
{
  return this->cursor.getState();
}

void Cursor::setState ( unsigned int state )
{
  this->cursor.setState ( state );
}

void Cursor::Load ( std::string filename, Color colorkey, bool use_cache ) // sheet_id ?
{
  this->cursor.Load ( filename, colorkey, use_cache );
}

void Cursor::Update ( void )
{
  this->cursor.Update();
}

void Cursor::Draw ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}


} // namespace nom
