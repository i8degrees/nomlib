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
#ifndef NOMLIB_LIBROCKET_UIDATAVIEW_LIST_HPP
#define NOMLIB_LIBROCKET_UIDATAVIEW_LIST_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/librocket/UIWidget.hpp"

// Forward declarations (third-party)
namespace Rocket {
namespace Controls {



} // namespace Controls
} // namespace Rocket

namespace nom {

/// \see http://librocket.com/wiki/documentation/tutorials/Datagrid
///
/// \todo Consider renaming class to UIDataViewPagedList or alike..?
class UIDataViewList: public nom::UIWidget
{
  public:
    /// \brief Default constructor.
    ///
    /// \see UIWidget::set_desktop, UIWidget::set_document_file,
    /// UIDataViewList::initialize.
    UIDataViewList();

    UIDataViewList( const Point2i& pos, const Size2i& dims );

    virtual ~UIDataViewList();

    virtual bool initialize();

    std::string column_title( int col ) const;

    bool set_column_title( int col, const std::string& name );

    /// \fixme
    int selection() const;

    /// \fixme
    void set_selection( int idx );

  private:
    /// \brief The currently selected card.
    ///
    /// \remarks This maps internally to the element's index position in
    /// the active cards database.
    int selection_;
};

} // namespace nom

#endif // include guard defined
