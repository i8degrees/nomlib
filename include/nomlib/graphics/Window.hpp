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
#ifndef NOMLIB_SDL_WINDOW_HPP
#define NOMLIB_SDL_WINDOW_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
//#include "nomlib/graphics/IDisplay.hpp"
#include "nomlib/graphics/smart_ptr.hpp"
#include "nomlib/graphics/VideoMode.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Renderer.hpp"

namespace nom {

//class Renderer;

class Window:
              public Renderer
                /*: public IDisplay*/
{
  public:
    /// Convenience definition type for the std::unique_ptr variant
    typedef std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> UniquePtr;

    Window ( void );
    ~Window ( void );

    /// Initialize a SDL window and renderer
    bool create (
                  const std::string& window_title, int32 width, int32 height,
                  uint32 window_flags,
                  uint32 context_flags = SDL_RENDERER_ACCELERATED
                );

    /// Returns a raw pointer to the SDL_Window struct in use for this object
    SDL_Window* window ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool window_valid ( void ) const;

    Point2i get_position ( void ) const;

    /// Get display surface bits per pixel
    ///
    /// \todo rename method to something more along lines of Canvas equiv.
    const uint8 getDisplayColorBits ( void ) const;

    uint32 getDisplayFlags ( void ) const;
    uint16 getDisplayPitch ( void ) const;
    void* getDisplayPixels ( void ) const;
    SDL_PixelFormat* getDisplayPixelsFormat ( void ) const;
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
    //bool getCanvasLock ( void ) const;

    void set_position ( int32 x, int32 y );

    /// Lock the display context's video surface; this must be done before you
    /// attempt to write directly to video memory, such as when you are
    /// manipulating surfaces at the pixel level.
    //bool lock ( void ) const;

    /// Unlocks the display context's video surface; this must be done after you
    /// are finished writing to the video buffer. During the time that the video
    /// surface is locked, no updates (think: rendering) outside of your local
    /// access can occur until the surfaces affected by the lock are relinquished.
    //void unlock ( void ) const;

    /// Update the surface of the screen inside the window
    ///
    /// (For use only with SDL Surfaces API)
    ///
    /// Equivalent to SDL 1.2 API SDL_Flip()
    bool flip ( void ) const;

    bool fullscreen ( uint32 flags );

    const std::string window_title ( void ) const;

    void set_window_title ( const std::string& title );

    /// FIXME
    /// (Windows does not like using IMG_Load (SDL2_image extension) for some
    /// reason, which limits us solely to BMP (Windows Bitmap) files, which
    /// arguably is inconvenient ;-P I think I just need to take another look at
    /// the SDL documentation to see if this is a known limitation of their icon
    /// loader on Windows platform.
    ///
    /// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-01
    bool set_window_icon ( const std::string& filename );

    int window_id ( void ) const;
    int window_display_id ( void ) const;

  private:
    /// Internal method used for checking to see if the display context's video
    /// surfacea actually needs locking before doing so for performance sake.
    //bool mustLock ( void ) const;
    Window::UniquePtr window_;

    int window_id_;
    int window_display_id_;
    bool enabled_;
};


} // namespace nom

#endif // include guard defined
