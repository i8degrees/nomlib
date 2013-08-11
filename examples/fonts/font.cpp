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

// Text class usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/graphics.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>

int main ( int argc, char* argv[] )
{
  nom::File dir;
  nom::Display context;
  nom::Text bitmap_font;
  nom::Text truetype_font;
  nom::Text not_font;

  std::string path = dir.path(argv[0]) + "/";

  context.createWindow ( 640, 480, 32, 0 ); // segmentation fault otherwise

  if ( ! bitmap_font.load ( path + "Resources/VIII.png" ) )
    return EXIT_FAILURE;

  if ( ! truetype_font.load ( path + "Resources/EnvyCodeRb.ttf", 36 ) )
    return EXIT_FAILURE;

  not_font.load ( path + "Resources/cards.json", 0 );
NOM_ASSERT ( not_font.getFontType() != 0 ); // should always err
NOM_ASSERT ( bitmap_font.getFontType() == 1 );
NOM_ASSERT ( truetype_font.getFontType() == 2 );

  bitmap_font.setColor ( nom::Color ( 255, 255, 255 ) ); // has no effect
  truetype_font.setColor ( nom::Color ( 255, 255, 255 ) );

  bitmap_font.setText ( "Hello, world!" );
  truetype_font.setText ( "Hello, boo!" );
  bitmap_font.setPosition ( nom::Coords ( 640/2, 480/2 ) );
  truetype_font.setPosition ( nom::Coords ( 640/2, 160 ) );

  bitmap_font.Update();
  truetype_font.Update();

  bitmap_font.Draw ( context.get() );
  truetype_font.Draw ( context.get() );

  context.Update();

  nom::sleep ( 1500 );

  bitmap_font.setText ( "Goodbye cruel world!" );
  truetype_font.setText ( "Goodbye cruel world!" );
  bitmap_font.setPosition ( nom::Coords ( 640/4, 480/4 ) );
  truetype_font.setPosition ( nom::Coords ( 640/4, 80 ) );

  bitmap_font.setFontStyle ( nom::FontStyle::Faded, 125 );
  truetype_font.setFontStyle ( nom::FontStyle::Faded, 125 );

  bitmap_font.Update();
  truetype_font.Update();

  bitmap_font.Draw ( context.get() );
  truetype_font.Draw ( context.get() );

  context.Update();

  nom::sleep ( 2500 );

NOM_DUMP_VAR ( bitmap_font.getText() );
NOM_DUMP_VAR ( bitmap_font.getFontWidth() );
NOM_DUMP_VAR ( bitmap_font.getFontHeight() );
NOM_DUMP_VAR ( bitmap_font.getPosition().x );
NOM_DUMP_VAR ( bitmap_font.getPosition().y );
NOM_DUMP_VAR ( bitmap_font.getColor().red );
NOM_DUMP_VAR ( bitmap_font.getColor().green );
NOM_DUMP_VAR ( bitmap_font.getColor().blue );
NOM_DUMP_VAR ( bitmap_font.getColor().alpha );

NOM_DUMP_VAR ( truetype_font.getText() );
NOM_DUMP_VAR ( truetype_font.getFontWidth() );
NOM_DUMP_VAR ( truetype_font.getFontHeight() );
NOM_DUMP_VAR ( truetype_font.getPosition().x );
NOM_DUMP_VAR ( truetype_font.getPosition().y );
NOM_DUMP_VAR ( truetype_font.getColor().red );
NOM_DUMP_VAR ( truetype_font.getColor().green );
NOM_DUMP_VAR ( truetype_font.getColor().blue );
NOM_DUMP_VAR ( truetype_font.getColor().alpha );

  return EXIT_SUCCESS;
}
