/******************************************************************************

    Clock -- time accounting -- interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_CLOCK_HEADERS
#define NOMLIB_CLOCK_HEADERS

#include <string>
#include <ctime>

#include "nomlib/config.hpp"

namespace nom {

/// Get the current date & time
const std::string getCurrentTime ( void );


} // namespace nom

#endif // NOMLIB_CLOCK_HEADERS defined
