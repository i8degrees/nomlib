/******************************************************************************

    State pure abstract class

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_ISTATE_HEADERS
#define NOMLIB_ISTATE_HEADERS

#include "config.hpp"
#include "sys/Logger.hpp"
#include "sys/SDL_Input.hpp"

namespace nom {

class IState: public SDL_Input // "has a" relationship
{
  public:
    virtual ~IState ( void )
    {
NOMLIB_LOG_INFO;
    }

    /// Required interface
    virtual void Update ( float ) = 0;

    /// Required interface
    virtual void Draw ( void* ) = 0;

    /// Optional interface
    virtual void onInit ( void ) {}

    /// Optional interface
    virtual void onExit ( void ) {}

    /// Optional interface
    virtual void Pause ( void ) {}

    /// Optional interface
    virtual void Resume ( void ) {}
};


} // namespace nom

#endif // NOMLIB_ISTATE_HEADERS defined
