/******************************************************************************
    IDisplay.h

    Abstract Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_IDISPLAY_HEADERS
#define NOMLIB_IDISPLAY_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "SDL.h"

#include "gamelib.h"

namespace nom
{
  class IDisplay
  {
    public:
      IDisplay ( void )
      //: display_width ( 0 ), display_height ( 0 ),
        //display_colorbit ( 0 ), display_flags ( 0 )
      {
        // NULL
      }

      virtual ~IDisplay ( void )
      {
        // NULL
      }

      virtual bool  CreateWindow  (   signed int display_width,
                                      signed int display_height,
                                      signed int display_colorbit, unsigned int flags
                                  ) = 0;
      private:
        //signed int display_width;
        //signed int display_height;
        //signed int display_colorbit;
        //unsigned int display_flags;
      protected:
        // ...
    };
}

#endif // NOMLIB_IDISPLAY_HEADERS defined
