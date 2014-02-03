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
#ifndef NOMLIB_GUI_DATAVIEW_LIST_HPP
#define NOMLIB_GUI_DATAVIEW_LIST_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/graphics/sprite/SpriteBatch.hpp"
#include "nomlib/gui/UIWidget.hpp"
#include "nomlib/gui/DataViewColumn.hpp"
#include "nomlib/gui/DataViewListStore.hpp"

namespace nom {

/// \brief GUI data tree container widget
class DataViewList: public UIWidget
{
  public:
    typedef DataViewList SelfType;
    typedef SelfType* RawPtr;
    typedef std::unique_ptr<SelfType> UniquePtr;
    typedef std::shared_ptr<SelfType> SharedPtr;

    /// \brief Internal rendering list for column headers
    typedef std::vector<Text::unique_ptr> HeaderLabels;

    /// \brief Internal rendering list for items
    typedef std::vector<IDrawable::shared_ptr> ItemDrawables;

    /// \brief Default constructor.
    DataViewList( void );

    /// \brief Destructor.
    virtual ~DataViewList( void );

    DataViewList  (
                    UIWidget* parent,
                    int64 id,
                    const Point2i& pos,
                    const Size2i& size
                  );

    /// \brief Query the validity of the object
    ///
    /// \remarks A valid object must have both the positioning & size bounding
    /// coordinates be set to a non-null object value. See nom::Point2i::null
    /// and nom::Size2i::null for their respective values.
    bool valid( void ) const;

    /// \brief Obtain a reference to the object.
    const DataViewColumn& column( uint cols_id ) const;
    const DataViewListStore::ValueType item( uint cols_id, uint row_id  ) const;

    uint columns_size( void ) const;
    uint items_size( uint cols_id ) const;

    /// \Obtain the column names from the store.
    const DataViewListStore::ColumnNames column_names( void ) const;

    bool insert_column( uint cols_id, const DataViewColumn& col );

    bool append_column( const DataViewColumn& col );

    bool insert_item( uint cols_id, const DataViewListStore::ValueTypeContainer& labels );
    bool insert_item( uint cols_id, uint row, const DataViewListStore::ValueType& label );

    // FIXME:
    // bool append_item( const DataViewListStore::ValueTypeContainer& label );

    virtual void update_columns( void );
    virtual void update_items( void );

    /// \brief Implements IDrawable::draw
    void draw( RenderTarget& target ) const;

    /// \brief Implements the EventHandler::process_event method.
    bool process_event( const nom::Event& ev );

  private:
    /// \remarks Implements IDrawable::update
    virtual void update( void );

    /// \brief Rendered column headers
    HeaderLabels drawable_headers_;

    /// \brief Rendered items
    ItemDrawables drawable_items_;

    /// \brief Internal container for columns, rows.
    DataViewListStore::UniquePtr store_;

    bool updated_;
};

} // namespace nom

#endif // include guard defined
