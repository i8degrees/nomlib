/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

#include <SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/RendererInfo.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

// Forward declarations
class Texture;

/// \brief Video subsystem responsible for managing high-level graphics display
/// (think: *very* fancy back-buffer).
///
/// A nom::RenderWindow is lifeless without its attached Renderer buddy!
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
    bool create ( SDL_WINDOW::RawPtr window, int32 rendering_driver = -1, uint32 context_flags = SDL_RENDERER_ACCELERATED );

    /// Get a raw pointer to the SDL_Renderer in use
    SDL_Renderer* renderer ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool renderer_valid ( void ) const;

    /// Get the current logical resolution size.
    Size2i logical_size() const;

    /// \brief Get the drawing scale for the rendering context.
    ///
    /// \see ::set_scale, ::set_logical_size.
    ///
    /// \todo Rename to logical_scale.
    const Point2f scale() const;

    /// Get the current viewport dimensions
    const IntRect viewport ( void ) const;

    /// Obtain the blending mode used for drawing
    const SDL_BlendMode blend_mode ( void ) const;

    /// \brief Get the renderer's output size dimensions.
    ///
    /// \returns Size2i object filled with the width and height fields.
    Size2i output_size( void ) const;

    /// Obtain the renderer's clipping rectangle bounds (X, Y, width & height)
    /// in pixels
    /// \todo Test me!
    const IntRect bounds ( void ) const;

    /// \brief Obtain information specific to your rendering hardware
    /// capabilities.
    ///
    /// \remarks See nom::RendererInfo
    const RendererInfo caps ( void ) const;

    /// \brief Obtain information specific to your rendering hardware
    /// capabilities.
    ///
    /// \note   This method is used internally by nomlib -- nom::Renderer::caps()
    ///         is probably the API you want to use outside of nomlib.
    static const RendererInfo caps ( SDL_Renderer* target );

    /// \brief Set the current rendering target back to the default renderer.
    bool reset_render_target() const;

    /// \brief Set a texture as the current rendering target.
    ///
    /// \remarks The nom::Texture must be initialized as a
    /// nom::Texture::Access::RenderTarget.
    ///
    /// \note Not all video hardware has support for this feature.
    ///
    /// \see ::reset_render_target
    bool set_render_target(const Texture* texture) const;

    /// Update the renderer surface on the attached window
    void update ( void ) const;

    /// Clear the whole rendering object; ignoring the viewport.
    ///
    /// (to a rendering color, if set?)
    ///
    /// This method is usually called after setting the rendering draw color.
    bool clear ( void ) const;

    /// Fill the rendering target video display with a new color.
    ///
    /// This combines two methods into one: set_color() and clear().
    bool fill ( const Color4i& color );

    /// Set device independent resolution for rendering
    ///
    /// \param width      Width of the new logical resolution
    /// \param height     Height of the new logical resolution
    ///
    /// Sets both a new view port and any necessary rescaling to fit within a
    /// fixed, logical resolution for rendering.
    bool set_logical_size( int width, int height );

    /// \see ::set_logical_size.
    bool set_logical_size( const Size2i& size );

    /// \brief Set the drawing scale for this rendering context.
    ///
    /// \see ::set_logical_size.
    ///
    /// \todo Rename to set_logical_scale.
    bool set_scale( const Point2f& scale_factor );

    /// Set a new drawing area for rendering
    ///
    /// \param bounds     Represents the drawing area to update;
    ///                   passing IntRect::null to set the viewport to the
    ///                   entire target
    ///
    /// \note When the window is resized, the current viewport is automatically
    /// resized to compensate.
    /// \todo Test me!
    bool set_viewport ( const IntRect& bounds );

    bool set_color ( const Color4i& color );

    /// Set the color blending mode (fill and line).
    ///
    /// SDL_BLENDMODE_NONE
    /// SDL_BLENDMODE_BLEND
    /// SDL_BLENDMODE_ADD
    /// SDL_BLENDMODE_MOD
    bool set_blend_mode ( const SDL_BlendMode mode );

    /// Set new clipping rectangle bounds for the rendering target.
    ///
    /// \param bounds    Passing nom::IntRect::null will disable clipping on
    ///                  the target
    ///
    /// \todo Test me!
    bool set_bounds ( const IntRect& bounds );

    /// \brief Obtain pixels buffer of the entire rendering target
    ///
    /// \returns An allocated memory buffer pointing to the current rendering
    /// target's pixel values on success, or NULLPTR on failure, such as a
    /// memory allocation failure (out of memory?).
    ///
    /// \remarks It is your responsibility to free the pointer that is returned.
    ///
    /// \todo Pixels pitch calculation
    void* pixels() const;

  protected:
    /// This is automatically released after the attached nom::RenderWindow has been
    /// destroyed.
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;
};


} // namespace nom

#endif // include guard defined
