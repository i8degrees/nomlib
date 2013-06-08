/******************************************************************************
    SDL_Cursor.h

  Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMNOM_SDL_CURSOR_HEADERS
#define NOMNOM_SDL_CURSOR_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "sprite.h"

#include "gamelib.h"

#include "Coords.h"

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

      unsigned int getX ( void );
      unsigned int getY ( void );
      void setX ( unsigned int x );
      void setY ( unsigned int y );
      void setXY ( unsigned int x = 0, unsigned int y = 0 );
      void updateXY ( unsigned int x = 0, unsigned int y = 0 );

      signed int getSheetID ( void );
      void setSheetID ( signed int sheet_id = -1 );

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

#endif // NOMNOM_SDL_CURSOR_HEADERS defined
