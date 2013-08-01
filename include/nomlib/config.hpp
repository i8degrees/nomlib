/******************************************************************************

    Configuration file for nomlib library

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_CONFIG_HEADERS
#define NOMLIB_CONFIG_HEADERS

// nomlib version
#include "nomlib/version.hpp"

// nomlib data types
#include "nomlib/types.hpp"

// Platform detection
#include "nomlib/platforms.hpp"

// nomlib debugging

// Standard debug level; logging of info, warnings & errors
#define NOM_DEBUG

// Internal development; logging of class object construction and destruction
#define NOM_DEBUG_TRACE


#ifndef __cplusplus
  #warning "nomlib requires a C++11 compiler"
#endif


#endif // NOMLIB_CONFIG_HEADERS defined
