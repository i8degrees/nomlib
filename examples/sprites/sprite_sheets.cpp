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

/// \brief nom::SpriteSheet usage example.

#include <iostream>
#include <string>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

const std::string APP_NAME = "nom::SpriteSheet";

const nom::Path p;
const std::string APP_RESOURCES_DIR = "Resources";
// const std::string RESOURCE_SPRITE_SHEET = APP_RESOURCES_DIR + p.native() + "cursors.json";

/// \brief Total number of sprite frames to be serialized in resulting output;
/// see faces.json. The value is also used in unit test validation.
const nom::sint TOTAL_NUMBER_SPRITES[2] = { 111,  // faces
                                            5     // cursors
                                          };

/// \brief Resulting output filename.
const std::string OUTPUT_SHEET_FILENAME[2] =  { "faces.json",
                                                "cursors.json"
                                              };

/// \brief Number of unit test values -- see above -- looped through.
const nom::uint TOTAL_SPRITE_SHEET_TESTS = 2;

nom::sint main ( nom::sint argc, char* argv[] )
{
  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::init( argc, argv ) == false )
  {
    nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize nomlib." );
    exit( NOM_EXIT_FAILURE );
  }
  atexit( nom::quit );

  // Existing sprite sheet
  // nom::SpriteSheet cursor( RESOURCE_SPRITE_SHEET );
  // cursors.dump();

  // Creating new sprite sheets
  nom::SpriteSheet sheets[2] =  { nom::SpriteSheet( // faces.json (0)
                                    "faces.png",  // meta-data info
                                    2082, 262,    // overall sheet size in pixels
                                    64, 64,       // sprite width & height in pixels
                                    1, 1,         // spacing and padding in pixels
                                    // Max number of frames (starting from zero)
                                    TOTAL_NUMBER_SPRITES[0]
                                  ),
                                  nom::SpriteSheet( // cursors.json (1)
                                    "cursors.png",  // meta-data info
                                    130, 16,        // overall sheet size in pixels
                                    26, 16,         // sprite width & height in pixels
                                    0, 0,           // spacing and padding in pixels
                                    // Max number of frames (starting from zero)
                                    TOTAL_NUMBER_SPRITES[1]
                                  )
                                };

  for( nom::uint idx = 0; idx < TOTAL_SPRITE_SHEET_TESTS; ++idx )
  {
    // Note that you should always be able to reconstruct the parameters specified
    // above with the resulting output file data, using the header info at the
    // bottom
    if ( sheets[idx].save( OUTPUT_SHEET_FILENAME[idx] ) == false )
    {
      nom::DialogMessageBox( APP_NAME, "ERROR: Could not save sprite sheet file: " + OUTPUT_SHEET_FILENAME[idx] );
      return NOM_EXIT_FAILURE;
    }

    // Err if sheets[idx] is empty, less than or greater than
    // TOTAL_NUMBER_SPRITES
    if( sheets[idx].empty() || sheets[idx].frames() < TOTAL_NUMBER_SPRITES[idx] || sheets[idx].frames() > TOTAL_NUMBER_SPRITES[idx] )
    {
      nom::DialogMessageBox( APP_NAME, "ERROR: Sprite sheet " + OUTPUT_SHEET_FILENAME[idx] + " is only " + std::to_string( sheets[idx].frames() ) + "/" + std::to_string( TOTAL_NUMBER_SPRITES[idx] ) + " frames!" );
      return NOM_EXIT_FAILURE;
    }
  } // end for loop of TOTAL_SPRITE_SHEET_TESTS

  for( nom::uint idx = 0; idx < TOTAL_SPRITE_SHEET_TESTS; ++idx )
  {
    if( sheets[idx].load( OUTPUT_SHEET_FILENAME[idx] ) == false )
    {
      nom::DialogMessageBox( APP_NAME, "ERROR: Could not load sprite sheet file: " + OUTPUT_SHEET_FILENAME[idx] );
      return NOM_EXIT_FAILURE;
    }

    if( sheets[idx].save( "testme.json" ) == false )
    {
      nom::DialogMessageBox( APP_NAME, "ERROR: Could not save sprite sheet file: " + OUTPUT_SHEET_FILENAME[idx] );
      return NOM_EXIT_FAILURE;
    }
  }

  // nom::DialogMessageBox ( APP_NAME, "SUCCESS: Sprite sheet saved at: " + OUTPUT_CURSORS_SHEET_FILENAME );

  return NOM_EXIT_SUCCESS;
}
