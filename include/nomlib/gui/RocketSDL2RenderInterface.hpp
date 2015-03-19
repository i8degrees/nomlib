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
#ifndef NOMLIB_GUI_ROCKET_SDL2_RENDER_INTERFACE_HPP
#define NOMLIB_GUI_ROCKET_SDL2_RENDER_INTERFACE_HPP

// #include <memory>

#include <Rocket/Core/Core.h>
#include <Rocket/Core/RenderInterface.h>

#include "nomlib/config.hpp"

#if defined(NOM_PLATFORM_WINDOWS)
  #define NOM_API_EXPORT __stdcall
#else
  #define NOM_API_EXPORT
#endif

namespace nom {

namespace priv {

typedef void (NOM_API_EXPORT* glUseProgramObjectARB_func) (unsigned int);

} // namespace priv

// Forward declarations
class RenderWindow;

/// \brief Rendering interfacing bridge between libRocket and nomlib
///
/// \note Currently, only the OpenGL back-end for SDL2 is supported.
///
/// \todo Add support for OpenGLES and OpenGLES2 (mobile platforms).
/// \see http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2
class RocketSDL2RenderInterface: public Rocket::Core::RenderInterface
{
  public:
    /// \brief Initialize OpenGL with the necessary settings for libRocket and
    /// SDL2 to be all friendly.
    ///
    /// \returns Boolean TRUE if OpenGL initialization is successful, and
    /// boolean FALSE on failure.
    ///
    /// \param width  The width of the clipping plane (orthographic matrix).
    /// \param height The height of the clipping plane (orthographic matrix).
    ///
    /// \note This method should be called before the construction of this
    /// object, after the creation of a RenderWindow, and before any other
    /// rendering (OpenGL or otherwise).
    ///
    /// \remarks This is not part of the libRocket interface, and is provided
    /// as part of nomlib's high-level initialization routines.
    static bool gl_init( int width, int height );

    RocketSDL2RenderInterface( RenderWindow* window );

    virtual ~RocketSDL2RenderInterface();

    /// \brief Implements Rocket::Core::SystemInterface::Release.
    ///
    /// \remarks Called when this file interface is released.
    virtual void Release();

    /// Called by Rocket when it wants to render geometry that it does not wish to optimise.
    virtual void
    RenderGeometry( Rocket::Core::Vertex* vertices, int num_vertices,
                    int* indices, int num_indices,
                    Rocket::Core::TextureHandle texture_handle,
                    const Rocket::Core::Vector2f& translation);

    /// Called by Rocket when it wants to enable or disable scissoring to clip content.
    virtual void EnableScissorRegion(bool enable);

    /// \brief Called by Rocket when it wants to change the scissor region.
    ///
    /// \fixme The clipping calculations are slightly off for the y / height
    /// dimensions. This bug can often be seen with use of libRocket's scrollbar
    /// feature (i.e.: visual debugger's element info window).
    virtual void SetScissorRegion(int x, int y, int width, int height);

    /// Called by Rocket when a texture is required by the library.
    virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
    /// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
    virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);

    /// Called by Rocket when a loaded texture is no longer required.
    virtual void
    ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

    /// \brief nomlib interface bridge between SDL2 and libRocket
    ///
    /// \remarks The interface does **not** own the pointer.
    RenderWindow* window_;

  private:
    /// \brief Shader context function pointer
    ///
    /// \note We bypass the use of GLEW by requesting this extension through
    /// SDL2.
    static priv::glUseProgramObjectARB_func ctx_;
};

} // namespace nom

#endif // include guard defined
