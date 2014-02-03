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
#ifndef NOMLIB_GUI_UI_EVENT_HANDLER_HPP
#define NOMLIB_GUI_UI_EVENT_HANDLER_HPP

#include <memory>
#include <vector>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gui/UIEventDispatcher.hpp"

namespace nom {

/// \brief Top-level event manager -- every nom::UIWidget derives from this
/// base class.
///
/// \remarks This class interface aliases much of the nom::UIEventDispatcher
/// class in order to ensure things stay decoupled and re-usable.
///
/// \todo We derive from Transformable -- this is intended for use by
/// nom::UIWidget and derived friends (so we do not have to use multiple
/// inheritance). nom::UIEventHandler has no use for this base class whatsoever.
class UIEventHandler: public Transformable
{
  public:
    typedef UIEventHandler self_type;
    typedef Transformable derived_class;
    typedef self_type* raw_ptr;

    /// \brief Default constructor; initialize the events managing object to a
    /// sane state.
    UIEventHandler( void );

    /// \brief Destructor.
    virtual ~UIEventHandler( void );

    /// \brief Copy constructor.
    UIEventHandler( const self_type& rhs );

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& rhs );

    /// \brief Construct an object with initializing the event dispatcher object
    /// to a specified pointer.
    UIEventHandler( const UIEventDispatcher::raw_ptr dispatcher );

    /// \brief Validity of object state checks.
    ///
    /// \returns Boolean TRUE if event dispatcher pointer is non-null;
    /// boolean FALSE if the event dispatcher pointer is null.
    bool valid( void ) const;

    /// \brief Obtain a pointer to the event dispatching object in use.
    ///
    /// \remarks An assert will be triggered upon detecting a null dispatcher
    /// object.
    UIEventDispatcher::raw_ptr dispatcher( void ) const;

    /// \brief Add an event observer (listener).
    ///
    /// \param ev The event object that identifies the observing object.
    /// \param observer The delegate (callback) object that is executed upon a
    /// call to ::emit.
    ///
    /// \returns Boolean TRUE upon a successful registration; boolean FALSE
    /// upon failure to register the event listener.
    bool register_event_listener( const UIEventDispatcher::event_type& ev, const UIEventDispatcher::callback_type& observer );

    /// \brief Remove an event listener.
    ///
    /// \param key The event identifier that references the event observer
    /// object.
    ///
    /// \returns Boolean TRUE upon a successful removal; boolean FALSE upon
    /// failure to remove the event listener.
    bool remove_event_listener( const UIEventDispatcher::event_type& key );

    /// \brief Obtain the total size of the number of registered observers.
    uint32 count( void ) const;

    bool emit( const UIEventDispatcher::event_type& ev, UIWidgetEvent& data ) const;

    /// \brief Search the list of registered observers for a particular event
    /// type.
    ///
    /// \param key The event identifier that references the event observer
    /// object.
    ///
    /// \returns Boolean TRUE upon one or more matched observers; boolean FALSE
    /// upon failure to find less than one matched observers.
    bool find( const UIEventDispatcher::event_type& key ) const;

    /// \brief This method is intentionally NO-OP and intended to be overridden
    /// by the end-user (developer) as necessary.
    ///
    /// \remarks Note that you should not be inheriting directly from this class,
    /// but instead from nom::UIWidget.
    ///
    /// \returns Boolean TRUE when all event(s) have been processed; boolean
    /// FALSE when event(s) have not been processed.
    virtual bool process_event( const Event& ev );

  private:
    /// \brief Set the event dispatcher to use.
    void set_dispatcher( const UIEventDispatcher::raw_ptr dispatcher );

    /// \brief A pointer to our events managing object.
    UIEventDispatcher::unique_ptr dispatcher_;
};

} // namespace nom

#endif // include guard defined
