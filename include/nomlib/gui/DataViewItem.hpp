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
#ifndef NOMLIB_GUI_DATAVIEW_ITEM_HPP
#define NOMLIB_GUI_DATAVIEW_ITEM_HPP

#include <memory>

#include "nomlib/config.hpp"

#include "nomlib/system/IObject.hpp"

namespace nom {

// Forward declarations
class UIStyle;
class String;

class IDataViewItem: public IObject
{
  public:
    IDataViewItem( void );

    virtual ~IDataViewItem( void );

    virtual ObjectTypeInfo type( void ) const = 0;
    virtual IObject* data( void ) const = 0;

    /// \brief Get the style for the item.
    ///
    /// \returns A non-NULL UIStyle object if the end-user has set one, or NULL
    /// if it has not been set.
    ///
    /// \see ::set_style
    std::shared_ptr<UIStyle> style( void ) const;

    /// \brief Set the style for the item.
    ///
    /// \remarks This is optional, and must be set the end-user.
    void set_style( std::shared_ptr<UIStyle> style );

  private:
    /// \brief Customizable column theme; this is intended to be set by the
    /// end-user. This must *not* depend on being available.
    std::shared_ptr<UIStyle> style_;
};

class DataViewTextItem: public IDataViewItem
{
  public:
    typedef DataViewTextItem self_type;

    DataViewTextItem( void );

    virtual ~DataViewTextItem( void );

    DataViewTextItem( const std::string& data );

    ObjectTypeInfo type( void ) const;

    IObject* data( void ) const;

  private:
    String* data_;
};

class DataViewItem: public IDataViewItem
{
  public:
    typedef DataViewItem self_type;

    DataViewItem( void );

    virtual ~DataViewItem( void );

    DataViewItem( IObject* data );

    ObjectTypeInfo type( void ) const;

    IObject* data( void ) const;

  private:
    IObject* data_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::DataViewItem
/// \ingroup gui
///
