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

/// \brief Dump information on rendering capabilities of a device

#include <iostream>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

const std::string APP_NAME = "device_info";

const int WINDOW_WIDTH = 0;
const int WINDOW_HEIGHT = 0;

int main ( int argc, char* argv[] )
{
  nom::Window window;
  nom::RendererInfo renderer_info;

  if ( nom::nomlib_init ( argc, argv ) == false )
  {
    NOM_LOG_ERR ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }

  nom::uint32 window_flags = SDL_WINDOW_HIDDEN;

  if ( window.create ( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
  {
    NOM_LOG_ERR ( APP_NAME, "Could not create a window." );
    exit ( NOM_EXIT_FAILURE );
  }

  renderer_info = window.caps();
  std::cout << renderer_info << std::endl;

  return NOM_EXIT_SUCCESS;
}
