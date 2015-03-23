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
#ifndef NOMLIB_PTREE_VSTRING_HPP
#define NOMLIB_PTREE_VSTRING_HPP

#include <cstring>

#include "nomlib/config.hpp"

namespace nom {

typedef nom::size_type ArrayIndex;

class VString
{
  public:
    typedef VString self_type;

    /// \brief Construct an invalid member key.
    VString();

    ~VString();

    /// \brief Construct a member key from a C string.
    VString(const char* key);

    /// \brief Construct a member key from a C++ string.
    VString(const std::string& key);

    /// \brief Construct a member key from an array element index.
    VString(ArrayIndex index);

    /// \brief Copy constructor.
    VString(const self_type& rhs);

    /// \brief Copy assignment operator.
    self_type& operator =(const self_type& rhs);

    /// \brief Lesser than comparison operator.
    ///
    /// \note This is required for an implementation inside std::map.
    bool operator <(const self_type& rhs) const;

    /// \brief Equality comparison operator.
    ///
    /// \note This is required for an implementation inside std::map.
    bool operator ==(const self_type& rhs) const;

    /// \brief Get the C++ string stored for this member key.
    std::string string() const;

    /// \brief Get the C string stored for this member key.
    const char* c_str() const;

    /// \brief Get the stored array index key.
    ArrayIndex index() const;

  private:
    void swap(self_type& rhs);

    std::string cstr_;
    ArrayIndex index_;
};

} // namespace nom

#endif // include guard defined
