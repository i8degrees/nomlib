/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

/// \brief nom::SpriteSheet usage example (mostly a stub for now!)

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

const std::string APP_NAME = "nom::SpriteSheet";

const nom::Path p;
const std::string APP_RESOURCES_DIR = "Resources";
const std::string RESOURCE_SPRITE_SHEET = APP_RESOURCES_DIR + p.native() + "cursors.json";

// const std::string OUTPUT_SHEET_FILENAME = "faces.json";
const std::string OUTPUT_SHEET_FILENAME = "cursors.json";

int main ( int argc, char* argv[] )
{
  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }
  atexit(nom::quit);

  // Existing sprite sheet
  // nom::SpriteSheet cursor ( RESOURCE_SPRITE_SHEET );
  // cursors.dump();

  // Creating a new sprite sheet; "faces.json"
  // nom::SpriteSheet faces  (
  //                           "faces.png",  // meta-data info
  //                           2082, 262,    // overall sheet size in pixels
  //                           64, 64,       // sprite width & height in pixels
  //                           1, 1,         // spacing and padding in pixels
  //                           111           // total number of sprite frames,
  //                         );              // always counting from zero

  // Note that you should always be able to reconstruct the parameters specified
  // above with the resulting output file data, using the header info at the
  // bottom
  // if ( faces.save ( OUTPUT_SHEET_FILENAME ) == false )
  // {
  //   nom::DialogMessageBox ( APP_NAME, "Could not save sprite sheet file: " + OUTPUT_SHEET_FILENAME );
  //   return NOM_EXIT_FAILURE;
  // }

  // Creating a new sprite sheet; "cursors.json"
  nom::SpriteSheet cursors  (
                              "cursors.png",  // meta-data info
                              130, 16,        // overall sheet size in pixels
                              26, 16,         // sprite width & height in pixels
                              0, 0,           // spacing and padding in pixels
                              5               // total number of sprite frames,
                            );                // always counting from zero

  if ( cursors.save ( OUTPUT_SHEET_FILENAME ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not save sprite sheet file: " + OUTPUT_SHEET_FILENAME );
    return NOM_EXIT_FAILURE;
  }

  if ( cursors.frames() < 1 )
  {
    nom::DialogMessageBox ( APP_NAME, "Error: Sprite sheet " + OUTPUT_SHEET_FILENAME + " appears to be empty!" );
    return NOM_EXIT_FAILURE;
  }

  nom::DialogMessageBox ( APP_NAME, "Sprite sheet saved at: " + OUTPUT_SHEET_FILENAME );

  return NOM_EXIT_SUCCESS;
}
