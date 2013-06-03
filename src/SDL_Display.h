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
  namespace SDL
  {
    class Display: public nom::IDisplay
    {
      public:
        Display ( unsigned int flags = 0 );
        ~Display ( void );

        bool CreateWindow ( unsigned int display_width = 0,
                            unsigned int display_height = 0,
                            unsigned int display_colorbit = 0,
                            unsigned int flags = 0
                          );

          SDL_Surface* getDisplay ( void );
          const unsigned int getDisplayWidth ( void ) const;
          const unsigned int getDisplayHeight ( void ) const;
          const unsigned int getDisplayColorBits ( void ) const;
          const unsigned int getDisplayFlags ( void ) const;

          bool updateDisplay ( void );

          void setTitle ( const std::string& app_name = "\0" );
          bool setIcon ( const std::string& app_icon = "\0" );
        private:
          // ...
      };
    }
}

#endif // NOMLIB_IDISPLAY_HEADERS defined
