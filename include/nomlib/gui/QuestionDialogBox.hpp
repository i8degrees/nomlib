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

/// \brief Question dialog box widget
class QuestionDialogBox: public MessageBox
{
  public:
    typedef QuestionDialogBox self_type;
    typedef MessageBox derived_class;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    typedef std::vector<std::string> ItemStrings;
    typedef std::vector<Text::unique_ptr> ItemLabels;

    // typedef std::vector<IDrawable::unique_ptr> ChoiceLabels;
    typedef std::vector<Text::unique_ptr> ChoiceLabels;

    QuestionDialogBox( void );

    virtual ~QuestionDialogBox( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    QuestionDialogBox( const self_type& rhs ) = delete;

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& rhs );

    QuestionDialogBox (
                        UIWidget* parent,
                        int64 id,
                        const Point2i& pos,
                        const Size2i& size
                      );

    // IDrawable::raw_ptr clone( void ) const;

    /// \brief Implements MessageBox::draw method.
    void draw( RenderTarget& target ) const;

    /// \brief Implements the MessageBox::process_event method.
    bool process_event( const nom::Event& ev );

    /// \brief Obtain the current selected index identifier (position) of the
    /// item labels group.
    ///
    /// \returns Index position identifier of the selection; negative one (-1)
    /// if nothing is selected.
    int selection( void ) const;

    /// \todo Rename method?
    uint items_size( void ) const;

    ItemStrings choices( void ) const;

    void append_choice( const Text& label );

    /// \brief Set the current selection index (position) of the item labels
    /// group.
    void set_selection( int pos );

  protected:
    // void copy( const self_type& rhs );

    /// \brief Implements MessageBox::update method.
    void update( void );

  private:
    ChoiceLabels choices_;

    /// \brief Selected item, identified by its index (position) from labels_
    /// container.
    ///
    /// \remarks The default state is to be non-selected (negative value).
    int selection_;

    // bool enabled_;
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
