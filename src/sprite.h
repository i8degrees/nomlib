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

#include "gfx.h"
#include "GColor.h"

class Gfx;

namespace NomNom
{
  class Sprite
  {
    public:
      Sprite ( void );
      Sprite ( unsigned int width, unsigned int height );
      ~Sprite ( void );

      unsigned int getX ( void );
      unsigned int getY ( void );
      unsigned int getWidth ( void );
      unsigned int getHeight ( void );

      unsigned int getXOffset ( void );
      unsigned int getYOffset ( void );
      unsigned int getWidthOffset ( void );
      unsigned int getHeightOffset ( void );

      void setX ( unsigned int x );
      void setY ( unsigned int y );
      void setXY ( unsigned int x = 0, unsigned int y = 0 );
      void updateXY ( unsigned int x = 0, unsigned int y = 0);
      void setWidth ( unsigned int width );
      void setHeight ( unsigned int height );
      void setXOffset ( unsigned int x_offset );
      void setYOffset ( unsigned int y_offset );
      void setWidthOffset ( unsigned int width_offset );
      void setHeightOffset ( unsigned int height_offset );
      unsigned int getState ( void );
      void setState ( unsigned int state );

      signed int getSheetID ( void );
      void setSheetID ( signed int id );
      //struct sheet getSheetDimensions ( void );
      void setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

      bool Load ( std::string filename );
      bool Load ( std::string filename, GColor colorkey, unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL );

      bool Draw ( Gfx *engine );

    private:
      SDL_Surface *sprite_buffer; // memory buffer allocation

      struct {
        unsigned int x; // sprite x-axis coord for blitting onto video surface
        unsigned int y; // sprite y-axis coord for blitting onto video surface
        unsigned int width; // sprite width
        unsigned int height; // sprite height
      } coords;

      /* These are not used at the moment and are reserved for future implementation */
      struct {
        unsigned int x; // clipping offset X coord
        unsigned int y; // clipping offset Y coord
        unsigned int width; // clipping offset width
        unsigned int height; // clipping offset height
      } offsets;

      unsigned int state; // alive, dying, dead, ...

      struct {
        signed int id; // maps a specific sprite within sheet
        unsigned int sprite_width; // width of sprite in sheet
        unsigned int sprite_height; // height of sprite in sheet
        unsigned int width; // width of sprite sheet
        unsigned int height; // height of sprite sheet
        unsigned int spacing; // applied between each sheet tile
        unsigned int padding; // applied on all four sides of sheet tile
      } sheet;
  };
}

namespace nom = NomNom; // alias declaration

#endif // GAMELIB_SPRITE_HEADERS defined
