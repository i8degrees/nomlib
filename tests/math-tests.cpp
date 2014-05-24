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
#include <nomlib/math.hpp>

#define NOM_UNIT_TEST(num) \
  ( "Unit Test #num" )

/// \brief Enable expected results output during each unit test.
///
/// \note See Makefile for what to pass to enable this feature during compile
/// time.
//#define NOM_DEBUG_TESTS

using namespace nom;

/// Point2 Unit Test 1
nom::int32 do_color_test1 ( void )
{
  NOM_LOG_TRACE(NOM);

  nom::Color4i testme1 ( 242, 1, 1, 255 );
  nom::Color4i testme2 = nom::Color4i::Black;
  nom::Color4i testme3 (-1,-1,-1, Color4i::ALPHA_OPAQUE );
  nom::Color4f testme4 (-1,-1,-1, Color4f::ALPHA_OPAQUE );

  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR(testme1);
    NOM_DUMP_VAR(testme2);
    NOM_DUMP_VAR(testme3);
    NOM_DUMP_VAR(testme4);
  #endif

  if ( testme2 == testme1 ) return NOM_EXIT_FAILURE;
  if ( testme3 != Color4i::null ) return NOM_EXIT_FAILURE;
  if ( testme4 != Color4f::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::int32 do_color_test2 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_color_test3 ( void ) { return NOM_EXIT_SUCCESS; }
nom::int32 do_color_test4 ( void ) { return NOM_EXIT_SUCCESS; }

nom::sint do_rect_test1 ( void )
{
  nom::IntRect testme1 ( 242, 1, 1, 0 );
  nom::IntRect testme2 ( -1, -1, -1, -1 );

  if ( testme1 == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( testme2 == IntRect::null ) return NOM_EXIT_SUCCESS;

  testme1 = testme2;

  if ( testme1 != IntRect::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

nom::sint do_rect_test2 ( void )
{
  NOM_LOG_TRACE(NOM);

  IntRect testme1 ( -1, -1, -1, -1 );
  FloatRect testme2 ( -1, -1, -1, -1 );

  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR(testme1);
    NOM_DUMP_VAR(testme2);
  #endif

  if ( testme1 != IntRect::null ) return NOM_EXIT_FAILURE;
  if ( testme2 != FloatRect::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

sint do_rect_test3 ( void )
{
  NOM_LOG_TRACE(NOM);

  uint idx = 0; // result increment counter

  // Expression part 1 (to be combined with parts 2)
  IntRect expr[12] =  { IntRect( 64, 32, 2, 2 ),  // 0
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 8, 8 ),
                        IntRect( 64, 32, 4, 4 )   // 9
                      };

  // Expression part 2 (to be combined with parts 1)
  IntRect ans[4] =    {
                        IntRect( 2, 2, 2, 2 ),  // 0
                        IntRect( 8, 8, 8, 8 ),
                        IntRect( 4, 4, 4, 4 )   // 2
                      };
  IntRect result[12]; // IntRect::null

  // Expected result: 64 / 2 = 32 && 32 / 2 = 16 && 2/2/2/2 = 1
  result[idx] = expr[idx] /= ans[0];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(32,16,1,1) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (1)

  // Expected result: 64 *= 2 = 128 && 32 *= 2 = 64 && 2*2*2*2 = 16
  result[idx] = expr[idx] *= ans[0];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[1] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(128,64,16,16) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (2)

  // Expected result: 64 -= 8 = 56 && 32 -= 8 = 24 && 8-8-8-8 = 0 (???)
  result[idx] = expr[idx] -= ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(56,24,0,0) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (3)

  // Expected result: 64 += 8 = 72 && 32 += 8 = 40 && 8+8+8+8 = 16 (???)
  // Our result: 72, 40, 32, 32 (???)
  result[idx] = expr[idx] += ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(72,40,32,32) ) return NOM_EXIT_FAILURE;

/* TODO: Finish checking test results
  idx += 1; // ..Next! (4)

  // Expected result: 64 / 8 = 8 && 32 / 8 = 4
  result[idx] = expr[idx] / ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(8,4,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (5)

  // Expected result: 64 * 8 = 512 && 32 * 8 = 256;
  result[idx] = expr[idx] * ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(512,256,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (6)

  // Expected result: 64 - 1 = 63 && 32 - 1 = 31;
  result[idx] = --expr[idx]; // ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(63,31,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (7)

  // Expected result: 64 - 8 = 56 && 32 - 8 = 24
  result[idx] = expr[idx] - ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(56,24,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (8)

  // Expected result: 64 += 1 = 65 && 32 += 1 = 33
  result[idx] = ++expr[idx]; //ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(65,33,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (9)

  // Expected result: 64 += 4 = 68 && 32 += 4 = 40
  result[idx] = expr[idx] += ans[2];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == IntRect::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != IntRect(68,36,8,8) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next! (10)
TODO */
  return NOM_EXIT_SUCCESS;
}

nom::sint do_point2_test1 ( void )
{
  NOM_LOG_TRACE(NOM);

  Point2i testme1 ( -1, -1 );
  Point2f testme2 ( -1, -1 );

  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR(testme1);
    NOM_DUMP_VAR(testme2);
  #endif

  if ( testme1 != Point2i::null ) return NOM_EXIT_FAILURE;
  if ( testme2 != Point2f::null ) return NOM_EXIT_FAILURE;

  return NOM_EXIT_SUCCESS;
}

sint do_point2_test2 ( void )
{
  NOM_LOG_TRACE(NOM);

  uint idx = 0; // result increment counter

  // Expression part 1 (to be combined with parts 2)
  Point2i expr[12] =  { Point2i( 64, 32 ),  // 0
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 ),
                        Point2i( 64, 32 )   // 9
                      };

  // Expression part 2 (to be combined with parts 1)
  Point2i ans[4] =    {
                        Point2i( 2, 2 ),  // 0
                        Point2i( 8, 8 ),
                        Point2i( 4, 4 )   // 2
                      };
  Point2i result[12]; // Point2i::null

  // Expected result: 64 / 2 = 32, 32 / 2 = 16
  result[idx] = expr[idx] /= ans[0];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(32,16) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 *= 2 = 128 && 32 *= 2 = 64
  result[idx] = expr[idx] *= ans[0];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[1] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(128,64) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 -= 8 = 56 && 32 -= 8 = 24
  result[idx] = expr[idx] -= ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(56,24) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 += 8 = 72 && 32 += 8 = 40
  result[idx] = expr[idx] += ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(72,40) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 / 8 = 8 && 32 / 8 = 4
  result[idx] = expr[idx] / ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(8,4) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 * 8 = 512 && 32 * 8 = 256;
  result[idx] = expr[idx] * ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(512,256) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 - 1 = 63 && 32 - 1 = 31;
  result[idx] = --expr[idx]; // ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(63,31) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 - 8 = 56 && 32 - 8 = 24
  result[idx] = expr[idx] - ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(56,24) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 += 1 = 65 && 32 += 1 = 33
  result[idx] = ++expr[idx]; //ans[1];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(65,33) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  // Expected result: 64 += 4 = 68 && 32 += 4 = 40
  result[idx] = expr[idx] += ans[2];
  #if defined( NOM_DEBUG_TESTS )
    NOM_DUMP_VAR( idx );
    NOM_DUMP_VAR( result[idx] );
  #endif

  if ( result[idx] == Point2i::null ) return NOM_EXIT_FAILURE;
  if ( result[idx] != Point2i(68,36) ) return NOM_EXIT_FAILURE;

  idx += 1; // ..Next!

  return NOM_EXIT_SUCCESS;
}


nom::int32 main ( nom::int32 args_count, char* args[] )
{
  if ( do_color_test1() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(1), "Failed nom::Color unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test2() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(2), "Failed nom::Color unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test3() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(3), "Failed nom::Color unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_color_test4() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(4), "Failed nom::Color unit test 4" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_rect_test1() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(1), "Failed Rect unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_rect_test2() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(2), "Failed Rect unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_rect_test3() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(3), "Failed Rect unit test 3" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_point2_test1() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(1), "Failed Point2 unit test 1" );
    return NOM_EXIT_FAILURE;
  }

  if ( do_point2_test2() != NOM_EXIT_SUCCESS )
  {
    DialogMessageBox( NOM_UNIT_TEST(2), "Failed Point2 unit test 2" );
    return NOM_EXIT_FAILURE;
  }

  NOM_LOG_INFO ( NOM_UNIT_TEST, "Math Unit Tests: Pass!" );
  return NOM_EXIT_SUCCESS;
}
