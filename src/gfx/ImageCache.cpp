/******************************************************************************

    Image Cache

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved

******************************************************************************/
#include "ImageCache.hpp"

namespace nom {

std::map <std::string, std::shared_ptr<void>> ImageCache::image_cache;

ImageCache::ImageCache ( void )
{
//NOMLIB_LOG_INFO;

  // Initialize instance variables
}

ImageCache::~ImageCache ( void )
{
//NOMLIB_LOG_INFO;

  // Clean up instance variables
}

std::shared_ptr<void> ImageCache::getImage ( const std::string& filename, const Color& colorkey, uint32 flags )
{
  std::map <std::string, std::shared_ptr<void>>::iterator itr = image_cache.find ( filename );

  // If we reach the end, we do not have this in cache, so prepare to cache it
  if ( itr == image_cache.end() )
  {
    SDL_Image image;
    std::shared_ptr<void> image_buffer = nullptr;

    image_buffer = std::shared_ptr<void> ( image.loadFromFile ( filename ) );

NOMLIB_LOG ( "ImageCache: " + filename + " has been added to the cache." );

    std::map <std::string, std::shared_ptr<void>>::iterator res = image_cache.insert( std::pair< std::string, std::shared_ptr<void>> ( filename, image_buffer ) ).first;

    return res->second; // Return the new pointer from cache
  }
  else
    return itr->second; // Return the existing pointer from cache
}

} // namespace nom
