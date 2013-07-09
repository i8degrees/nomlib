/******************************************************************************

    Object Cache

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved

******************************************************************************/
#include "ObjectCache.hpp"

namespace nom {
  namespace priv {

// Initialize our cache in global space
std::map <std::string, std::shared_ptr<void>> ObjectCache::cache;

ObjectCache::ObjectCache ( void )
{
//NOMLIB_LOG_INFO;
}

ObjectCache::~ObjectCache ( void )
{
//NOMLIB_LOG_INFO;
}

std::shared_ptr<void> ObjectCache::addObject  ( const std::string& key,
                                                std::shared_ptr<void> object
                                              )
{
  std::map <std::string, std::shared_ptr<void>>::iterator res;

  res = cache.insert( std::pair< std::string, std::shared_ptr<void>> ( key, object ) ).first;

NOMLIB_LOG ( "ObjectCache: " + key + " has been added to the cache." );

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
