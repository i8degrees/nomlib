/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <i8degrees@gmail.com>
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
#include <iostream>
#include <cstdlib>
#include <cassert>

#include <nomlib/graphics.hpp>

/// nom::RenderWindow Unit Test 1
nom::int32 do_window_test1 ( void );

/// nom::RenderWindow Unit Test 2
nom::int32 do_window_test2 ( void );

/// nom::RenderWindow Unit Test 3
nom::int32 do_window_test3 ( void );

/// nom::RenderWindow Unit Test 4
nom::int32 do_window_test4 ( void );

nom::int32 do_window_test1 ( void )
{
  const int WINDOW_WIDTH = 320;
  const int WINDOW_HEIGHT = 240;
  nom::Window window;

  if ( window.create ( "window_test1", WINDOW_WIDTH, WINDOW_HEIGHT, 0 ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  if ( window.set_window_icon( "icon.png" ) == false ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_window_test2 ( void )
{
  nom::Window window;
  std::string title = "nomlib::RenderWindow Unit Test 2";
  window.set_window_title( title );

  if ( window.window_title() != title ) return NOM_EXIT_SUCCESS;

  return NOM_EXIT_FAILURE;
}

nom::int32 do_window_test3 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_window_test4 ( void ) { return NOM_EXIT_SUCCESS; }

nom::int32 main ( nom::int32 args_count, char* args[] )
{
  if ( do_window_test1() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::RenderWindow unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_window_test2() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::RenderWindow unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_window_test3() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::RenderWindow unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_window_test4() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::RenderWindow unit test 4" );
    return NOM_EXIT_FAILURE;
  }

  NOM_LOG_INFO ( NOM_UNIT_TEST, "Graphics Unit Tests: Pass!" );
  return NOM_EXIT_SUCCESS;
}
