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
#ifndef NOMLIB_SDL_RENDERER_HPP
#define NOMLIB_SDL_RENDERER_HPP

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/graphics/smart_ptr.hpp"

namespace nom {

/// \brief Video subsystem responsible for managing high-level graphics display
/// (think: fancy backbuffer).
///
/// A nom::Window is lifeless without its attached Renderer buddy!
class Renderer
{
  public:
    /// Convenience definition type for the std::unique_ptr variant
    typedef std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> UniquePtr;

    /// Default constructor; initializes instance to sane defaults
    Renderer ( void );

    /// Lazy destructor with nothing to do
    virtual ~Renderer ( void );

    /// Initialize a Renderer instance with an existing, initialized SDL_Window
    ///
    /// Initializes with the first rendering driver supporting our request
    /// flags
    /// Enables video acceleration when able
    bool initialize ( SDL_Window* window, int32 rendering_driver = -1, uint32 context_flags = SDL_RENDERER_ACCELERATED );

    /// Get a raw pointer to the SDL_Renderer in use
    SDL_Renderer* renderer ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool renderer_valid ( void ) const;

    /// Update the renderer surface on the attached window
    void update ( void ) const;

    /// Fill the rendering target video display with a new color.
    ///
    /// Default color fill is "BSOD (TM) Blue".
    bool clear ( const Color& color = Color::Blue );

  protected:
    /// This is automatically released after the attached nom::Window has been
    /// destroyed.
    Renderer::UniquePtr renderer_;
};


} // namespace nom

#endif // include guard defined
