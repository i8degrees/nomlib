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
#ifndef NOMLIB_GUI_STRING_HPP
#define NOMLIB_GUI_STRING_HPP

#include <string>

#include "nomlib/config.hpp"
#include "nomlib/core/IObject.hpp"

namespace nom {

/// \brief C++ string object type
class String: public IObject
{
  public:
    typedef String self_type;

    /// \brief Default constructor.
    String( void );

    /// \brief Destructor.
    virtual ~String( void );

    /// \brief Construct an object using a C++ string.
    String( const std::string& string );

    /// \brief Construct an object using a C-style string.
    String( const char* string );

    /// \brief Get the stored string.
    ///
    /// \returns C++ string object reference.
    const std::string& str( void ) const;

    /// \brief Get the stored string.
    ///
    /// \returns C-style string pointer.
    const char* cstr( void ) const;

  private:
    std::string str_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::String
/// \ingroup gui
///
/// This class is a wrapper around std::string and exists to be a compatible
/// object type for nom::DataViewListStore.
///
/// \see DataViewItem
/// \see DataViewListStore
///
