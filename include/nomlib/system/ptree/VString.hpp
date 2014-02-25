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
#ifndef NOMLIB_SYSTEM_PTREE_VSTRING_HPP
#define NOMLIB_SYSTEM_PTREE_VSTRING_HPP

// #include <cstring>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/system/ptree/ptree_config.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"

namespace nom {

class VString
{
  public:
    typedef VString SelfType;

    VString( void );
    ~VString( void );

    VString( uint val );

    VString( const char* val );

    VString( const std::string& val );

    /// \brief Copy constructor.
    VString( const SelfType& copy );

    /// \brief Copy assignment operator.
    SelfType& operator =( const SelfType& other );

    /// \NOTE Required implementation for usage inside a std::map template.
    bool operator <( const VString& other ) const;

    /// \NOTE Required implementation for usage inside a std::map template.
    bool operator ==( const VString& other ) const;

    /// \NOTE Required implementation for usage inside a std::map template.
    uint operator[]( uint val );

    /// \NOTE Required implementation for usage inside a std::map template.
    const std::string& operator[]( const std::string& val );

    const std::string value( void ) const; // TODO: rename??

    uint index(void) const;

  private:
    // TODO: Implement as const char*
    std::string value_;
    uint index_;
};

} // namespace nom

#endif // include guard defined
