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
#include "nomlib/gui/UIEvent.hpp"

#include "nomlib/math/Rect.hpp" // nom::UIWidgetResizeEvent

namespace nom {

/// \brief Container class for user interface events generated from widgets.
class UIWidgetEvent: public UIEvent
{
  public:
    typedef UIWidgetEvent self_type;
    typedef self_type* RawPtr;

    /// \brief Default constructor.
    UIWidgetEvent( void );

    /// \brief Destructor.
    virtual ~UIWidgetEvent( void );

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
    /// \param id     The widget's unique identifier associated with the event.
    ///
    /// \remarks The id field will be initialized to negative one (-1).
    UIWidgetEvent( sint index, const std::string& text, int64 id );

    /// \brief Construct a complete object.
    ///
    /// \param index  The widget-defined index associated with the widget
    ///               event.
    /// \param text   The widget-defined string associated with the widget
    ///               event.
    /// \param ev     The nom::Event associated with the widget event.
    /// \param id     The widget's unique identifier associated with the event.
    UIWidgetEvent (
                    sint index,
                    const std::string& text,
                    const Event& ev,
                    int64 id
                  );

    sint index( void ) const;
    const std::string& text( void ) const;

    void set_index( int idx );
    void set_text( const std::string& text );

  private:
    sint index_;
    std::string text_;
};

// Forward declarations
class IObject;

class UIWidgetTreeEvent: public UIEvent
{
  public:
    UIWidgetTreeEvent( void ) :
      obj_{ nullptr }
    {
      // NOM_LOG_TRACE( NOM );
    }
    virtual ~UIWidgetTreeEvent( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    int column( void ) const
    {
      return this->column_;
    }

    IObject* data( void ) const
    {
      return this->obj_;
    }

    void set_column( int col )
    {
      this->column_ = col;
    }

    void set_data( IObject* obj )
    {
      this->obj_ = obj;
    }

  private:
    int column_;
    IObject* obj_;
};

// class UIWidgetResizeEvent: public UIEvent
class UIWidgetResizeEvent: public UIWidgetEvent
{
  public:
    UIWidgetResizeEvent( void );

    virtual ~UIWidgetResizeEvent( void );

    const IntRect& bounds( void ) const;

    void set_bounds( const IntRect& bounds );

  private:
    IntRect bounds_;
};

} // namespace nom

#endif // include guard defined
