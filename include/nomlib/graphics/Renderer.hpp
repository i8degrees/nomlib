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
#ifndef NOMLIB_SDL2_RENDERER_HPP
#define NOMLIB_SDL2_RENDERER_HPP

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
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

    Coords viewport ( void ) const;

    /// Obtain the blending mode used for drawing
    const SDL_BlendMode blend_mode ( void ) const;

    /// Update the renderer surface on the attached window
    void update ( void ) const;

    /// Set an input texture source as the new rendering target.
    ///
    /// \param input_texture    The targeted texture
    bool update ( SDL_Texture* input_texture );

    /// Clear the whole rendering object; ignoring the viewport.
    ///
    /// (to a rendering color, if set?)
    ///
    /// This method is usually called after setting the rendering draw color.
    bool clear ( void ) const;

    /// Fill the rendering target video display with a new color.
    ///
    /// This combines two methods into one: set_color() and clear().
    ///
    /// Default color fill is "BSOD (TM) Blue".
    bool fill ( const Color& color = Color::Blue );

    /// \brief Set device independent resolution for rendering
    ///
    /// \param width      Width of the new logical resolution
    /// \param height     Height of the new logical resolution
    ///
    /// Sets both a new view port and any necessary rescaling to fit within a
    /// fixed, logical resolution for rendering.
    bool set_viewport ( int width, int height );

    bool set_color ( const Color& color );

    /// Set the color blending mode (fill and line).
    ///
    /// SDL_BLENDMODE_NONE
    /// SDL_BLENDMODE_BLEND
    /// SDL_BLENDMODE_ADD
    /// SDL_BLENDMODE_MOD
    bool set_blend_mode ( const SDL_BlendMode mode );

  protected:
    /// This is automatically released after the attached nom::Window has been
    /// destroyed.
    Renderer::UniquePtr renderer_;
};


} // namespace nom

#endif // include guard defined
