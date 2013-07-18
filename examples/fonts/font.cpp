/******************************************************************************

    Unified Fonts Demo with nomlib

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "nomlib/graphics.hpp"
#include "nomlib/math.hpp"
#include "nomlib/system.hpp"

#define dump_var(var) NOMLIB_DUMP_VAR(var)

int main ( int argc, char* argv[] )
{
  nom::OSXFS dir;
  nom::SDL_Display context;
  nom::Text bitmap_font;
  nom::Text truetype_font;
  nom::Text not_font;

  std::string path = dir.getDirName(argv[0]) + "/";

  context.createWindow ( 640, 480, 32, 0 ); // segmentation fault otherwise

  if ( ! bitmap_font.Load ( path + "resources/VIII.png" ) )
    return EXIT_FAILURE;

  if ( ! truetype_font.Load ( path + "resources/EnvyCodeRb.ttf", 36 ) )
    return EXIT_FAILURE;

  not_font.Load ( path + "resources/cards.json", 0 );
NOMLIB_ASSERT ( not_font.getFontType() != 0 ); // should always err

NOMLIB_ASSERT ( bitmap_font.getFontType() == 1 );
NOMLIB_ASSERT ( truetype_font.getFontType() == 2 );

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

dump_var ( bitmap_font.getText() );
dump_var ( bitmap_font.getFontWidth() );
dump_var ( bitmap_font.getFontHeight() );
dump_var ( bitmap_font.getPosition().x );
dump_var ( bitmap_font.getPosition().y );
dump_var ( bitmap_font.getColor().red );
dump_var ( bitmap_font.getColor().green );
dump_var ( bitmap_font.getColor().blue );
dump_var ( bitmap_font.getColor().alpha );

dump_var ( truetype_font.getText() );
dump_var ( truetype_font.getFontWidth() );
dump_var ( truetype_font.getFontHeight() );
dump_var ( truetype_font.getPosition().x );
dump_var ( truetype_font.getPosition().y );
dump_var ( truetype_font.getColor().red );
dump_var ( truetype_font.getColor().green );
dump_var ( truetype_font.getColor().blue );
dump_var ( truetype_font.getColor().alpha );

  return EXIT_SUCCESS;
}
