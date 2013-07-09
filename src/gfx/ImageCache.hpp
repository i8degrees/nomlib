/******************************************************************************

    Image Cache

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved

******************************************************************************/
#ifndef NOMLIB_IMAGE_CACHE_HEADERS
#define NOMLIB_IMAGE_CACHE_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include "sdl/utils.hpp"
#include "math/Color.hpp"
#include "gfx/SDL_Image.hpp"
#include "config.hpp"

namespace nom {

class ImageCache
{
  public:
    ~ImageCache ( void );

    static std::shared_ptr<void> getImage ( const std::string& filename,
                                            const Color& colorkey, uint32 flags
                                          );

  private:
    ImageCache ( void );
    static std::map <std::string, std::shared_ptr<void>> image_cache;
};

} // namespace nom

#endif // NOMLIB_IMAGE_CACHE_HEADERS defined
