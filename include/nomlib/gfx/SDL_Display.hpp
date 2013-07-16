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

#include "nomlib/config.hpp"
#include "nomlib/gfx/IDisplay.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/gfx/SDL_Canvas.hpp"

namespace nom {
  namespace priv {

/// Custom deleter for the display context smart pointer -- this is managed by
/// SDL and thus we do not own it -- we must let SDL manage it, so we
/// intentionally will do nothing in this call.
void Display_FreeSurface ( SDL_Surface* );

  } // namespace priv
} // namespace nom


namespace nom {

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

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    int32_t getDisplayWidth ( void ) const;
    int32_t getDisplayHeight ( void ) const;
    int32_t getDisplayColorBits ( void ) const;
    uint32_t getDisplayFlags ( void ) const;
    u_short getDisplayPitch ( void ) const;
    void* getDisplayPixels ( void ) const;
    void* getDisplayPixelsFormat ( void ) const;
    const Coords getDisplayBounds ( void ) const;
    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    bool getCanvasLock ( void ) const;

    void Update ( void );

    /// As per libSDL docs, this method call should not be used when the display
    /// surface is locked
    /// \todo TEST ME
    void Update ( const Coords& coords );
    void toggleFullScreenWindow ( int32_t width, int32_t height );

    const std::string getWindowTitle ( void ) const;
    void* getWindowIcon ( void ) const;

    void setWindowTitle ( const std::string& app_name = "\0" );
    void setWindowIcon ( const std::string& app_icon = "\0" );
};


} // namespace nom

#endif // NOMLIB_SDL_DISPLAY_HEADERS defined
