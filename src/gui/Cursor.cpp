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
NOM_LOG_TRACE ( NOM );

  initialize ( 0, 0, 0, 0 );
}

Cursor::Cursor ( int32 x, int32 y, int32 width, int32 height )
{
NOM_LOG_TRACE ( NOM );

  this->initialize ( x, y, width, height );
}

Cursor::~Cursor ( void )
{
NOM_LOG_TRACE ( NOM );
}

int32 Cursor::getX ( void ) const
{
  Coords x_value = this->cursor.getPosition();
  return x_value.x;
}

int32 Cursor::getY ( void ) const
{
  Coords y_value = this->cursor.getPosition();
  return y_value.y;
}

void Cursor::setSize ( int32 width, int32 height )
{
  this->cursor.setSize ( width, height );
}

void Cursor::setPosition ( int32 x, int32 y )
{
  this->cursor.setPosition ( x, y );
}

void Cursor::move ( int32 x, int32 y )
{
  this->cursor.move ( x, y );
}

int32 Cursor::getSheetID ( void )
{
  return this->cursor.getSheetID();
}

void Cursor::setSheetID ( int32 sheet_id )
{
  this->cursor.setSheetID ( sheet_id );
}

void Cursor::setSheetDimensions ( int32 sheet_width, int32 sheet_height, int32 spacing, int32 padding )
{
  this->cursor.setSheetDimensions ( sheet_width, sheet_height, spacing, padding );
}

int32 Cursor::getState ( void )
{
  return this->cursor.getState();
}

void Cursor::setState ( int32 state )
{
  this->cursor.setState ( state );
}

int32 Cursor::moveCursorUp ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::moveCursorDown ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::moveCursorLeft ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::moveCursorRight ( void )
{
  return -1; // Do nothing virtual implementation
}

bool Cursor::load ( const std::string& filename, const Color& colorkey, bool use_cache )
{
  if ( this->cursor.load ( filename, colorkey, use_cache ) == false )
  {
NOM_LOG_ERR ( NOM, "Could not load cursor resource file: " + filename );
    return false;
  }

  return true;
}

void Cursor::update ( void )
{
  this->cursor.Update();
}

void Cursor::draw ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}

bool Cursor::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( this->cursor.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }

  //this->cursor.Update();

  return true;
}

void Cursor::initialize ( int32 x, int32 y, int32 width, int32 height )
{
  this->cursor = Sprite ( width, height );
  this->cursor.setPosition ( x, y );
  this->cursor.setState ( 0 );
}


  } // namespace ui
} // namespace nom
