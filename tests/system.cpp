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

#include <nomlib/system.hpp>

/// nom::File Unit Test 1
nom::int32 do_file_test1 ( nom::int32 args_count, char* args[] );

/// nom::File Unit Test 2
nom::int32 do_file_test2 ( void );

/// nom::File Unit Test 3
nom::int32 do_file_test3 ( void );

/// nom::File Unit Test 4
nom::int32 do_file_test4 ( void );

nom::int32 do_file_test1 ( nom::int32 args_count, char* args[] )
{
  nom::File dir;
  std::string pwd = "\0";

  pwd = dir.path ( args[0] );

  if ( args_count < 1 ) return NOM_EXIT_FAILURE;
  if ( pwd == "" || pwd == "\0" ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_file_test2 ( void )
{
  nom::Path p;

  #if defined ( NOM_PLATFORM_OSX ) || defined ( NOM_PLATFORM_LINUX ) || defined ( NOM_PLATFORM_POSIX )
    if ( p.native().compare ("/") != 0 ) return NOM_EXIT_FAILURE;
  #elif defined ( NOM_PLATFORM_WINDOWS )
    if ( p.native().compare ("\\") != 0 ) return NOM_EXIT_FAILURE;
  #else // Unsupported
    return NOM_EXIT_FAILURE;
  #endif

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_file_test3 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_file_test4 ( void ) { return NOM_EXIT_SUCCESS; }

nom::int32 main ( nom::int32 args_count, char* args[] )
{
  if ( do_file_test1 ( args_count, args ) != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::File unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_file_test2() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::File unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_file_test3() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::File unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_file_test4() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::File unit test 4" );
    return NOM_EXIT_FAILURE;
  }

  NOM_LOG_INFO ( NOM_UNIT_TEST, "System Unit Tests: Pass!" );
  return NOM_EXIT_SUCCESS;
}
