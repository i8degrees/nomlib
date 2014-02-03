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
#ifndef NOMLIB_GUI_UI_WIDGET_EVENT_HPP
#define NOMLIB_GUI_UI_WIDGET_EVENT_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/Event.hpp"

// Temporary: remove me when resized_bounds_ goes away
#include "nomlib/math/Rect.hpp"

namespace nom {

/// \brief Container class for user interface events generated from widgets.
class UIWidgetEvent
{
  public:
    typedef UIWidgetEvent self_type;
    typedef self_type* RawPtr;

    /// \brief Default constructor.
    UIWidgetEvent( void );

    /// \brief Destructor.
    ~UIWidgetEvent( void );

    /// \brief Copy constructor.
    UIWidgetEvent( const self_type& copy );

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& other );

    /// \brief Construct a minimally valid object.
    ///
    /// \param ev The nom::Event associated with the widget event.
    ///
    /// \remarks The index and id fields will be initialized to negative one
    /// (-1).
    UIWidgetEvent( const Event& ev );

    /// \brief Construct a valid object.
    ///
    /// \param index  The widget-defined index associated with the widget
    ///               event.
    /// \param text   The widget-defined string associated with the widget
    ///               event.
    /// \param ev     The nom::Event associated with the widget event.
    ///
    /// \remarks The id field will be initialized to negative one (-1).
    UIWidgetEvent( sint index, const std::string& text, const Event& ev );

    /// \brief Construct a complete object.
    ///
    /// \param index  The widget-defined index associated with the widget
    ///               event.
    /// \param text   The widget-defined string associated with the widget
    ///               event.
    /// \param ev     The nom::Event associated with the widget event.
    /// \param id     The widget's unique identifier associated with the widget
    ///               event.
    UIWidgetEvent (
                    sint index,
                    const std::string& text,
                    const Event& ev,
                    int64 id
                  );

    /// \brief Obtain a pointer to the object.
    RawPtr get( void );

    /// \brief Query the validity of the object.
    bool valid( void ) const;

    sint index( void ) const;
    const std::string& text( void ) const;

    /// \brief Obtain the associated nom::Event object.
    const Event& event( void ) const;

    /// \brief Get the unique identifier associated with the widget.
    int64 id( void ) const;

    void set_index( int idx );
    void set_text( const std::string& text );

    /// \brief Set the nom::Event object associated with this UI event.
    void set_event( const Event& ev );

    /// \brief Set the source event's widget identifier.
    void set_id( int64 id );

  private:
    sint index_;
    std::string text_;

    /// \brief The nom::Event object associated with the UI event.
    Event event_;

    /// \brief Unique identifier of the callback object, identified by its
    /// nom::UIWidget id.
    int64 id_;

  // Temporary: until we get around to refactoring events subsystem
  public:
    IntRect resized_bounds_;
};

} // namespace nom

#endif // include guard defined
