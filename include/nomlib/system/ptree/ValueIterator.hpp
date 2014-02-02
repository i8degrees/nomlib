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
#ifndef NOMLIB_SYSTEM_PTREE_VALUE_ITERATOR_HPP
#define NOMLIB_SYSTEM_PTREE_VALUE_ITERATOR_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/ptree/ptree_config.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"
#include "nomlib/system/ptree/ValueIteratorBase.hpp"

namespace nom {

class ValueIterator: public ValueIteratorBase
{
  public:
    typedef ValueIterator SelfType;
    typedef ValueIteratorBase DerivedType;

    typedef SelfType* RawPtr;
    typedef SelfType Iterator;

    /// \brief Default constructor.
    ValueIterator( void );

    /// \brief Destructor.
    ~ValueIterator( void );

    /// \brief Copy constructor
    ValueIterator( const ValueIterator& copy );

    /// \brief Copy constructor
    ValueIterator( const ArrayIterator& itr );

    /// \brief Copy constructor
    ValueIterator( const ObjectIterator& itr );

    /// \brief Copy assignment
    SelfType& operator =( const SelfType& other );

    /// \brief Obtain a reference to the iterator's object.
    ///
    /// \returns A reference to the nom::Value object.
    ValueTypeReference operator *( void ) const;

    /// \brief Obtain a pointer to the iterator's object.
    ///
    /// \returns A pointer to the nom::Value object.
    ValueTypePointer operator ->( void ) const;

    /// \brief Increment the object by one
    SelfType& operator ++( void );

    /// \brief Increment by specified parameter
    SelfType operator ++( sint );

    /// \brief Decrement by one
    SelfType& operator --( void );

    /// \brief Decrement by specified parameter
    SelfType operator --( sint );
};

} // namespace nom

#endif // include guard defined
