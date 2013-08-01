/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_IDISPLAY_HEADERS
#define NOMLIB_IDISPLAY_HEADERS

#include <string>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"

namespace nom {

class IDisplay
{
  public:
    IDisplay ( void )
    {
      // Do nothing
    }

    virtual ~IDisplay ( void )
    {
      // Do nothing
    }

    virtual void createWindow ( int32_t display_width, int32_t display_height,
                                int32_t display_colorbit, uint32_t flags = 0
                              ) = 0;

    virtual void* get ( void ) const = 0;
    virtual int32_t getDisplayWidth ( void ) const = 0;
    virtual int32_t getDisplayHeight ( void ) const = 0;
    virtual int32_t getDisplayColorBits ( void ) const = 0;
    virtual uint32_t getDisplayFlags ( void ) const = 0;
    virtual uint16 getDisplayPitch ( void ) const = 0;
    virtual void* getDisplayPixels ( void ) const = 0;
    virtual void* getDisplayPixelsFormat ( void ) const = 0;
    virtual const Coords getDisplayBounds ( void ) const = 0;

    virtual void Update ( void ) = 0;
    virtual void Update ( const Coords& ) = 0;

    virtual void toggleFullScreenWindow ( int32_t width, int32_t height ) = 0;

    virtual const std::string getWindowTitle ( void ) const = 0;
    virtual void setWindowTitle ( const std::string& ) = 0;

    virtual void* getWindowIcon ( void ) const = 0;
    virtual void setWindowIcon ( const std::string& ) = 0;

    private:
      // ...
    protected:
      // ...
};


} // namespace nom

#endif // NOMLIB_IDISPLAY_HEADERS defined
