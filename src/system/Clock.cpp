/******************************************************************************

    Clock -- time accounting -- interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/system/Clock.hpp"

namespace nom {

const std::string getCurrentTime ( void )
{
  time_t timer = time ( nullptr );

  return ctime ( &timer );
}


} // namespace nom
