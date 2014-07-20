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

namespace nom {

/// \brief Pure abstract interface for an event listener.
///
/// \note Observer pattern
///
/// \see UIEventDispatcher
class IUIEventListener
{
  public:
    typedef IUIEventListener self_type;

    typedef UIEvent event_type;

    virtual ~IUIEventListener( void ) {}

    virtual void operator() ( event_type& ev ) const = 0;
};

/// \brief Widget event listener
///
/// \note Observer pattern
///
/// \see UIEventDispatcher
class UIWidgetListener: public IUIEventListener
{
  public:
    typedef UIWidgetListener self_type;

    typedef UIEvent event_type;
    typedef std::function<void(event_type*)> callback_type;

    UIWidgetListener( const callback_type& observer );

    virtual ~UIWidgetListener( void );

    void operator() ( event_type& ev ) const;

  private:
    std::function<void(event_type*)> cb_;
};

class IUIEventDispatcher
{
  public:
    typedef IUIEventDispatcher self_type;

    typedef UIEvent event_type;
    typedef IUIEventListener callback_type;

    virtual ~IUIEventDispatcher( void ) {}

    virtual bool register_event_listener( const event_type& ev, std::shared_ptr<callback_type> observer ) = 0;
    virtual bool remove_event_listener( const event_type& key ) = 0;
    virtual bool emit( const event_type& ev, UIEvent& ) const = 0;
    virtual uint32 size( void ) const = 0;
    virtual bool find( const event_type& key ) const = 0;
};

/// \brief Events manager
///
/// \remarks Note that duplicate events -- defined by the event_type -- are
/// *not* allowed, and no protection against this has been implemented.
///
/// \note This method is modeled after the Observer pattern's Subject class
/// interface.
class UIEventDispatcher: public IUIEventDispatcher
{
   public:
    /// \brief The object's class name.
    typedef UIEventDispatcher self_type;

    /// \brief A raw pointer to the object's instance.
    typedef UIEvent event_type;

    /// \brief The data container type of the callback.
    ///
    /// \remarks Delegate execution is done via C++'s object operator method.
    typedef IUIEventListener callback_type;

    /// \brief The data container type that holds a one-to-one mapping of
    /// registered event observers to its event identifier object.
    typedef std::map<event_type, std::vector<std::shared_ptr<callback_type>>> event_table;

    /// \brief Default constructor.
    UIEventDispatcher( void );

    /// \brief Destructor.
    virtual ~UIEventDispatcher( void );

    /// \brief Add an event observer (listener).
    ///
    /// \param ev The event object that identifies the observing object.
    /// \param observer The delegate (callback) object that is executed upon a
    /// call to ::emit.
    ///
    /// \returns Boolean TRUE upon a successful registration; boolean FALSE
    /// upon failure to register the event listener.
    bool register_event_listener( const event_type& ev, std::shared_ptr<callback_type> observer );

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
    bool emit( const event_type& ev, UIEvent& data ) const;

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
    const event_table& observers( void ) const;

    /// \brief Internal setter helper for observers.
    void set_observers( const event_table& ev_table );

    event_table observers_;
};

} // namespace nom

/// \brief Convenience macro for registering an event listener for emitted
/// nom::UIWidgetEvent signals.
///
/// \param obj  The nom::UIWidget deriving object that is calling from.
/// \param evt  The nom::UIEvent signal to listen for.
/// \param func The callback method to be executed.
///
/// \see See also: nom::UIEvent, nom::UIWidgetEvent.
#define NOM_CONNECT_UIWIDGET_EVENT( obj, evt, func ) \
  ( obj->dispatcher()->register_event_listener( evt, std::make_shared<nom::UIWidgetListener>( [&] ( nom::UIEvent* ev ) { func( *static_cast<nom::UIWidgetEvent*>( ev ) ); } ) ) )

/// \brief Convenience macro for registering an event listener for emitted
/// nom::UIWidgetResizeEvent signals.
///
/// \param obj  The nom::UIWidget deriving object that is calling from.
/// \param evt  The nom::UIEvent signal to listen for.
/// \param func The callback method to be executed.
///
/// \see See also: nom::UIEvent, nom::UIWidgetResizeEvent.
#define NOM_CONNECT_RESIZE_EVENT( obj, evt, func ) \
  ( obj->dispatcher()->register_event_listener( evt, std::make_shared<UIWidgetListener>( [&] ( UIEvent* ev ) { func( *static_cast<nom::UIWidgetResizeEvent*>( ev ) ); } ) ) )

/// \brief Convenience macro for registering an event listener for emitted
/// nom::UIWidgetTreeEvent signals.
///
/// \param obj  The nom::UIWidget deriving object that is calling from.
/// \param evt  The nom::UIEvent signal to listen for.
/// \param func The callback method to be executed.
///
/// \see See also: nom::UIEvent, nom::UIWidgetTreeEvent.
#define NOM_CONNECT_UIWIDGET_TREE_EVENT( obj, evt, func ) \
  ( obj->dispatcher()->register_event_listener( evt, std::make_shared<nom::UIWidgetListener>( [&] ( nom::UIEvent* ev ) { func( *static_cast<nom::UIWidgetTreeEvent*>( ev ) ); } ) ) )

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
/// 5. http://stackoverflow.com/questions/14633808/the-observer-pattern-further-considerations-and-generalised-c-implementation
///
