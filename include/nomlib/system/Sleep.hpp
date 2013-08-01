/******************************************************************************

    Sleep / delay interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SLEEP_HEADERS
#define NOMLIB_SLEEP_HEADERS

#include <algorithm>

#include <SDL/SDL.h>

#include "nomlib/config.hpp"

namespace nom {

/// SDL_Delay wrapper
///
/// Values below 10 milliseconds are clamped to 10
void sleep ( uint32 milliseconds );


} // namespace nom

#endif // NOMLIB_SLEEP_HEADERS defined
