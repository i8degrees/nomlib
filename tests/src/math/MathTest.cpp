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
#include "gtest/gtest.h"

#include <nomlib/math.hpp>

// #define NOM_DEBUG_MATH_TEST_OUTPUT_COLORS
// #define NOM_DEBUG_MATH_TEST_OUTPUT_POINTS
// #define NOM_DEBUG_MATH_TEST_OUTPUT_RECTS

namespace nom {

class MathTest: public ::testing::Test
{
  public:
    MathTest( void )
    {
      // ...
    }

    ~MathTest( void )
    {
      // ...
    }

    virtual void SetUp( void )
    {
      //
    }

    virtual void TearDown( void )
    {
      //
    }

  protected:
    // Member methods & variables
};

TEST_F( MathTest, Color4API )
{
  nom::Color4i testme1 ( 242, 1, 1, 255 );
  nom::Color4i testme2 = nom::Color4i::Black;
  nom::Color4i testme3 (-1,-1,-1, Color4i::ALPHA_OPAQUE );
  nom::Color4f testme4 (-1,-1,-1, Color4f::ALPHA_OPAQUE );

  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_COLORS )
    NOM_DUMP(testme1);
    NOM_DUMP(testme2);
    NOM_DUMP(testme3);
    NOM_DUMP(testme4);
  #endif

  EXPECT_EQ( testme1, nom::Color4i( 242, 1, 1, nom::Color4i::ALPHA_OPAQUE ) );

  EXPECT_NE( testme2, testme1 );

  EXPECT_EQ( testme3, Color4i::null );

  EXPECT_EQ( testme4, Color4f::null );
}

TEST_F( MathTest, Point2CoreAPI )
{
  Point2i testme1 ( 2, 1 );
  Point2f testme2 ( -1, -1 );

  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP(testme1);
    NOM_DUMP(testme2);
  #endif

  EXPECT_EQ( testme1, nom::Point2i( 2, 1 ) );
  EXPECT_NE( testme1, Point2i::null );

  EXPECT_EQ( testme2, Point2f::null );
}

TEST_F( MathTest, Point2ExtendedAPI )
{
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

  result[idx] = expr[idx] /= ans[0];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(32,16) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] *= ans[0];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(128,64) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] -= ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(56,24) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] += ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(72,40) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] / ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(8,4) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] * ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(512, 256) );

  idx += 1; // ..Next!

  result[idx] = --expr[idx]; // ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(63, 31) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] - ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(56,24) );

  idx += 1; // ..Next!

  result[idx] = ++expr[idx]; //ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(65, 33) );

  idx += 1; // ..Next!

  result[idx] = expr[idx] += ans[2];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_POINTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], Point2i::null );
  EXPECT_EQ( result[idx], Point2i(68,36) );

  idx += 1; // ..Next!
}

TEST_F( MathTest, Point2EqualityAPI )
{
  nom::Point2i lhs( 25, 25 );
  nom::Point2i rhs( 50, 50 );

  nom::Point2i lhse( 2, 2 );
  nom::Point2i rhse( 2, 2 );

  ASSERT_TRUE( lhs < rhs );
  ASSERT_TRUE( rhs > lhs );
  ASSERT_TRUE( lhse <= rhse );
  ASSERT_TRUE( lhse >= rhse );
  ASSERT_TRUE( lhs <= rhs );
  ASSERT_TRUE( rhs >= lhs );
  ASSERT_TRUE( lhs == lhs );
  ASSERT_FALSE( lhs == rhs );
}

TEST_F( MathTest, RectCoreAPI )
{
  nom::IntRect testme1 ( 242, 1, 1, 0 );
  nom::FloatRect testme2 ( -1, -1, -1, -1 );

  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP(testme1);
    NOM_DUMP(testme2);
  #endif

  EXPECT_NE( testme1, IntRect::null );
  EXPECT_EQ( testme1, nom::IntRect( 242, 1, 1, 0 ) );

  EXPECT_EQ( testme2, FloatRect::null );
}

TEST_F( MathTest, RectExtendedAPI )
{
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

  result[idx] = expr[idx] /= ans[0];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(32,16,1,1) );

  idx += 1; // ..Next! (1)

  result[idx] = expr[idx] *= ans[0];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(128,64,16,16) );

  idx += 1; // ..Next! (2)

  result[idx] = expr[idx] -= ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(56,24,0,0) );

  idx += 1; // ..Next! (3)

  result[idx] = expr[idx] += ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(72,40,16,16) );

  idx += 1; // ..Next! (4)

  result[idx] = expr[idx] / ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(8,4,1,1) );

  idx += 1; // ..Next! (5)

  result[idx] = expr[idx] * ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(512,256,64,64) );

  idx += 1; // ..Next! (6)

  result[idx] = --expr[idx]; // ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(63,31,7,7) );

  idx += 1; // ..Next! (7)

  result[idx] = expr[idx] - ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(56,24,0,0) );

  idx += 1; // ..Next! (8)

  result[idx] = ++expr[idx]; //ans[1];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(65,33,9,9) );

  idx += 1; // ..Next! (9)

  result[idx] = expr[idx] += ans[2];
  #if defined( NOM_DEBUG_MATH_TEST_OUTPUT_RECTS )
    NOM_DUMP( idx );
    NOM_DUMP( result[idx] );
  #endif

  EXPECT_NE( result[idx], IntRect::null );
  EXPECT_EQ( result[idx], IntRect(68,36,8,8) );

  idx += 1; // ..Next! (10)
}

TEST_F( MathTest, RectOperators )
{
  nom::IntRect lhs( 25, 25, 4, 4 );
  nom::IntRect rhs( 50, 50, 8, 8 );

  nom::IntRect lhse( 2, 2, 4, 4 );
  nom::IntRect rhse( 2, 2, 8, 8 );

  EXPECT_EQ( true, lhs < rhs );
  EXPECT_EQ( true, rhs > lhs );
  EXPECT_EQ( true, lhse <= rhse );
  EXPECT_EQ( false, lhse >= rhse );
  EXPECT_EQ( true, lhs <= rhs );
  EXPECT_EQ( true, rhs >= lhs );
  EXPECT_EQ( true, lhs == lhs );
  EXPECT_EQ( false, lhs == rhs );
}

TEST_F( MathTest, Size2Operators )
{
  nom::Size2i lhs( 25, 25 );
  nom::Size2i rhs( 50, 50 );

  nom::Size2i lhse( 2, 2 );
  nom::Size2i rhse( 4, 4 );

  EXPECT_EQ( true, lhs < rhs );
  EXPECT_EQ( true, rhs > lhs );
  EXPECT_EQ( true, lhse <= rhse );
  EXPECT_EQ( false, lhse >= rhse );
  EXPECT_EQ( true, lhs <= rhs );
  EXPECT_EQ( true, rhs >= lhs );
  EXPECT_EQ( true, lhs == lhs );
  EXPECT_EQ( false, lhs == rhs );
}

TEST_F( MathTest, Point2iCtor )
{
  Point2i f(2);
  Point2i out;

  out.x = 2;
  out.y = 2;
  EXPECT_EQ( out, f );
}

TEST_F( MathTest, Size2iCtor )
{
  Size2i f(2);
  Size2i out;

  out.w = 2;
  out.h = 2;
  EXPECT_EQ( out, f );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
