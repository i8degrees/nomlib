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
#ifndef NOMLIB_GUI_QUESTION_DIALOG_BOX_HPP
#define NOMLIB_GUI_QUESTION_DIALOG_BOX_HPP

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/MessageBox.hpp"

namespace nom {

// Forward declarations
class UIEvent;

/// \brief Question dialog box widget
class QuestionDialogBox: public MessageBox
{
  public:
    typedef QuestionDialogBox self_type;
    typedef MessageBox derived_class;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    typedef std::vector<std::string> ItemStrings;

    typedef std::vector<Text> ChoiceLabels;

    QuestionDialogBox (
                        UIWidget* parent,
                        int64 id,
                        const Point2i& pos,
                        const Size2i& size
                      );

    virtual ~QuestionDialogBox( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    QuestionDialogBox( const self_type& rhs ) = delete;

    /// \brief Perform a bounds coordinates collision test.
    int hit_test( const Point2i& pt );

    /// \brief Implements MessageBox::draw method.
    void draw( RenderTarget& target ) const;

    /// \brief Obtain the current selected index identifier (position) of the
    /// item labels group.
    ///
    /// \returns Index position identifier of the selection; negative one (-1)
    /// if nothing is selected.
    int selection( void ) const;

    /// \todo Rename method?
    uint items_size( void ) const;

    /// \todo Rename to items?
    ItemStrings choices( void ) const;

    /// \todo Rename to item?
    std::string choice( int pos ) const;

    const Color4i& selected_text_color( void ) const;

    /// \todo Rename to append_item?
    void append_choice( const std::string& label );

    /// \brief Set the text color used to highlight the current selection
    /// item.
    void set_selected_text_color( const Color4i& color );

    /// \brief Set the current selection index (position) of the item labels
    /// group.
    void set_selection( int pos );

  protected:
    /// \brief Re-implements UIWidget::on_mouse_down.
    virtual void on_mouse_down( const Event& evt );

    /// \brief Re-implements UIWidget::on_mouse_wheel.
    virtual void on_mouse_wheel( const Event& evt );

    /// \brief Re-implements UIWidget::on_key_down.
    virtual void on_key_down( const Event& evt );

  private:
    /// \brief Implements MessageBox::update method.
    void update( void );

    ChoiceLabels choices_;

    /// \brief Selected item, identified by its index (position) from labels_
    /// container.
    ///
    /// \remarks The default state is to be non-selected (negative value).
    int selection_;

    /// \brief The text color used to highlight the selection text label.
    Color4i selected_text_color_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::QuestionDialogBox
/// \ingroup gui
///
///         [DESCRIPTION STUB]
///
/// Usage example:
/// \code
///
/// #include <nomlib/gui.hpp>
///
/// \endcode
///
