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
#include "nomlib/system/Event.hpp"

namespace nom {

class UIEvent
{
  public:
    typedef UIEvent self_type;
    typedef self_type* raw_ptr;

    /// \brief One and only one event instance of each unique enumeration may be
    /// in use (listening / observed) at a time, thus the division that I have
    /// given them; "public" event types refer to those that the end-user
    /// (developer) has access to, whereas the "private" events are those that are
    /// either  already in use by class instances, or reserved  for future (private)
    /// implementation.
    enum
    {
      INVALID = 0,        // Default

      // Private event slots

      ON_KEY_DOWN,
      ON_KEY_UP,
      ON_MOUSE_MOTION_ENTER,
      ON_MOUSE_MOTION_LEAVE,
      ON_MOUSE_DOWN,
      ON_MOUSE_UP,
      ON_MOUSE_DCLICK,
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

      ON_WIDGET_UPDATE,

      // Public event slots

      KEY_DOWN,
      KEY_UP,
      MOUSE_MOTION_ENTER,
      MOUSE_MOTION_LEAVE,
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

      WIDGET_UPDATE,

      MOUSE_DCLICK,
      MOUSE_LEFT_UP,
      DROP_FILE,
      WINDOW_FOCUS
    };

    UIEvent( void );

    virtual ~UIEvent( void );

    UIEvent( uint32 type );

    virtual UIEvent* clone( void ) const;

    virtual UIEvent* etype( void );

    uint32 type( void ) const;

    /// \brief Obtain the associated nom::Event object.
    ///
    /// \todo Rename method to sys_event?
    const Event& event( void ) const;

    void set_type( uint32 type );

    /// \brief Set the nom::Event object associated with this UI event.
    ///
    /// \todo Rename method to set_sys_event?
    void set_event( const Event& ev );

    bool operator <( const self_type& rhs ) const;

    bool operator ==( const self_type& rhs ) const;

  private:
    uint32 type_;

    /// \brief The nom::Event object associated with the UI event.
    Event event_;
};

} // namespace nom

#endif // include guard defined
