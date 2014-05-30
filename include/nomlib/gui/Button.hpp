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
#ifndef NOMLIB_GUI_BUTTON_HPP
#define NOMLIB_GUI_BUTTON_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

/// \brief Basic GUI button container widget.
class Button: public UIWidget
{
  public:
    typedef Button self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    Button  (
              UIWidget* parent,
              int64 id,
              const Point2i& pos,
              const Size2i& size
            );

    /// \brief Destructor.
    virtual ~Button( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    Button( const self_type& copy ) = delete;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Get the preferred size of the widget.
    ///
    /// \note Re-implements UIWidget::size_hint.
    virtual const Size2i size_hint( void ) const;

    // \brief Query the validity of the object
    //
    // \remarks A valid object must have both the positioning & size bounding
    // coordinates be set to a non-null object value. See nom::Point2i::null
    // and nom::Size2i::null for their respective values.
    // virtual bool valid( void ) const;

    /// \brief Re-implements IDrawable::update.
    virtual void update( void );

    /// \brief Re-implements IDrawable::draw.
    virtual void draw( RenderTarget& target ) const;

    /// \brief Implements the EventHandler::process_event method.
    bool process_event( const nom::Event& ev );

    const std::string& label_text( void ) const;

    void set_label( const std::string& text );

  protected:
    /// \brief Re-implements UIWidget::on_size_changed.
    virtual void on_size_changed( const UIWidgetEvent& ev );

  private:
    std::string text_;
    Text label_;
};

} // namespace nom

#endif // include guard defined
