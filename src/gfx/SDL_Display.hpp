/******************************************************************************

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_DISPLAY_HEADERS
#define NOMLIB_SDL_DISPLAY_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>

#include <SDL/SDL.h>

#include "gfx/IDisplay.hpp"
#include "math/Coords.hpp"
#include "math/Color.hpp"
#include "gfx/SDL_Canvas.hpp"
#include "nomlib_config.hpp"

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
      /// Obtains raw pointer to the object's video surface buffer
      ///
      /// Returns ( SDL_Surface* )
      ///
      void* get ( void ) const;

      /// Is this SDL_Surface* initialized ( != nullptr )?
      bool valid ( void ) const;

      int32_t getDisplayWidth ( void ) const;
      int32_t getDisplayHeight ( void ) const;
      int32_t getDisplayColorBits ( void ) const;
      uint32_t getDisplayFlags ( void ) const;
      u_short getDisplayPitch ( void ) const;
      void* getDisplayPixels ( void ) const;
      void* getDisplayPixelsFormat ( void ) const;
      const nom::Coords getDisplayBounds ( void ) const;
      /// I think that we are accessing the value of an
      /// (internal?) property of the SDL_Surface structure that is described as being
      /// "private" as per the docs.
      ///
      /// Return value of this internal property is presumed to be boolean -- no
      /// verification has been made of this. Testing of this method *appears*
      /// to be in working order.
      bool getCanvasLock ( void ) const;

      void Update ( void );
      /// \todo
      /// TEST ME
      /// \endtodo
      /// As per libSDL docs, this method call should not be used when the display
      /// surface is locked
      void Update ( const nom::Coords& coords );
      void toggleFullScreenWindow ( int32_t width, int32_t height );

      const std::string getWindowTitle ( void ) const;
      void* getWindowIcon ( void ) const;

      void setWindowTitle ( const std::string& app_name = "\0" );
      void setWindowIcon ( const std::string& app_icon = "\0" );

    private:
      // ...
  };
}

#endif // NOMLIB_SDL_DISPLAY_HEADERS defined
