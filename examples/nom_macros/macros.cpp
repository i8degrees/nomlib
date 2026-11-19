/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/system.hpp"
#include "nomlib/math.hpp"

nom::int32 main( nom::int32 argc, char* argv[] )
{
  nom::Point2i pos;
  pos.x = 25;
  pos.y = 25;

  // Note that SDL2 does not need to be initialized before using the logging
  // facilities!

  NOM_DUMP( nom::Point2i(pos) );

  // NOM_DUMP_VAR();
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "pos:", pos );
  NOM_LOG_TRACE( NOM );
  NOM_STUBBED( NOM );

  NOM_LOG_VERBOSE( NOM, "Verbose message" );
  NOM_LOG_DEBUG( NOM, "Debug message" );
  NOM_LOG_INFO( NOM, "Could not initialize something totally not important.");
  NOM_LOG_WARN( NOM, "Could not initialize image resource." );
  NOM_LOG_ERR( NOM, "Could not initialize image resource." );
  NOM_LOG_CRIT( NOM, "Could not initialize application. Exiting..." );

  return NOM_EXIT_SUCCESS;
}
