/******************************************************************************
    IDisplay.h

    Abstract Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_IDISPLAY_HEADERS
#define NOMLIB_IDISPLAY_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "SDL.h"

#include "Coords.h"
#include "gamelib.h"

namespace nom
{
  class IDisplay
  {
    public:
      IDisplay ( void )
      {
        // NULL
      }

      virtual ~IDisplay ( void )
      {
        // NULL
      }

      virtual void createWindow ( int32_t display_width, int32_t display_height,
                                  int32_t display_colorbit, uint32_t flags = 0
                                ) = 0;

      virtual const void* get ( void ) const = 0;
      virtual const int32_t getDisplayWidth ( void ) const = 0;
      virtual const int32_t getDisplayHeight ( void ) const = 0;
      virtual const int32_t getDisplayColorBits ( void ) const = 0;
      virtual const uint32_t getDisplayFlags ( void ) const = 0;
      virtual const u_short getDisplayPitch ( void ) const = 0;
      virtual const void* getDisplayPixels ( void ) const = 0;
      virtual const void* getDisplayPixelsFormat ( void ) const = 0;
      virtual const nom::Coords getDisplayClip ( void ) const = 0;

      virtual void Update ( void ) = 0;
      virtual const void toggleFullScreenWindow ( int32_t width, int32_t height ) const = 0;

      virtual const std::string getWindowTitle ( void ) const = 0;
      virtual void setWindowTitle ( const std::string& ) = 0;

      virtual void* getWindowIcon ( void ) const = 0;
      virtual void setWindowIcon ( const std::string& ) = 0;

      private:
        // ...
      protected:
        // ...
    };
}

#endif // NOMLIB_IDISPLAY_HEADERS defined
