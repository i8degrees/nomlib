/******************************************************************************
    SDL_Display.h

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_DISPLAY_HEADERS
#define NOMLIB_SDL_DISPLAY_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "IDisplay.h"
#include "Coords.h"
//#include "Color.h"
#include "gfx.h" // setWindowIcon dependency
#include "gamelib.h"

namespace nom
{
  class SDL_Display: public IDisplay
  {
    public:
      SDL_Display ( void );
      ~SDL_Display ( void );

      void createWindow ( int32_t display_width, int32_t display_height,
                          int32_t display_colorbit, uint32_t flags = 0
                        );

      void* get ( void ) const;
      int32_t getDisplayWidth ( void ) const;
      int32_t getDisplayHeight ( void ) const;
      int32_t getDisplayColorBits ( void ) const;
      uint32_t getDisplayFlags ( void ) const;
      u_short getDisplayPitch ( void ) const;
      void* getDisplayPixels ( void ) const;
      void* getDisplayPixelsFormat ( void ) const;
      const nom::Coords getDisplayClip ( void ) const;

      static void Update ( void* video_buffer );
      void toggleFullScreenWindow ( int32_t width, int32_t height ) const;

      const std::string getWindowTitle ( void ) const;
      void* getWindowIcon ( void ) const;

      void setWindowTitle ( const std::string& app_name = "\0" );
      void setWindowIcon ( const std::string& app_icon = "\0" );

    private:
      // ...
  };
}

#endif // NOMLIB_SDL_DISPLAY_HEADERS defined
