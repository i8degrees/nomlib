/******************************************************************************

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SPRITE_HEADERS
#define NOMLIB_SPRITE_HEADERS

#include <iostream>
#include <string>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "gfx/SDL_Canvas.hpp"
#include "math/Transformable.hpp"

namespace nom {

class Sprite: public SDL_Drawable,  //  "is a" inheritance
              public Transformable  //  "has a" inheritance
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

    bool Load ( std::string filename, Color colorkey, bool use_cache = 0,
                unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL
              );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    SDL_Canvas sprite_buffer;
    Coords offsets;

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


} // namespace nom

#endif // NOMLIB_SPRITE_HEADERS defined
