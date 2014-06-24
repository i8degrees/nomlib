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
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "nomlib/tests/common/ImageDiff.hpp"

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>

namespace nom {

const File cwd;
const Path p;
const std::string RESOURCES_PATH =  cwd.currentPath() + p.native() + "Resources"
                                    + p.native() + "ImageDiffTest" + p.native();

const std::string SIMPLE_EQUALITY_RESOURCES_PATH[2] = {
                                                        // First path
                                                        RESOURCES_PATH +
                                                        "SimpleEqualityComparison_Reference" +
                                                        p.native(),
                                                        // Second path
                                                        RESOURCES_PATH +
                                                        "SimpleEqualityComparison" +
                                                        p.native()
                                                      };

const std::string SIMPLE_INEQUALITY_RESOURCES_PATH[2] = {
                                                          // First path
                                                          RESOURCES_PATH +
                                                          "SimpleInequalityComparison_Reference" +
                                                          p.native(),
                                                          // Second path
                                                          RESOURCES_PATH +
                                                          "SimpleInequalityComparison" +
                                                          p.native()
                                                        };

const std::string COMPLEX_EQUALITY_RESOURCES_PATH[2] =  {
                                                          // First path
                                                          RESOURCES_PATH +
                                                          "ComplexEqualityComparison_Reference" +
                                                          p.native(),
                                                          // Second path
                                                          RESOURCES_PATH +
                                                          "ComplexEqualityComparison" +
                                                          p.native()
                                                        };

const std::string COMPLEX_INEQUALITY_RESOURCES_PATH[2] =  {
                                                            RESOURCES_PATH +
                                                            "ComplexInequalityComparison_Reference" +
                                                            p.native(),
                                                            RESOURCES_PATH +
                                                            "ComplexInequalityComparison" +
                                                            p.native()
                                                          };

void dump_result( const ImageDiffResult& r )
{
  NOM_DUMP_VAR( "incorrect_pixels: ", r.incorrect_pixels );
  NOM_DUMP_VAR( "mse_channels: ", r.mse );
  NOM_DUMP_VAR( "mse_channels: ", r.mse_channels );
  NOM_DUMP_VAR( "psnr_channels: ", r.psnr );
  NOM_DUMP_VAR( "psnr_channels: ", r.psnr_channels );
  NOM_DUMP_VAR( "ssim: ", r.ssim );
}

TEST( ImageDiffTest, InitializeConstructor )
{
  File fp;

  ImageDiff obj( fp.currentPath(), fp.currentPath() );

  EXPECT_EQ( fp.currentPath(), obj.directory( 0 ) )
  << "Path should match constructed value";

  EXPECT_EQ( fp.currentPath(), obj.directory( 1 ) )
  << "Path should match constructed value";

  EXPECT_EQ( "", obj.directory( 2 ) )
  << "Path should not exist";
}

TEST( ImageDiffTest, InvalidImageFiles )
{
  ImageDiffResult r;
  ImageDiff obj( RESOURCES_PATH, RESOURCES_PATH );

  r = obj.compare("FakeImageFile1_0.png");

  EXPECT_FALSE( r.passed );
  EXPECT_EQ( 0, r.incorrect_pixels );
  EXPECT_EQ( 0, r.mse );
  EXPECT_EQ( Color4f::null, r.mse_channels );
  EXPECT_EQ( 0, r.psnr );
  EXPECT_EQ( Color4f::null, r.psnr_channels );
  EXPECT_EQ( 0, r.ssim );
}

/// \fixme This test is broken on builds for Windows platform.
TEST( ImageDiffTest, SimpleEqualityComparison )
{
  File fp;
  ImageDiffResult r;

  ImageDiff obj( SIMPLE_EQUALITY_RESOURCES_PATH[0], SIMPLE_EQUALITY_RESOURCES_PATH[1] );

  r = obj.compare("VisualTests1_0.png");

  EXPECT_TRUE( r.passed );
  EXPECT_TRUE( r.incorrect_pixels == 0 );
}

/// \fixme This test is broken on builds for Windows platform.
TEST( ImageDiffTest, ComplexEqualityComparison )
{
  File fp;
  ImageDiffResult r;

  ImageDiff obj( COMPLEX_EQUALITY_RESOURCES_PATH[0], COMPLEX_EQUALITY_RESOURCES_PATH[1] );

  r = obj.compare("listbox-horiz-truetypefont-layout.png");

  EXPECT_TRUE( r.passed );
  EXPECT_TRUE( r.incorrect_pixels == 0 );
}

/// \fixme This test is broken on builds for Windows platform.
TEST( ImageDiffTest, SimpleInequalityComparison )
{
  File fp;
  ImageDiffResult r;

  ImageDiff obj( SIMPLE_INEQUALITY_RESOURCES_PATH[0], SIMPLE_INEQUALITY_RESOURCES_PATH[1] );

  r = obj.compare("VisualTests1_0.png");

  EXPECT_FALSE( r.passed );
  EXPECT_FALSE( r.incorrect_pixels == 0 );

  // EXPECT_TRUE( r.incorrect_pixels == 307200 );

  dump_result( r );
}

/// \see http://rileyadams.net/gsoc/July5/out.html#PlayPen_CameraSetDirection_1000
///
/// \fixme This test is broken on builds for Windows platform.
TEST( ImageDiffTest, ComplexInequalityComparison )
{
  File fp;
  ImageDiffResult r;

  ImageDiff obj( COMPLEX_INEQUALITY_RESOURCES_PATH[0], COMPLEX_INEQUALITY_RESOURCES_PATH[1] );

  r = obj.compare("PlayPen_CameraSetDirection_1000.png");

  EXPECT_FALSE( r.passed );
  EXPECT_TRUE( r.incorrect_pixels == 9310 );

  // FIXME: I cannot compare the MSE, PSNR & SSIM because our results are ever
  // so slightly different from the original author's posted results. Further
  // research is needed...

  dump_result( r );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Only used for setting the working directory path to that of the executable,
  // so we can quickly get to saved screen-shots.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
