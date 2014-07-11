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
#include "nomlib/system/ColorDatabase.hpp"

namespace nom {

ColorDatabase::ColorDatabase( void )
{
  // NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );
}

ColorDatabase::~ColorDatabase( void )
{
  // NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );
}

const Color4i& ColorDatabase::find_color( const std::string& key )
{
  auto res = this->colors_.find( key );

  if( res != this->colors_.end() )
  {
    return res->second;
  }

  return Color4i::null;
}

bool ColorDatabase::append_color( const std::string& key, const Color4i& c )
{
  pair member( key, c );

  auto res = this->colors_.find( key );

  // Err if duplicate member
  if( res != this->colors_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not insert color: the color already exists." );
    return false;
  }

  this->colors_.insert( member );

  return true;
}

void ColorDatabase::erase_color( const std::string& key )
{
  this->colors_.erase( key );
}

} // namespace nom
