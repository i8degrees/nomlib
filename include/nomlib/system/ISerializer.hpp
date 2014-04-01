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
#ifndef NOMLIB_SYSTEM_ISERIALIZER_HPP
#define NOMLIB_SYSTEM_ISERIALIZER_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"

namespace nom {

/// \brief Abstract interface for serializing and unserializing nom::Value
/// objects.
class ISerializer
{
  public:
    typedef ISerializer SelfType;

    typedef SelfType* RawPtr;
    typedef std::unique_ptr<SelfType> UniquePtr;

    ISerializer ( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual ~ISerializer ( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual bool serialize( const Value& source, const std::string& output ) const = 0;
    virtual bool unserialize( const std::string& input, Value& dest ) const = 0;
};

} // namespace nom

#endif // include guard defined

/// \class nom::ISerializer
/// \ingroup system
///
///   [TO BE WRITTEN]
///
