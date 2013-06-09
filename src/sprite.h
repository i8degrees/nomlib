/******************************************************************************
    sprite.h

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SPRITE_HEADERS
#define GAMELIB_SPRITE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

#include "Color.h"

#include "gfx.h"

#include "SDL_Drawable.hpp"

#include "SDL_Canvas.hpp"

namespace nom
{
  class Sprite: public nom::SDL_Drawable,  //  "is a" inheritance
                public nom::Transformable  //  "has a" inheritance
  {
    public:
      Sprite ( void );
      Sprite ( unsigned int width, unsigned int height );
      virtual ~Sprite ( void );

      unsigned int getState ( void );
      void setState ( unsigned int state );

      signed int getSheetID ( void );
      void setSheetID ( signed int id );
      void setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

      bool Load ( std::string filename );
      bool Load ( std::string filename, nom::Color colorkey, unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL );

      void Draw ( void* video_buffer );

    private:
      nom::SDL_Canvas sprite_buffer;

      unsigned int state; /// alive, dying, dead, ...

      struct {
        signed int id; /// maps a specific sprite within sheet
        unsigned int sprite_width; /// width of sprite in sheet
        unsigned int sprite_height; /// height of sprite in sheet
        unsigned int width; /// width of sprite sheet
        unsigned int height; /// height of sprite sheet
        unsigned int spacing; /// applied between each sheet tile
        unsigned int padding; /// applied on all four sides of sheet tile
      } sheet;
  };
}

#endif // GAMELIB_SPRITE_HEADERS defined
