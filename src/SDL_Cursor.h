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

#include "GCoords.h"
#include "sprite.h"

#include "gamelib.h"

namespace NomNom
{
  class SDL_Cursor
  {
    public:
      SDL_Cursor ( void );
      ~SDL_Cursor ( void );

      SDL_Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height );
      //void Init ( void );
      void Load ( std::string filename, GColor colorkey );

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

      GCoords getCursorPos ( unsigned int x, unsigned int y );
      void moveCursorUp();
      void moveCursorDown();
      void moveCursorLeft();
      void moveCursorRight();

      void Update ( void );
      void Draw ( Gfx *engine );
      //bool Draw ( SDL_Surface *video_buffer );

    private:
      nom::Sprite cursor; // interface cursor

      // cursor XY coords mapping
      std::vector<GCoords> coords_map;

      //    coords_map schema:
      //
      //  [index-pos].[ x, y, width, height ]
      //
  };
}

namespace nom = NomNom; // alias declaration

#endif // NOMNOM_SDL_CURSOR_HEADERS defined
