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
#ifndef NOMLIB_GUI_TESTS_DATAGRID_CARD_COLLECTION_HPP
#define NOMLIB_GUI_TESTS_DATAGRID_CARD_COLLECTION_HPP

#include <string>

#include "nomlib/config.hpp"
#include "nomlib/tests/gui/datagrid/Card.hpp"

namespace nom {

/// \brief A mock CardCollection
///
/// \remarks This interface should **not** be used outside of unit tests; for
/// starters, assertion macros are disabled for running under this particular
/// testing environment.
///
/// \see libRocketDataGridTest, TTcards::CardCollection
class CardCollection
{
  public:
    CardCollection();
    virtual ~CardCollection();

    CardList cards() const;

    bool load_db();

    nom::size_type num_rows() const;

    /// \brief Lookup a card ID by name.
    ///
    /// \param name The card's name.
    const Card& lookup_by_name( const std::string& name ) const;

    /// \brief Lookup a card name by ID.
    ///
    /// \param id The card's identifier.
    const Card& lookup_by_id( int id ) const;

    /// \brief Rudimentary debugging aid.
    std::string dump();

  private:
    std::vector<Card> cards_;
};

} // namespace nom

#endif // include guard defined
