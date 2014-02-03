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
#ifndef NOMLIB_GUI_UI_ITEM_CONTAINER_HPP
#define NOMLIB_GUI_UI_ITEM_CONTAINER_HPP

#include <string>
#include <memory>
#include <vector>
#include <initializer_list>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"

namespace nom {

/// \brief Data storage container for a widget's item.
///
/// \remarks Note that this storage container is zero-based indexed.
class UIItemContainer
{
  public:
    typedef UIItemContainer self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    UIItemContainer( void );

    /// \brief Destructor.
    ~UIItemContainer( void );

    /// \brief Copy constructor.
    UIItemContainer( const self_type& rhs );

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& rhs );

    /// \brief Obtain a copy of the object.
    ///
    /// \returns A new instance of UIItemContainer, initialized with the
    /// contents of the existing object.
    UIItemContainer::raw_ptr clone( void ) const;

    /// \brief Obtain a reference to the object.
    const self_type& get( void ) const;

    /// \brief Obtain the selection (selected) item.
    ///
    /// \returns The item's stored index on success, or nom::npos on failure.
    int selection( void ) const;

    /// \brief Obtain the set string at the given index.
    ///
    /// \returns If the given index position does not exist, an empty,
    /// null-terminated string will be returned.
    const std::string item_label( uint pos ) const;

    /// \brief Obtain the contained item strings of the object.
    const ItemStrings& item_labels( void ) const;

    /// \brief Query the storage status of the object's container.
    ///
    /// \returns Boolean TRUE if the storage container is empty, boolean FALSE
    /// if the storage container is not empty.
    bool empty( void ) const;

    /// \brief Query the number of items stored in the object's container.
    uint size( void ) const;

    /// \brief Destroy all the items stored in the object's container.
    void clear( void );

    /// \brief Destroy an item at a specified index.
    void erase( uint pos );

    /// \brief Sets the selection to the given index.
    ///
    /// \remarks The selection (selected) state will be removed if the given
    /// index is not found (does not exist).
    void set_selection( uint pos );

    /// \brief Selects the item with the specified string in the control.
    ///
    /// \returns Boolean TRUE if the text label was found; boolean FALSE if the
    /// text label was not found.
    bool set_selection( const std::string& label );

    /// \brief Find an item whose label matches the given string.
    ///
    /// \returns The index position of the text label upon a match, or nom::npos
    /// if no text label was matched.
    int find( const std::string& label );

    /// \brief Add a label string to the specified index of the object's
    /// container.
    ///
    /// \param pos The index position to insert the item before.
    ///
    /// \remarks Any existing data stored at the given index position will be
    /// overwritten.
    ///
    /// \returns The index of the inserted item on success, or nom::npos on
    /// failure.
    int insert( uint pos, const std::string& label );

    /// \brief Add label strings to the specified index of the object's
    /// container.
    ///
    /// \param pos The index position to insert the item before.
    ///
    /// \remarks Any existing data stored at the given index position will be
    /// overwritten.
    ///
    /// \returns The index of the last inserted item on success, or nom::npos
    /// on failure.
    int insert( uint pos, const std::initializer_list<std::string>& labels );

    /// \brief Add a label string to the end of the object's container.
    ///
    /// \returns The index of the inserted item on success, or nom::npos on
    /// failure.
    int append( const std::string& label );

    /// \brief Add label strings to the end of the object's container.
    ///
    /// \returns The index of the last inserted item on success, or nom::npos
    /// on failure.
    int append( const std::vector<std::string>& labels );

  private:
    /// \brief Internal storage container of the items.
    ItemStrings labels_;

    /// \brief The indexed position of the selection (selected) item.
    ///
    /// \remarks An invalid selection state is nom::npos.
    int selection_;
};

} // namespace nom

#endif // include guard defined
