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
#ifndef NOMLIB_SYSTEM_PTREE_VALUE_CONST_ITERATOR_HPP
#define NOMLIB_SYSTEM_PTREE_VALUE_CONST_ITERATOR_HPP

#include "nomlib/config.hpp"
#include "nomlib/ptree/ptree_config.hpp"
#include "nomlib/ptree/ptree_forwards.hpp"
#include "nomlib/ptree/ValueIteratorBase.hpp"

namespace nom {

/// \brief Read-only iteration for nom::Value
///
/// \todo Finish implementation.
class ValueConstIterator: public ValueIteratorBase
{
  public:
    typedef ValueConstIterator SelfType;
    typedef ValueIteratorBase DerivedType;

    typedef SelfType* RawPtr;
    typedef SelfType& Reference;

    typedef const ValueIteratorBase::ValueTypeReference ConstReference;
    typedef SelfType ConstIterator;

    /// \brief Default constructor.
    ValueConstIterator();

    /// \brief Destructor.
    ~ValueConstIterator();

    /// \brief Copy constructor
    ValueConstIterator(const ValueConstIterator& rhs);

    /// \brief Copy constructor
    ValueConstIterator(const ObjectIterator& rhs);

    SelfType& operator =(const DerivedType& rhs);

    /// \brief Obtain a pointer to the iterator's object.
    ///
    /// \returns A pointer to the nom::Value object.
    const Value::RawPtr operator ->() const;

    /// \brief Obtain a reference to the iterator's object.
    ///
    /// \returns A reference to the nom::Value object.
    ConstReference operator *() const;

    /// \brief Increment the object by one
    ///
    /// \remarks The object is not modified.
    SelfType& operator ++();

    /// \brief Increment by specified parameter
    ///
    /// \remarks The object is not modified.
    SelfType operator ++(int);

    /// \brief Decrement by one
    ///
    /// \remarks The object is not modified.
    SelfType& operator --();

    /// \brief Decrement by specified parameter
    ///
    /// \remarks The object is not modified.
    SelfType operator --(int);
};

} // namespace nom

#endif // include guard defined
