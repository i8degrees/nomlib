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
#ifndef NOMLIB_SYSTEM_IVALUE_SERIALIZER_HPP
#define NOMLIB_SYSTEM_IVALUE_SERIALIZER_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/serializers/serializers_config.hpp"
#include "nomlib/ptree/ptree_forwards.hpp"

namespace nom {

/// \brief Abstract interface for saving nom::Value objects.
class IValueSerializer
{
  public:
    typedef IValueSerializer self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    IValueSerializer( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual ~IValueSerializer( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual std::string serialize( const Value& source ) = 0;

    virtual bool save( const Value& source, const std::string& filename ) = 0;
};

} // namespace nom

#endif // include guard defined

/// \class nom::IValueSerializer
/// \ingroup system
///
///   [TO BE WRITTEN]
///
