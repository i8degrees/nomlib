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

#include <vector>
#include <map>
#include <memory>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class UIStyle;
class DataViewColumn;
class DataViewItem;
class DataViewTextItem;
class DataViewDrawableItem;

// class IDrawable;
class Text;
class SpriteBatch;

/// \brief Data container for DataViewList
class DataViewListStore
{
  public:
    typedef DataViewListStore self_type;
    typedef std::unique_ptr<DataViewListStore> unique_ptr;
    typedef std::unique_ptr<DataViewListStore> shared_ptr;

    /// \brief Item container; one-to-one relationship with a column;
    /// indexed by column ID.
    typedef std::vector<std::shared_ptr<DataViewItem>> ValueTypeContainer;

    typedef std::map<DataViewColumn, ValueTypeContainer> Items;
    typedef std::pair<DataViewColumn, ValueTypeContainer> ItemPair;

    /// \brief Return type for column name getter(s).
    typedef std::vector<std::string> ColumnNames;

    /// \brief Default constructor
    DataViewListStore( void );

    /// \brief Destructor.
    virtual ~DataViewListStore( void );

    /// \brief Get an object pointer to the column.
    ///
    /// \returns A non-owned object pointer to the column.
    const DataViewColumn& column( uint col ) const;

    /// \brief Get an object pointer to the item.
    ///
    /// \returns A non-owned object pointer to the item.
    DataViewItem* item( uint col, uint pos ) const;
    Text* item_text( uint col, uint pos ) const;
    SpriteBatch* item_sprite( uint col, uint pos ) const;

    uint columns_size( void ) const;
    uint items_size( uint cols_id ) const;

    /// \Obtain the column names from the store.
    const ColumnNames column_names( void ) const;

    /// \brief Erase stored columns.
    void clear_columns( void );

    /// \brief Erase all the columns from the store.
    void clear_items( uint col );

    /// \brief Erase an item from the store.
    bool erase_item( uint col, uint pos );

    /// \fixme
    void insert_column( uint cols, const DataViewColumn& col );

    void append_column( const DataViewColumn& col );

    bool insert_text_item( uint col, uint pos, const DataViewTextItem& item );
    bool append_text_item( uint col, const DataViewTextItem& item );

    bool insert_bitmap_item( uint col, uint pos, const DataViewDrawableItem& item );
    bool append_bitmap_item( uint col, const DataViewDrawableItem& item );

    bool insert_item( uint col, uint pos, DataViewItem* value );

    /// \brief Add an item onto the end of a column.
    ///
    /// \param col    The column identifier.
    /// \param value  An DataViewItem compatible object to insert.
    bool append_item( uint col, DataViewItem* value );

    /// \todo Consider removing method?
    void insert_items( uint col, const ValueTypeContainer& value );

    /// \brief Apply a custom style for an item within a column.
    ///
    /// \param col    The column identifier.
    /// \param pos    The pos identifier.
    /// \param style  The UIStyle object to apply.
    bool set_item_style( uint col, uint pos, std::shared_ptr<UIStyle> style );

    /// \brief Apply a custom style for every item within a column.
    ///
    /// \param col    The column identifier.
    /// \param style  The UIStyle object to apply.
    bool set_items_style( uint col, std::shared_ptr<UIStyle> style );

    // void set_items_selection( uint col, int value );

  private:
    /// \brief Storage items; one column to many items relationship
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
