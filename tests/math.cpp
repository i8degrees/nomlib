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

#include <nomlib/math.hpp>

using namespace nom;

/// nom::Color Unit Test 1
nom::int32 do_color_test1 ( void );

/// nom::Color Unit Test 2
nom::int32 do_color_test2 ( void );

/// nom::Color Unit Test 3
nom::int32 do_color_test3 ( void );

/// nom::Color Unit Test 4
nom::int32 do_color_test4 ( void );

/// nom::Coords Unit Test 1
nom::int32 do_coords_test1 ( void );

/// nom::Coords Unit Test 2
nom::int32 do_coords_test2 ( void );

/// nom::Coords Unit Test 3
nom::int32 do_coords_test3 ( void );

/// nom::Coords Unit Test 4
nom::int32 do_coords_test4 ( void );

nom::int32 do_color_test1 ( void )
{
  NOM_LOG_TRACE(NOM);

  nom::Color4u testme1 ( 242, 1, 1, 255 );
  nom::Color4u testme2 = nom::Color4u::Black;
  nom::Color4u testme3 (-1,-1,-1, Color4u::ALPHA_OPAQUE );
  nom::Color4f testme4 (-1,-1,-1, Color4f::ALPHA_OPAQUE );

  NOM_DUMP_VAR(testme1);
  NOM_DUMP_VAR(testme2);
  NOM_DUMP_VAR(testme3);
  NOM_DUMP_VAR(testme4);

  if ( testme2 == testme1 ) return NOM_EXIT_FAILURE;
  if ( testme3 != Color4u::null ) return NOM_EXIT_FAILURE;
  if ( testme4 != Color4f::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_color_test2 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_color_test3 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_color_test4 ( void ) { return NOM_EXIT_SUCCESS; }

nom::int32 do_coords_test1 ( void )
{
  nom::Coords testme1 ( 242, 1, 1, 0 );
  nom::Coords testme2 ( -1, -1, -1, -1 );

  if ( testme1.isNull() ) return NOM_EXIT_FAILURE;
  if ( testme2.isNull() ) return NOM_EXIT_SUCCESS;
  if ( testme2 == nom::Coords::null ) return NOM_EXIT_SUCCESS;

  testme1 = testme2;

  if ( testme1.isNull() ) return NOM_EXIT_SUCCESS;
  if ( testme2.isNull() ) return NOM_EXIT_SUCCESS;
  if ( testme2 == nom::Coords::null ) return NOM_EXIT_SUCCESS;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_coords_test2 ( void )
{
  NOM_LOG_TRACE(NOM);

  IntRect testme1 ( -1, -1, -1, -1 );
  FloatRect testme2 ( -1, -1, -1, -1 );

  NOM_DUMP_VAR(testme1);
  NOM_DUMP_VAR(testme2);

  if ( testme1 != IntRect::null ) return NOM_EXIT_FAILURE;
  if ( testme2 != FloatRect::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_coords_test3 ( void )
{
  NOM_LOG_TRACE(NOM);

  Point2i testme1 ( -1, -1 );
  Point2f testme2 ( -1, -1 );

  NOM_DUMP_VAR(testme1);
  NOM_DUMP_VAR(testme2);

  if ( testme1 != Point2i::null ) return NOM_EXIT_FAILURE;
  if ( testme2 != Point2f::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_coords_test4 ( void )
{
  return NOM_EXIT_SUCCESS;
}

nom::int32 main ( nom::int32 args_count, char* args[] )
{
  if ( do_color_test1() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Color unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test2() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Color unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test3() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Color unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test4() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Color unit test 4" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_coords_test1() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Coords unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_coords_test2() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Coords unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_coords_test3() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Coords unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_coords_test4() != NOM_EXIT_SUCCESS )
  {
    NOM_LOG_ERR ( NOM_UNIT_TEST, "Failed nom::Coords unit test 4" );
    return NOM_EXIT_FAILURE;
  }

  NOM_LOG_INFO ( NOM_UNIT_TEST, "Math Unit Tests: Pass!" );
  return NOM_EXIT_SUCCESS;
}
