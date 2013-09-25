/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SDL_DISPLAY_HEADERS
#define NOMLIB_SDL_DISPLAY_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include <SDL/SDL.h>

#include "nomlib/config.hpp"
//#include "nomlib/graphics/IDisplay.hpp"
#include "nomlib/graphics/VideoMode.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/graphics/Canvas.hpp"

namespace nom {
  namespace priv {

/// Custom deleter for the display context smart pointer -- this is managed by
/// SDL and thus we do not own it -- we must let SDL manage it, so we
/// intentionally will do nothing in this call.
void Display_FreeSurface ( Surface* );

  } // namespace priv
} // namespace nom


namespace nom {

class Display/*: public IDisplay*/
{
  public:
    Display ( void );
    ~Display ( void );

    void createWindow ( int32_t display_width, int32_t display_height,
                        int32_t display_colorbit, uint32_t flags = 0
                      );
    /// Obtains raw pointer to the object's video surface buffer
    ///
    /// Returns ( SDL_Surface* )
    ///
    Surface* get ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    int32_t getDisplayWidth ( void ) const;
    int32_t getDisplayHeight ( void ) const;

    /// Get display surface bits per pixel
    ///
    /// \todo rename method to something more along lines of Canvas equiv.
    const uint8 getDisplayColorBits ( void ) const;

    uint32_t getDisplayFlags ( void ) const;
    u_short getDisplayPitch ( void ) const;
    void* getDisplayPixels ( void ) const;
    PixelFormat* getDisplayPixelsFormat ( void ) const;
    const Coords getDisplayBounds ( void ) const;

    /// Obtain a list of supported video modes
    ///
    /// Returns a sorted vector of VideoMode objects, from greatest to least.
    ///
    /// \todo Test out 8-bit, 16-bit, 24-bit video surfaces
    VideoModeList getVideoModes ( void ) const;

    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    bool getCanvasLock ( void ) const;

    /// Lock the display context's video surface; this must be done before you
    /// attempt to write directly to video memory, such as when you are
    /// manipulating surfaces at the pixel level.
    bool lock ( void ) const;

    /// Unlocks the display context's video surface; this must be done after you
    /// are finished writing to the video buffer. During the time that the video
    /// surface is locked, no updates (think: rendering) outside of your local
    /// access can occur until the surfaces affected by the lock are relinquished.
    void unlock ( void ) const;

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

  private:
    /// Internal method used for checking to see if the display context's video
    /// surfacea actually needs locking before doing so for performance sake.
    bool mustLock ( void ) const;
};


} // namespace nom

#endif // NOMLIB_SDL_DISPLAY_HEADERS defined
