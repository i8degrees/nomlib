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

    /// \todo Rename to Signal?
    enum
    {
      UIEVENT = 0,

      // Widget event signals

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

      /// The event emitted when the widget's position has been modified.
      ///
      /// This event type models SDL_WINDOWEVENT_MOVED for the sake of
      /// consistency.
      WINDOW_MOVED,

      /// The event emitted when the widget's size has been modified.
      ///
      /// ~~This event is always preceded by WINDOW_SIZE_CHANGED.~~
      ///
      /// ~~This event type models SDL_WINDOWEVENT_RESIZED for the sake of
      /// consistency.~~
      WINDOW_RESIZED,

      /// The emitted event when the widget's size fields has been modified,
      /// either as the result of the underlying system (us, i.e.: layout update)
      /// or by the end-user.
      ///
      /// ~~This event is followed by WINDOW_RESIZED when the size was modified
      /// by an external event, i.e.: end-user or the underlying system (us,
      /// i.e.: layout).~~
      ///
      /// ~~This event type models SDL_WINDOWEVENT_SIZE_CHANGED for the sake of
      /// consistency.~~
      WINDOW_SIZE_CHANGED,

      /// The event emitted when the underlying system (us) requests that the
      /// widget's window be closed.
      WINDOW_CLOSE,

      WIDGET_UPDATE,

      MOUSE_DCLICK,
      MOUSE_LEFT_UP,
      DROP_FILE,
      WINDOW_FOCUS
    };

    /// \brief Default constructor.
    ///
    /// \remarks The event is initialized to invalid parameters.
    ///
    /// \see UIEvent::set_type, UIEvent::set_id.
    UIEvent( void );

    /// \brief Destructor.
    virtual ~UIEvent( void );

    /// \brief Construct an event from a defined type.
    ///
    /// \internal
    ///
    /// \note This constructor is required for usage with UIEventDispatcher
    /// (including its macros).
    ///
    /// \endinternal
    UIEvent( uint32 type );

    /// \brief Create a unique copy of the object.
    virtual UIEvent* clone( void ) const;

    /// \brief Get the type of event for the object.
    uint32 type( void ) const;

    /// \brief Obtain the associated nom::Event object.
    ///
    /// \remarks The UIEvent is not required to set this field. You should
    /// always ensure that the type field of the nom::Event struct is valid
    /// before using data from it.
    ///
    /// \see nom::EventHandler, nom::Event
    ///
    /// \todo Rename method to sys_event?
    const Event& event( void ) const;

    /// \brief Get the unique identifier associated with the widget.
    int64 id( void ) const;

    /// \brief Set the type of event of the object.
    void set_type( uint32 type );

    /// \brief Set the underlying nom::Event object associated with this event.
    ///
    /// \todo Rename method to set_sys_event?
    void set_event( const Event& ev );

    /// \brief Set the source event's widget identifier.
    void set_id( int64 id );

    /// \brief Less than comparison operator.
    ///
    /// \remarks This is required for implementation inside a std::map.
    ///
    /// \see nom::UIEventDispatcher
    bool operator <( const self_type& rhs ) const;

    /// \brief Equality comparison operator.
    ///
    /// \remarks This is required for implementation inside a std::map.
    ///
    /// \see nom::UIEventDispatcher
    bool operator ==( const self_type& rhs ) const;

  private:
    /// \brief Event type.
    ///
    /// \see UIEvent anonymous enum.
    uint32 type_;

    /// \brief The underlying nom::Event object associated with the event.
    Event event_;

    /// \brief Unique identifier of the callback object, identified by its
    /// nom::UIWidget id.
    int64 id_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::UIEvent
/// \ingroup gui
///
/// \code
/// void YourWidget::process_event( UIEvent* ev )
/// {
///   UIWidgetEvent* event = dynamic_cast( UIWidgetEvent*, ev );
///   if( event != nullptr )
///   {
///     // Do event logic
///   }
/// }
/// \endcode
