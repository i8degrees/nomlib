/******************************************************************************

    nomlib System Interface Library

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SYSTEM_HEADERS
#define NOMLIB_SYSTEM_HEADERS

#include "nomlib/config.hpp"

#include "nomlib/system/Clock.hpp"
#include "nomlib/system/FPS.hpp"
#include "nomlib/system/GameStates.hpp"
#include "nomlib/system/ObjectCache.hpp"
#include <nomlib/system/File.hpp>
#include "nomlib/system/SDL_App.hpp"
#include "nomlib/system/Input.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/system/Sleep.hpp"

#if defined ( NOM_PLATFORM_OSX )
  #include <nomlib/system/OSX/ResourcePath.hpp>
#endif

#endif // NOMLIB_SYSTEM_HEADERS defined
