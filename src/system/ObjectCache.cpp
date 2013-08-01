/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/system/ObjectCache.hpp"

namespace nom {
  namespace priv {

// Initialize our cache in global space
std::map <std::string, std::shared_ptr<void>> ObjectCache::cache;

ObjectCache::ObjectCache ( void )
{
//NOM_LOG_CLASSINFO;
}

ObjectCache::~ObjectCache ( void )
{
//NOM_LOG_CLASSINFO;
}

std::shared_ptr<void> ObjectCache::addObject  ( const std::string& key,
                                                std::shared_ptr<void> object
                                              )
{
  std::map <std::string, std::shared_ptr<void>>::iterator res;

  res = cache.insert( std::pair< std::string, std::shared_ptr<void>> ( key, object ) ).first;

NOM_LOG_INFO ( "ObjectCache: " + key + " has been added to the cache." );

  return res->second;
}

bool ObjectCache::removeObject ( const std::string& key )
{
  cache.erase ( key );

  return true;
}

std::shared_ptr<void> ObjectCache::getObject ( const std::string& key )
{
  std::map <std::string, std::shared_ptr<void>>::iterator itr;

  itr = cache.find ( key );

  // We have found no entity matching the key if we reach the end of the cache;
  // otherwise, we return the existing entity from the cache
  if ( itr == cache.end() )
  {
    return nullptr;
  }
  else
  {
    return itr->second;
  }
}


  } // namespace priv
} // namespace nom
