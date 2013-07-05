/******************************************************************************

    Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_CURSOR_HEADERS
#define NOMLIB_SDL_CURSOR_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "math/Coords.hpp"
#include "gfx/Sprite.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  class SDL_Cursor
  {
    public:
      SDL_Cursor ( void );
      ~SDL_Cursor ( void );

      SDL_Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height );
      //void Init ( void );
      void Load ( std::string filename, nom::Color colorkey );

      nom::int32 getX ( void );
      nom::int32 getY ( void );
      void setPosition ( unsigned int x = 0, unsigned int y = 0 );
      void move ( unsigned int x = 0, unsigned int y = 0 );

      signed int getSheetID ( void );
      void setSheetID ( signed int sheet_id = -1 );
      void setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

      unsigned int getState ( void );
      void setState ( unsigned int state );

      //bool isCursorLocked ( void );
      //void lockCursor ( bool toggle );

      nom::Coords getCursorPos ( unsigned int x, unsigned int y );
      void moveCursorUp();
      void moveCursorDown();
      void moveCursorLeft();
      void moveCursorRight();

      void Update ( void );
      void Draw ( void* video_buffer );

    private:
      nom::Sprite cursor; // interface cursor

      // cursor XY coords mapping
      std::vector<nom::Coords> coords_map;

      //    coords_map schema:
      //
      //  [index-pos].[ x, y, width, height ]
      //
  };
}

#endif // NOMLIB_SDL_CURSOR_HEADERS defined
