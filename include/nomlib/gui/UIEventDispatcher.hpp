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
#ifndef NOMLIB_GUI_UI_EVENT_DISPATCHER_HPP
#define NOMLIB_GUI_UI_EVENT_DISPATCHER_HPP

#include <memory>
#include <functional>
#include <vector>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"
#include "nomlib/gui/UIEventCallback.hpp"

namespace nom {

/*
struct EventType
{
  public:
    EventType( void ) {}
    ~EventType( void ) {}

    EventType( enum UIEvent type )
    {
      this->type_ = type;
      this->id_ = -1;
    }

    EventType( enum UIEvent type, int64 id )
    {
      this->type_ = type;
      this->id_ = id;
    }

    bool operator <( const EventType& rhs ) const
    {
      // return( this->type_ < rhs.type_ );
      return( this->type_ < rhs.type_ );
    }

    bool operator ==( const EventType& rhs ) const
    {
      // return( this->type_ == rhs.type_ );
      return( this->id_ == rhs.id_ );
    }

  private:
    int64 id_;
    enum UIEvent type_;
    // UIEventCallback* delegate_;
};
*/

/// \brief Events manager
///
/// \remarks Note that duplicate events -- defined by the event_type -- are
/// *not* allowed, and no protection against this has been implemented.
///
/// \note This method is modeled after the Observer pattern's Subject class
/// interface.
class UIEventDispatcher
{
   public:
    /// \brief The object's class name.
    typedef UIEventDispatcher self_type;

    /// \brief A raw pointer to the object's instance.
    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    /// \brief A raw pointer to the object's instance.
    typedef UIEvent event_type;

    /// \brief The data container type of the callback.
    ///
    /// \remarks Delegate execution is done via C++'s object operator method.
    typedef UIEventCallback callback_type;

    /// \brief The data container type that holds a one-to-one mapping of
    /// registered event observers to its event identifier object.
    typedef std::map<event_type, std::vector<callback_type>> CallbackTable;

    /// \brief Default constructor.
    UIEventDispatcher( void );

    /// \brief Destructor.
    ~UIEventDispatcher( void );

    /// \brief Copy constructor.
    UIEventDispatcher( const self_type& rhs );

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& rhs );

    /// \brief Add an event observer (listener).
    ///
    /// \param ev The event object that identifies the observing object.
    /// \param observer The delegate (callback) object that is executed upon a
    /// call to ::emit.
    ///
    /// \returns Boolean TRUE upon a successful registration; boolean FALSE
    /// upon failure to register the event listener.
    bool register_event_listener( const event_type& ev, const callback_type& observer );

    /// \brief Remove an event listener.
    ///
    /// \param key The event identifier that references the event observer
    /// object.
    ///
    /// \returns Boolean TRUE upon a successful removal; boolean FALSE upon
    /// failure to remove the event listener.
    bool remove_event_listener( const event_type& key );

    /// \brief Obtain the total count of the number of registered observers.
    uint32 size( void ) const;

    /// \note This method is modeled after the Observer pattern's notify or
    /// update method.
    bool emit( const event_type& ev, UIWidgetEvent& data ) const;

    /// \brief Search the list of registered observers for a particular event
    /// type.
    ///
    /// \param key The event identifier that references the event observer
    /// object.
    ///
    /// \returns Boolean TRUE upon one or more matched observers; boolean FALSE
    /// upon failure to find less than one matched observers.
    bool find( const event_type& key ) const;

  private:
    /// \brief Internal getter helper for observers.
    const CallbackTable& observers( void ) const;

    /// \brief Internal setter helper for observers.
    void set_observers( const CallbackTable& ev_table );

    CallbackTable observers_;
};

} // namespace nom

/// \brief Convenience macro for binding UI widget events.
///
/// \param obj  The nom::UIWidget deriving object that is calling from.
/// \param evt  The nom::UIEvent type to register.
/// \param func The callback method to be executed.
///
/// \remarks See also: nom::UIEvent, nom::UIWidgetEvent.
#define NOM_CONNECT_UIEVENT( obj, evt, func ) \
  ( obj->dispatcher()->register_event_listener( evt, nom::UIEventCallback( [&] ( const nom::UIWidgetEvent& ev ) { func( ev ); } ) ) )

#endif // include guard defined

/// \class nom::UIEventDispatcher
/// \ingroup gui
///
///   [TODO: DESCRIPTION]
///
/// References (class design, ideas, inspiration):
///
/// 1. http://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
/// 2. http://sourcemaking.com/design_patterns/observer/cpp/1
/// 3. http://docs.wxwidgets.org/trunk/classwx_command_event.html
/// 4. http://librocket.com/wiki/documentation/tutorials/Dragging
///
