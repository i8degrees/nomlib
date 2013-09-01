/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/gui/Cursor.hpp"

namespace nom {
  namespace ui {

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

bool Cursor::load ( std::string filename, Color colorkey, bool use_cache ) // sheet_id ?
{
  if ( this->cursor.load ( filename, colorkey, use_cache ) == false )
  {
NOM_LOG_ERR ( "Could not load cursor resource file: " + filename );
    return false;
  }

  return true;
}

void Cursor::Update ( void )
{
  this->cursor.Update();
}

void Cursor::Draw ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}

bool Cursor::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( this->cursor.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( "Failed to resize the video surface." );
    return false;
  }

  //this->cursor.Update();

  return true;
}


  } // namespace ui
} // namespace nom
