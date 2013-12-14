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
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

/// \brief Video subsystem responsible for managing high-level graphics display
/// (think: *very* fancy back-buffer).
///
/// A nom::Window is lifeless without its attached Renderer buddy!
class Renderer
{
  public:
    /// Default constructor; initializes instance to sane defaults
    Renderer ( void );

    /// Lazy destructor with nothing to do
    virtual ~Renderer ( void );

    /// Initialize a Renderer instance with an existing, initialized SDL_Window
    ///
    /// Initializes with the first rendering driver supporting our request
    /// flags
    /// Enables video acceleration when able
    bool initialize ( SDL_WINDOW::RawPtr window, int32 rendering_driver = -1, uint32 context_flags = SDL_RENDERER_ACCELERATED );

    /// Get a raw pointer to the SDL_Renderer in use
    SDL_RENDERER::RawPtr renderer ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool renderer_valid ( void ) const;

    /// Get the current logical resolution size
    ///
    /// \todo Test me!
    const Point2i logical_size ( void ) const;

    /// Get the current viewport dimensions
    ///
    /// \todo Test me!
    const Coords viewport ( void ) const;

    /// Obtain the current scale factor
    const Point2i scale_factor ( void ) const;

    /// Obtain the blending mode used for drawing
    const SDL_BlendMode blend_mode ( void ) const;

    /// Obtain the renderer's output size (width & height) in pixels
    ///
    /// \return Point2i::null on ERR
    const Point2i size ( void ) const;

    /// Obtain the renderer's clipping rectangle bounds (X, Y, width & height)
    /// in pixels
    /// \todo Test me!
    const Coords bounds ( void ) const;

    /// Update the renderer surface on the attached window
    void update ( void ) const;

    /// Set an input texture source as the new rendering target.
    ///
    /// \param input_texture    The targeted texture
    bool update ( SDL_TEXTURE::RawPtr input_texture );

    /// Clear the whole rendering object; ignoring the viewport.
    ///
    /// (to a rendering color, if set?)
    ///
    /// This method is usually called after setting the rendering draw color.
    bool clear ( void ) const;

    /// Fill the rendering target video display with a new color.
    ///
    /// This combines two methods into one: set_color() and clear().
    bool fill ( const Color4u& color );

    /// Set device independent resolution for rendering
    ///
    /// \param width      Width of the new logical resolution
    /// \param height     Height of the new logical resolution
    ///
    /// Sets both a new view port and any necessary rescaling to fit within a
    /// fixed, logical resolution for rendering.
    bool set_logical_size ( int width, int height );

    /// Set a new drawing area for rendering
    ///
    /// \param rect       Represents the drawing area to update;
    ///                   passing Coords::null to set the viewport to the entire
    ///                   target
    ///
    /// \note When the window is resized, the current viewport is automatically
    /// resized to compensate.
    /// \todo Test me!
    bool set_viewport ( const Coords& rect );

    bool set_color ( const Color4u& color );

    /// Set the color blending mode (fill and line).
    ///
    /// SDL_BLENDMODE_NONE
    /// SDL_BLENDMODE_BLEND
    /// SDL_BLENDMODE_ADD
    /// SDL_BLENDMODE_MOD
    bool set_blend_mode ( const SDL_BlendMode mode );

    /// Set new clipping rectangle bounds for the rendering target.
    ///
    /// \param clip_bounds  Passing nom::Coords::null will disable clipping on
    ///                     the target
    ///
    /// \todo Test me!
    bool set_bounds ( const Coords& clip_bounds );

    /// Obtain pixels buffer of the entire rendering target
    ///
    /// \fixme Pixels pitch calculation
    void* pixels ( void ) const;

  protected:
    /// This is automatically released after the attached nom::Window has been
    /// destroyed.
    SDL_RENDERER::UniquePtr renderer_;
};


} // namespace nom

#endif // include guard defined
