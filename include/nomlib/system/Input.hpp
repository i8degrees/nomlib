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
#ifndef NOMLIB_SDL2_INPUT_HEADERS
#define NOMLIB_SDL2_INPUT_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"

/// Enable debugging output of every key press & release event
//#define NOM_DEBUG_SDL2_KEYBOARD_INPUT

/// Enable debugging output of every mouse click, release & motion
//#define NOM_DEBUG_SDL2_MOUSE_INPUT

/// Enable debugging output of every joystick button event
//#define NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT

/// Enable debugging output of every joystick axis event
//#define NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT

/// Enable debugging output of every joystick addition & removal event
//#define NOM_DEBUG_SDL2_JOYSTICK_EVENT

/// Enable debugging output of every Drag 'N' Drop event
#define NOM_DEBUG_SDL2_DRAG_DROP_INPUT

namespace nom {

/// SDL mapping to individual PS3 axes
///
/// TODO: Needs testing / verification
enum PSXAXES
{
  LEFTX = 0,
  LEFTY = 1,
  RIGHTX = 2,
  RIGHTY = 3
};

/// SDL mapping to individual PS3 controller buttons
enum PSXBUTTON
{
  SELECT            = 0,
  JOYCLICKLEFT      = 1,
  JOYCLICKRIGHT     = 2,
  START             = 3,
  UP                = 4,
  RIGHT             = 5,
  DOWN              = 6,
  LEFT              = 7,
  L2                = 8,
  R2                = 9,
  L1                = 10,
  R1                = 11,
  TRIANGLE          = 12,
  CIRCLE            = 13,
  CROSS             = 14,
  SQUARE            = 15,
  PSBUTTON          = 16
};

/// \brief SDL Input Events Handling
class Input
{
  public:
    typedef std::unique_ptr<SDL_Joystick, void (*)(SDL_Joystick*)> JoystickUniquePtr;

    Input ( void );
    virtual ~Input ( void );

    virtual void HandleInput ( EventType* event );
  protected:
    virtual void onUserEvent ( uint32 type, int32 code, void* data1, void* data2 );

    virtual void onQuit ( void );
    virtual void onResize ( int32 width, int32 height );
    virtual void onRestore ( void );
    virtual void onMinimize ( void );
    virtual void onInputFocus ( void );
    virtual void onInputBlur ( void );
    virtual void onMouseFocus ( void );
    virtual void onMouseBlur ( void );
    virtual void onExpose ( void );

    virtual void onKeyDown ( int32 key, int32 mod, uint32 window_id );
    virtual void onKeyUp ( int32 key, int32 mod, uint32 window_id  );

    virtual void onMouseMotion ( int32 x, int32 y, uint32 window_id );

    /// Handler for mouse wheel events
    ///
    /// \remark x > 0 = Left
    /// \remark x < 0 = Right
    /// \remark y > 0 = Up
    /// \remark y < 0 = Down
    ///
    /// \note Coordinates values X & Y are typically inverted; software -- such
    /// as the Mouse Preference Pane under OS X v10.7+ -- allow you to reverse
    /// this while using the Apple Mighty Mouse.
    virtual void onMouseWheel ( int32 x, int32 y, uint32 window_id );
    virtual void onMouseLeftButtonDown ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseMiddleButtonDown ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseRightButtonDown ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseButtonFourDown ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseButtonFiveDown ( int32 x, int32 y, uint32 window_id  );

    virtual void onMouseLeftButtonUp ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseMiddleButtonUp ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseRightButtonUp ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseButtonFourUp ( int32 x, int32 y, uint32 window_id  );
    virtual void onMouseButtonFiveUp ( int32 x, int32 y, uint32 window_id  );

    virtual void onJoyButtonDown ( int32 which, int32 button );
    virtual void onJoyButtonUp ( int32 which, int32 button );
    virtual void onJoyAxis ( int32 which, int32 axis, uint16 value );
    virtual void onJoystickAdd ( void );
    virtual void onJoystickRemove ( void );

    /// Drag 'N' Drop events
    ///
    /// \note To enable drag and drop events on Mac OS X, you must add the
    /// appropriate "Document Types" in your application bundle's Info.plist.
    ///
    /// (Document Types: public.data to enable all types)
    virtual void onDragDrop ( const std::string& file_path, uint32 timestamp );

  private:
    JoystickUniquePtr joystick_;
    SDL_JoystickID joystick_id_;
};


} // namespace nom

#endif // include guard defined
