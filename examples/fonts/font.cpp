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

int main ( int argc, char* argv[] )
{
  nom::OSXFS dir;
  nom::Display context;
  nom::Text bitmap_font;
  nom::Text truetype_font;
  nom::Text not_font;

  std::string path = dir.getDirName(argv[0]) + "/";

  context.createWindow ( 640, 480, 32, 0 ); // segmentation fault otherwise

  if ( ! bitmap_font.Load ( path + "Resources/VIII.png" ) )
    return EXIT_FAILURE;

  if ( ! truetype_font.Load ( path + "Resources/EnvyCodeRb.ttf", 36 ) )
    return EXIT_FAILURE;

  not_font.Load ( path + "Resources/cards.json", 0 );
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
