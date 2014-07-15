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
#ifndef NOMLIB_GUI_DATAVIEW_COLUMN_HPP
#define NOMLIB_GUI_DATAVIEW_COLUMN_HPP

#include "nomlib/config.hpp"
#include "nomlib/gui/IDataViewColumn.hpp"

namespace nom {

// Forward declarations
class UIStyle;

/// \brief Data column container for DataViewList
class DataViewColumn: public IDataViewColumn
{
  public:
    typedef DataViewColumn SelfType;

    typedef SelfType* RawPtr;
    typedef std::unique_ptr<SelfType> UniquePtr;
    typedef std::shared_ptr<SelfType> SharedPtr;

    DataViewColumn( void );
    virtual ~DataViewColumn( void );

    DataViewColumn( uint id, const std::string& name, int width, enum IDataViewColumn::Alignment align );

    uint id( void ) const;

    const std::string& title( void ) const;

    int width( void ) const;

    IDataViewColumn::Alignment alignment( void ) const;

    bool sortable( void ) const;

    bool hidden( void ) const;

    /// \brief Get the style for the column.
    ///
    /// \returns A non-NULL UIStyle object if the end-user has set one, or NULL
    /// if it has not been set.
    ///
    /// \see ::set_style
    std::shared_ptr<UIStyle> style( void ) const;

    /// \brief Set the style for the column.
    ///
    /// \note This is optional, and must be set the end-user.
    void set_style( std::shared_ptr<UIStyle> style );

  private:
    /// \brief Column identifier; zero-based index.
    uint id_;

    /// \brief Column name
    std::string title_;

    /// \brief Minimal column width
    ///
    /// \remarks Not used; reserved for future implementation.
    int min_width_;

    /// \brief Maximum column width.
    int width_;

    /// \brief Column alignment.
    enum IDataViewColumn::Alignment alignment_;

    /// \brief Sortable data flag.
    ///
    /// \remarks Not used; reserved for future implementation.
    bool sortable_;

    /// \brief Toggle column data to be shown or hidden.
    bool visibility_;

    /// \brief Customizable column theme; this is intended to be set by the
    /// end-user. This must *not* depend on being available.
    std::shared_ptr<UIStyle> style_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::DataViewColumn
/// \ingroup gui
///
