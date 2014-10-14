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
#ifndef NOMLIB_GUI_UIQUESTION_DIALOG_BOX_HPP
#define NOMLIB_GUI_UIQUESTION_DIALOG_BOX_HPP

#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/UIMessageBox.hpp"

namespace nom {

/// \brief Simple question dialog model
///
/// \remarks This interface exists to ease the mapping & sync of keyboard, mouse
/// and joystick inputs.
class UIQuestionDialogBox: public UIMessageBox
{
  public:
    typedef UIQuestionDialogBox self_type;
    typedef self_type* raw_ptr;

    /// \brief Default constructor; initialize default element IDs.
    ///
    /// \see UIWidget::set_desktop, UIWidget::load_document_file,
    /// UIWidget::show.
    UIQuestionDialogBox();

    /// \brief Destructor.
    virtual ~UIQuestionDialogBox();

    /// \brief The ID selector used to search for the number of children; the
    /// response elements to index, i.e.: form tag ID.
    const std::string& response_id() const;

    /// \brief The element tag to use for indexing as a possible response
    /// choice (i.e.: button).
    const std::string& element_type() const;

    nom::size_type num_responses() const;

    /// \brief Obtain the current selection index (position) of the responses.
    ///
    /// \returns Index position of the selection; or negative one (-1) if
    /// nothing is selected.
    int selection() const;

    /// \brief Get the response element IDs.
    std::vector<std::string> responses() const;
    std::string response( int pos ) const;

    void set_response_id( const std::string& id );
    void set_element_type( const std::string& type );

    /// \brief Set the current selection index (position).
    void set_selection( int pos );

  private:
    std::string response_id_;
    std::string element_type_;

    /// \brief Selected item, identified by its index (position).
    int selection_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::UIQuestionDialogBox
/// \ingroup librocket
///
///         [DESCRIPTION STUB]
///
/// Usage example:
/// \code
///
/// #include <nomlib/librocket.hpp>
///
/// \endcode
///
