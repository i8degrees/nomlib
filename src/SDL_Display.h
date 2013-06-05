/******************************************************************************
    SDL_Display.h

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_DISPLAY_HEADERS
#define NOMLIB_SDL_DISPLAY_HEADERS

#include <iostream>
#include <string>

//#include "Color.h"
//#include "Coords.h"

#include "gamelib.h"

#include "gfx.h"
#include "IDisplay.h"

namespace nom
{
  class SDL_Display: public nom::IDisplay
  {
    public:
      SDL_Display ( unsigned int sdl_flags = SDL_INIT_VIDEO );

      ~SDL_Display ( void );

      bool CreateWindow  (  signed int display_width, signed int display_height,
                            signed int display_colorbit = 32, unsigned int flags = 0
                          );

      static SDL_Surface* getDisplay ( void )
      {
        return SDL_GetVideoSurface();
      }

      const signed int getDisplayWidth ( void ) const
      {
        return SDL_GetVideoSurface()->w;
      }

      const signed int getDisplayHeight ( void ) const
      {
        return SDL_GetVideoSurface()->h;
      }

      const signed int getDisplayColorBits ( void ) const
      {
        return SDL_GetVideoSurface()->format->BitsPerPixel;
      }

      const unsigned int getDisplayFlags ( void ) const
      {
        return SDL_GetVideoSurface()->flags;
      }

      const unsigned short getDisplayPitch ( void ) const
      {
        return SDL_GetVideoSurface()->pitch;
      }

      static const bool updateDisplay ( void );

      static void setTitle ( const std::string& app_name = "\0" )
      {
        SDL_WM_SetCaption ( app_name.c_str(), NULL );
      }

      static bool setIcon ( const std::string& app_icon = "\0" );

    private:
      // ...
  };
}

#endif // NOMLIB_SDL_DISPLAY_HEADERS defined
