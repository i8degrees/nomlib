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
#ifndef NOMLIB_GRAPHICS_RENDER_WINDOW_HPP
#define NOMLIB_GRAPHICS_RENDER_WINDOW_HPP

#include <vector>
#include <memory>

#include "SDL.h" // SDL2

#include "nomlib/config.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/graphics/VideoMode.hpp"
#include "nomlib/graphics/Renderer.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/clock.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

//class Renderer;

class RenderWindow: public Renderer
{
  public:
    typedef RenderWindow SelfType;
    typedef SelfType* RawPtr;

    /// \brief Default constructor; initialize an object to sane, but invalid
    /// defaults
    RenderWindow( void );

    /// \brief Destructor; the current rendering context for the object is
    /// freed at this time.
    ///
    /// \remarks The rendering context becomes invalid when the owning window
    /// object is freed. Note that any and all resources that were created using
    /// this context will also become invalid at this point.
    ~RenderWindow( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This resource has been marked non-copyable.
    RenderWindow( const SelfType& copy ) = delete;

    /// \brief Copy assignment operator.
    ///
    /// \remarks This resource has been marked non-copyable.
    SelfType& operator =( const SelfType& other ) = delete;

    /// Initialize a SDL window and renderer
    ///
    /// \todo Rename to initialize
    bool create (
                  const std::string& window_title,
                  int32 width,
                  int32 height,
                  uint32 window_flags,
                  uint32 context_flags = SDL_RENDERER_ACCELERATED
                );

    /// Initialize a SDL window and renderer.
    ///
    /// \see nom::RenderWindow::create.
    bool create (
                  const std::string& window_title,
                  const Size2i& res,
                  uint32 window_flags,
                  uint32 context_flags = SDL_RENDERER_ACCELERATED
                );

    /// Obtain a pointer to this Window object.
    RenderWindow::RawPtr get ( void );

    /// Returns a raw pointer to the SDL_Window struct in use for this object
    SDL_WINDOW::RawPtr window ( void ) const;

    /// Obtain a SDL_Surface pointer from this Window.
    SDL_SURFACE::RawPtr window_surface ( void ) const;

    /// \brief Get validity of the window and rendering context.
    bool valid( void ) const;

    /// Is this object initialized -- not nullptr?
    bool window_valid( void ) const;

    /// Obtain this Window's position.
    Point2i position ( void ) const;

    /// Get this window's size in pixels
    ///
    /// \return Point2i object containing the width (x) and height (y)
    Point2i size ( void ) const;

    /// Obtain the BPP (bits per pixel) of this window
    const uint8 bits_per_pixel ( void ) const;

    uint32 window_flags ( void ) const;

    uint32 pixel_format ( void ) const;

    /// Get the desktop area represented by a display monitor
    ///
    /// The primary desktop display is located at 0, 0.
    ///
    /// \returns  nom::IntRect object filled with the display bounds.
    ///
    /// \todo Test me
    const IntRect display_bounds ( void ) const;

    /// Obtain a list of supported video modes
    ///
    /// Returns a sorted vector of VideoMode objects, from greatest to least.
    ///
    /// \todo Test out 8-bit, 16-bit, 24-bit video surfaces
    ///
    /// \todo SDL2 support
    VideoModeList getVideoModes ( void ) const;

    void set_size ( int32 width, int32 height );

    void set_position ( int32 x, int32 y );

    /// Update the surface of the screen inside the window
    ///
    /// (For use only with SDL Surfaces API)
    ///
    /// Equivalent to SDL 1.2 API SDL_Flip()
    bool flip ( void ) const;

    /// Getter for fullscreen_ state variable
    bool fullscreen ( void ) const;

    /// Set new fullscreen state
    void set_fullscreen ( bool state );

    /// \brief Set this window full screen if it is windowed, and windowed if
    /// it is full screen.
    ///
    /// \note This method calls SDL_WINDOW_FULLSCREEN_DESKTOP
    bool toggle_fullscreen ( void );

    /// Set this window full screen if it is windowed, and windowed if it is
    /// full screen.
    ///
    /// \param flags  SDL_WINDOW_FULLSCREEN_DESKTOP || SDL_WINDOW_FULLSCREEN
    bool toggle_fullscreen ( uint32 flags );

    const std::string window_title ( void ) const;

    void set_window_title ( const std::string& title );

    /// \brief Set an icon for this window
    ///
    /// \remarks  A nom::RenderWindow must first be initialized before using this
    ///           method call; see nom::RenderWindow::create
    bool set_window_icon ( const std::string& filename );

    /// Obtain this window's unique identifier
    ///
    /// \return This window's unique identifier
    uint32 window_id( void ) const;

    /// Obtain a pointer to a SDL_Window struct by ID.
    ///
    /// \param id           Unique identifier used for the lookup
    ///
    /// \return SDL_Window pointer if exists; NULL if no Window exists
    ///
    /// \todo Is there a reason why this is a static method???
    ///
    /// \todo Rename to window_from_id?
    static SDL_WINDOW::RawPtr window_id( uint32 id );

    /// Obtain this window's display index
    ///
    /// \return This window's display index
    int window_display_id ( void ) const;

    /// \brief Get the window which currently has mouse focus.
    ///
    /// \returns The window with mouse focus.
    SDL_Window* mouse_focus( void ) const;

    /// Allow the screen to be blanked by a screen saver.
    ///
    /// \todo Test me
    void enable_screensaver ( void );

    /// Prevent the screen from being blanked by a screen saver.
    ///
    /// It is automatically re-enabled by SDL upon exit.
    ///
    /// \todo Test me
    void disable_screensaver ( void );

    /// Get the status of the OS screensaver.
    ///
    /// \return TRUE if enabled; FALSE if disabled.
    ///
    /// \todo Test me
    bool screen_saver ( void );

    /// Maximize this window.
    ///
    /// \todo Test me
    void maximize_window ( void );

    /// Minimize this window.
    ///
    /// \todo Test me
    void minimize_window ( void );

    /// Raise this window.
    ///
    /// \todo Test me
    void raise_window ( void );

    /// Restore this window.
    ///
    /// \todo Test me
    void restore_window ( void );

    /// Show this window
    ///
    /// \todo Test me
    void show_window ( void );

    /// Hide this window
    ///
    /// \todo Test me
    void hide_window ( void );

    /// Set the window input grab mode.
    ///
    /// \param grab     TRUE to grab input; FALSE to release input
    ///
    /// \todo Test me
    void set_window_grab ( bool grab );

    /// Set the minimum size of the window's client area.
    ///
    /// \param min_width    Minimum width of window in pixels
    /// \param min_height   Minimum height of window in pixels
    ///
    /// \todo Test me
    void set_minimum_window_size ( int min_width, int min_height );

    /// Set the maximum size of the window's client area.
    ///
    /// \param max_width    Maximum width of window in pixels
    /// \param max_height   Maximum height of window in pixels
    ///
    /// \todo Test me
    void set_maximum_window_size ( int max_width, int max_height );

    /// \brief Output a screen-shot of the window as a PNG file.
    ///
    /// \param    filename  Absolute or relative file path.
    ///
    /// \todo Restructure code shared with ::save_screenshot.
    ///
    /// \see RenderWindow::save_screenshot, Image::save_png.
    bool save_png_file( const std::string& filename ) const;

    /// Save a screen shot of the window as a PNG file
    ///
    /// \param    filename  Absolute or relative file path
    ///
    /// \returns  std::string containing the complete file path to the saved
    ///           image on success -- a null-terminated string on failure.
    ///
    /// \remarks  This method automatically appends a unique ID onto the end of
    ///           the given filename string.
    ///
    /// \todo    Pixels pitch calculation (see screenshot.initialize call)
    ///
    /// \see RenderWindow::save_png_file, Image::save_png.
    bool save_screenshot( const std::string& filename ) const;

    /// Set the current Window as the active rendering context; this must be
    /// called before doing any drawing (this includes creation of textures)
    /// when using multiple rendering windows.
    void make_current ( void );

    /// \brief Obtain the active rendering context
    ///
    /// \returns  Pointer to the SDL_Renderer structure containing the active
    ///           rendering context.
    ///
    /// \remarks  Used internally within nomlib for automatically using the
    ///           active context -- set by nom::RenderWindow::make_current.
    static SDL_Renderer* context( void );

  private:
    /// \brief  Set a new nom::RenderWindow as the active rendering context; we must
    ///         always have a context active at any given time for generating
    ///         resources -- nom::Texture, nom::Image, etc.
    static void set_context ( RenderWindow::RawPtr window );

    static SDL_Renderer* context_;

    SDL_WINDOW::UniquePtr window_;

    /// Cache the unique window identifier we get from SDL upon initialization
    uint32 window_id_;

    /// Cache the display identifier we get from SDL upon initialization
    int window_display_id_;

    /// State of the window (visible or not)
    bool enabled_;

    /// Toggle window & full-screen states
    bool fullscreen_;
};


} // namespace nom

#endif // include guard defined
