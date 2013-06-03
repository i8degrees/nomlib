/******************************************************************************
    IDisplay.h

    Abstract Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_IDISPLAY_HEADERS
#define NOMLIB_IDISPLAY_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

//#include "Color.h"
//#include "Coords.h"

#include "gamelib.h"

namespace nom
{
  class IDisplay
  {
    public:
      IDisplay ( unsigned int flags = 0 )
      {
        // ...
      }
      virtual ~IDisplay ( void )
      {
        // ...
      }

      virtual bool CreateWindow ( unsigned int display_width = 0,
                                  unsigned int display_height = 0,
                                  unsigned int display_colorbit = 0,
                                  unsigned int flags = 0
                                ) = 0;

        virtual SDL_Surface* getDisplay ( void ) = 0;
        virtual bool updateDisplay ( void ) = 0;

        virtual void setTitle ( const std::string& app_name = "\0" ) = 0;

      protected:
        // ...
    };
}

#endif // NOMLIB_IDISPLAY_HEADERS defined
