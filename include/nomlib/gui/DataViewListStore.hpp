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
#ifndef NOMLIB_GUI_DATAVIEW_LIST_STORE_HPP
#define NOMLIB_GUI_DATAVIEW_LIST_STORE_HPP

#include <map>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/ptree.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/gui/DataViewColumn.hpp"
#include "nomlib/gui/DataViewItem.hpp"

namespace nom {

// Forward declarations
class UIStyle;

/// \brief Data container for DataViewList
class DataViewListStore
{
  public:
    typedef DataViewListStore SelfType;
    typedef SelfType* RawPtr;

    typedef std::unique_ptr<SelfType> UniquePtr;
    typedef std::shared_ptr<SelfType> SharedPtr;

    /// \brief Internal column object store
    typedef std::vector<DataViewColumn::UniquePtr> Columns;

    /// \brief Item container; one-to-one relationship with a column;
    /// indexed by column ID.
    typedef IDataViewItem ValueType;
    typedef std::vector<ValueType*> ValueTypeContainer;

    typedef std::map<uint, ValueTypeContainer> Items;
    typedef std::pair<uint, ValueTypeContainer> ItemPair;

    /// \brief Return type for column name getter(s).
    typedef std::vector<std::string> ColumnNames;

    /// \brief Default constructor
    DataViewListStore( void );

    /// \brief Destructor.
    virtual ~DataViewListStore( void );

    /// \brief Obtain a reference to the object.
    const DataViewColumn& column( uint cols_id ) const;
    ValueType* item( uint cols_id, uint row_id  ) const;

    uint columns_size( void ) const;
    uint items_size( uint cols_id ) const;

    uint items_size( void ) const;

    /// \Obtain the column names from the store.
    const ColumnNames column_names( void ) const;

    void clear( void );

    // bool erase_item( uint col, IDataViewItem* item );

    bool insert_column( uint cols, const DataViewColumn& col );

    bool append_column( const DataViewColumn& col );

    bool insert_item( uint cols_id, const ValueTypeContainer& value );

    bool insert_item( uint cols_id, uint row_id, ValueType* value );

    // bool insert_item( uint cols_id, const DrawablesContainer& values );
    // bool insert_item( uint cols_id, uint row_id, const IDrawable& value );

    /// \todo Fix implementation
    bool append_item( const ValueTypeContainer& value );

    /// \brief Add an item onto the end of a column.
    ///
    /// \param col The column identifier.
    /// \param value An IDataViewItem compatible object to insert.
    bool append_item( uint col, ValueType* value );

    /// \brief Apply a custom style for every item within the column.
    ///
    /// \param col The column identifier.
    /// \param style The UIStyle object to apply.
    bool set_item_style( uint col, std::shared_ptr<UIStyle> style );

  private:
    /// \brief Internal columns objects; this is used to render from.
    Columns cols_;

    /// \brief Internal item objects; this is used to render from.
    Items items_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::DataViewListStore
/// \ingroup gui
///
///         [DESCRIPTION STUB]
///
/// \todo Implement loading and saving of data contained in this object.
