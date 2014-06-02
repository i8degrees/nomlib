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
#ifndef NOMLIB_GRAPHICS_CURSOR_HPP
#define NOMLIB_GRAPHICS_CURSOR_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"

// Forward declarations
struct SDL_Cursor;

namespace nom {

namespace priv {

void free_cursor( SDL_Cursor* c );

} // namespace priv

struct MouseState
{
  /// \brief The X & Y coordinates of the mouse cursor position, relative to
  /// the focused window.
  Point2i pos;

  /// \brief 32-bit button bit-mask of the current button state.
  uint32 state;
};

/// \brief Platform-dependent mouse cursor interface
///
/// \todo Implement interfacing for SDL_CreateCursor and SDL_CreateSystemCursor.
/// \see https://wiki.libsdl.org/CategoryMouse
class Cursor
{
  public:
    typedef Cursor self_type;

    typedef self_type* raw_ptr;

    /// \brief The mouse cursor types.
    ///
    /// \see SDL_SystemCursor
    enum Type
    {
      Arrow,      /// Arrow
      Beam,       /// Infrared-beam
      Wait,       /// Wait
      CrossHair,  /// Crosshair
      WaitArrow,  /// Small wait cursor, or MouseCursor::CursorType::Wait if
                  /// not available
      SizeENWSE,  /// Double arrow pointing northwest and southeast
      SizeNESW,   /// Double arrow pointing northeast and southwest
      SizeWE,     /// Double arrow pointing west and east
      SizeNS,     /// Double arrow pointing north and south
      SizeAll,    /// Four pointed arrow pointing north, south, east and west
      No,         /// Slashed circle or crossbones
      Hand,       /// Hand
      NumSystemCursors
    };

    /// \brief Default constructor.
    Cursor( void );

    /// \brief Destructor.
    ~Cursor( void );

    /// \brief Get the active mouse cursor.
    ///
    /// \returns The active mouse cursor on success, or NULL on failure, such
    /// as if there is no mouse.
    ///
    /// \remarks This method returns a pointer to the current cursor, which is
    /// owned by SDL. You should *not* free the pointer.
    SDL_Cursor* cursor( void ) const;

    /// \brief Get the default mouse cursor.
    ///
    /// \returns The default mouse cursor on success, or NULL on failure.
    SDL_Cursor* default_cursor( void ) const;

    /// \brief Retrieve the current state of the mouse.
    ///
    /// \see https://wiki.libsdl.org/SDL_GetMouseState?highlight=%28%5CbCategoryMouse%5Cb%29%7C%28CategoryEnum%29%7C%28CategoryStruct%29
    MouseState mouse_state( void ) const;

    Type system_cursor( void ) const;

    /// \brief Set a platform-dependent system cursor.
    ///
    /// \see nom::Cursor::Type enumerations.
    void set_system_cursor( Type id );

    /// \brief Get the state of the cursor.
    ///
    /// \returns Boolean TRUE if the cursor state is enabled, and boolean FALSE
    /// if the cursor state is disabled.
    bool cursor_state( void ) const;

    /// \brief Toggle the visibility state of the cursor.
    ///
    /// \todo Error handling?
    void show_cursor( bool state );

  private:
    /// \brief Set the active cursor.
    ///
    /// \remarks This method can be used to force a cursor redraw by passing
    /// NULL.
    void set_cursor( SDL_Cursor* cursor );

    /// \brief Track the active system cursor in use.
    Type system_cursor_;

    // std::unique_ptr<SDL_Cursor, void(*) (SDL_Cursor*)> cursor_;
};

} // namespace nom

#endif // include guard defined
