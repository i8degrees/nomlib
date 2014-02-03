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
#ifndef NOMLIB_GUI_UI_EVENT_HPP
#define NOMLIB_GUI_UI_EVENT_HPP

#include "nomlib/config.hpp"

namespace nom {

/// \brief One and only one event instance of each unique enumeration may be
/// in use (listening / observed) at a time, thus the division that I have
/// given them; "public" event types refer to those that the end-user
/// (developer) has access to, whereas the "private" events are those that are
/// either  already in use by class instances, or reserved  for future (private)
/// implementation.
enum UIEvent
{
  INVALID = 0,        // Default

  // Private event slots

  ON_KEY_DOWN,
  ON_KEY_UP,
  ON_MOUSE_MOTION,
  ON_MOUSE_DOWN,
  ON_MOUSE_UP,
  ON_MOUSE_WHEEL,
  ON_JOY_DOWN,
  ON_JOY_UP,
  ON_JOY_AXIS,
  ON_TOUCH_DOWN,

  ON_WINDOW_KEY_DOWN,
  ON_WINDOW_KEY_UP,
  ON_WINDOW_MOUSE_MOTION,
  ON_WINDOW_MOUSE_DOWN,
  ON_WINDOW_MOUSE_UP,
  ON_WINDOW_MOUSE_WHEEL,
  ON_WINDOW_JOY_DOWN,
  ON_WINDOW_JOY_UP,
  ON_WINDOW_JOY_AXIS,
  ON_WINDOW_TOUCH_DOWN,
  ON_WINDOW_TOUCH_UP,

  /// The event emitted when the widget's position has been modified.
  ///
  /// This event type models SDL_WINDOWEVENT_MOVED for the sake of consistency.
  ON_WINDOW_MOVED,

  /// The event emitted when the widget's size has been modified.
  ///
  /// This event is always preceded by ON_WINDOW_SIZE_CHANGED.
  //
  /// This event type models SDL_WINDOWEVENT_RESIZED for the sake of consistency.
  ON_WINDOW_RESIZED,

  /// The emitted event when the widget's size fields has been modified, either
  /// as the result of the underlying system (us, i.e.: layout update) or by the
  /// end-user.
  ///
  /// This event is followed by ON_WINDOW_RESIZED when the size was modified by
  /// an external event, i.e.: end-user or the underlying system (us, i.e.:
  /// layout).
  ///
  /// This event type models SDL_WINDOWEVENT_SIZE_CHANGED for the sake of
  /// consistency.
  ON_WINDOW_SIZE_CHANGED,

  /// The event emitted when the underlying system (us) requests that the widget's
  /// window be closed.
  ON_WINDOW_CLOSE,

  // Public event slots

  KEY_DOWN,
  KEY_UP,
  MOUSE_MOTION,
  MOUSE_DOWN,
  MOUSE_UP,
  MOUSE_WHEEL,
  JOY_DOWN,
  JOY_UP,
  JOY_AXIS,
  TOUCH_DOWN,
  TOUCH_UP,

  WINDOW_KEY_DOWN,
  WINDOW_KEY_UP,
  WINDOW_MOUSE_MOTION,
  WINDOW_MOUSE_DOWN,
  WINDOW_MOUSE_UP,
  WINDOW_MOUSE_WHEEL,
  WINDOW_JOY_DOWN,
  WINDOW_JOY_UP,
  WINDOW_JOY_AXIS,
  WINDOW_TOUCH_DOWN,
  WINDOW_TOUCH_UP,

  WINDOW_MOVED,
  WINDOW_RESIZED,
  WINDOW_SIZE_CHANGED,
  WINDOW_CLOSE,

  MOUSE_DCLICK,
  MOUSE_LEFT_UP,
  DROP_FILE,
  WINDOW_FOCUS
};

/*
enum UIKeyEvent
{
  KEY_DOWN = 0,
  KEY_UP
};

enum UIJoystickEvent
{
  JOY_BUTTON_DOWN = 0,
  JOY_BUTTON_UP,
  JOY_DEVICE_CONNECTED,
  JOY_DEVICE_DISCONNECTED
};

enum UITextEvent
{
  INVVALID = 0,
  START_TEXT_EDITING,
  STOP_TEXT_EDITING
};

enum UIWidgetEvent
{
  INVALID = 0,
  FOCUS,
  DEFOCUS,
  DRO_PFILE,
  RESIZE,
  MOVE,
  RENDER,
  KEY_DOWN,
  KEY_UP,
  MINIMIZE,
  RESTORE,
  CLIPBOARD_CUT,
  CLIPBOARD_COPY
  CLIPBOARD_PASTE
};

enum UIMouseEvent
{
  INVALID = 0,
  MOUSE_DOWN,
  MOUSE_UP,
  MOUSE_LEFT_DOWN,
  MOUSE_LEFT_UP,
  MOUSE_RIGHT_DOWN,
  MOUSE_RIGHT_UP,
  MOUSE_MIDDLE_DOWN,
  MOUSE_MIDDLE_UP,
  MOUSE_LEFT_DCLICK,
  MOUSE_RIGHT_DCLICK,
  MOUSE_MIDDLE_DCLICK,
  MOUSE_ENTER_WINDOW,
  MOUSE_LEAVE_WINDOW
};

enum UIWheelEvent
{
  INVALID = 0,
  MOUSE_WHEEL
};
*/

/*
class IEvent
{
  public:
    typedef IEvent self_type;
    typedef self_type* raw_ptr;

    IEvent( void ) :
      type_{ 0 },
      timestamp_{ 0 }
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual ~IEvent( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    IEvent( uint32 type, uint32 timestamp ) :
      type_{ type },
      timestamp_{ timestamp }
    {
      // NOM_LOG_TRACE( NOM );
    }

    // IEvent::raw_ptr clone( void ) const
    // {
    //   return IEvent::raw_ptr( new IEvent( *this ) );
    // }

    // virtual IEvent::raw_ptr clone( void ) const = 0;

    uint32 type( void ) const
    {
      return this->type_;
    }

    uint32 timestamp( void ) const
    {
      return this->timestamp_;
    }

    void set_type( uint32 type )
    {
      this->type_ = type;
    }

    void set_timestamp( uint32 timestamp )
    {
      this->timestamp_ = timestamp;
    }

  private:
    uint32 type_;
    uint32 timestamp_;
};

class UIKeyEvent: public IEvent
{
  public:
    typedef UIKeyEvent self_type;
    typedef self_type* raw_ptr;

    UIKeyEvent( void ) :
      IEvent( 768, 0 ),
      sym_{ 0 },
      mod_{ KMOD_NONE },
      window_id_{ 0 }
    {
      // NOM_LOG_TRACE( NOM );
    }

    ~UIKeyEvent( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    UIKeyEvent( uint32 type, int32 symbol ) :
      IEvent( type, 0 ),
      sym_{ symbol },
      mod_{ KMOD_NONE },
      window_id_{ 0 }
    {
      // NOM_LOG_TRACE( NOM );
    }

    UIKeyEvent( uint32 type, int32 symbol, uint16 mod ) :
      IEvent( type, 0 ),
      sym_{ symbol },
      mod_{ mod },
      window_id_{ 0 }
    {
      // NOM_LOG_TRACE( NOM );
    }

    IEvent::raw_ptr clone( void ) const
    {
      return UIKeyEvent::raw_ptr( new UIKeyEvent( *this ) );
    }

    int32 symbol( void ) const
    {
      return this->sym_;
    }

    uint16 modifier( void ) const
    {
      return this->mod_;
    }

  private:
    int32 sym_;
    uint16 mod_;
    uint32 window_id_;
};
*/

} // namespace nom

#endif // include guard defined
