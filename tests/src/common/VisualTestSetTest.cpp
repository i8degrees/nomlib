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

#include "nomlib/tests/common/VisualTestSet.hpp"

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>

namespace nom {

TEST( VisualTestSetTest, DefaultConstructor )
{
  VisualTestSet obj;

  EXPECT_EQ( "", obj.test_set() )
  << "Test set name should be a null-terminated string";

  EXPECT_EQ( "", obj.test_name() )
  << "Test name should be a null-terminated string";

  EXPECT_EQ( nom::revision(), obj.version() )
  << "Version string should match the constructor's recorded engine version";

  EXPECT_EQ( "", obj.timestamp() )
  << "Timestamp string should be a null-terminated string";

  EXPECT_EQ( "", obj.comment() )
  << "Test comment should be a null-terminated string";

  EXPECT_EQ( Size2i::null, obj.resolution() )
  << "Resolution should be Size2i::null";

  EXPECT_EQ( 0, obj.images().size() )
  << "Image file list size should be zero";

  EXPECT_EQ( "", obj.directory() )
  << "Image file directory should be a null-terminated string";
}

TEST( VisualTestSetTest, InitializeConstructor )
{
  File fp;
  Size2i resolution( 640, 480 );
  std::string ts = nom::file_timestamp();

  VisualTestSet obj (
                      "VisualTestSetTest",
                      "InitializeTestSetConstructor",
                      ts,
                      resolution,
                      fp.currentPath()
                    );

  EXPECT_EQ( "VisualTestSetTest", obj.test_set() )
  << "Test set name should match";

  EXPECT_EQ( "InitializeTestSetConstructor", obj.test_name() )
  << "Test name should match";

  EXPECT_EQ( nom::revision(), obj.version() )
  << "Version string should match";

  EXPECT_EQ( ts, obj.timestamp() )
  << "Timestamp string should match";

  EXPECT_EQ( "", obj.comment() )
  << "Test comment should be a null-terminated string";

  EXPECT_EQ( resolution, obj.resolution() )
  << "Resolution should match";

  EXPECT_EQ( 0, obj.images().size() )
  << "Image file list size should be zero";

  EXPECT_EQ( fp.currentPath(), obj.directory() )
  << "Image file directory should be the current path";
}

TEST( VisualTestSetTest, EqualityComparison )
{
  File fp;
  Size2i resolution( 640, 480 );
  std::string ts = nom::file_timestamp();

  VisualTestSet eq1  (
                        "VisualTestSetTest",
                        "InitializeTestSetConstructor",
                        ts,
                        resolution,
                        fp.currentPath()
                      );

  VisualTestSet eq2 (
                        "VisualTestSetTest",
                        "InitializeTestSetConstructor",
                        ts,
                        resolution,
                        fp.currentPath()
                      );

  VisualTestSet eq3 (
                        "",
                        "",
                        ts,
                        resolution,
                        ""
                      );

  VisualTestSet ineq1 (
                        "",
                        "",
                        ts,
                        Size2i( 320, 240 ),
                        fp.currentPath()
                      );

  ineq1.append_image( "Image0.png" );
  ineq1.append_image( "Image1.png" );
  ineq1.append_image( "Image2.png" );
  ineq1.append_image( "Image3.png" );

  VisualTestSet ineq2 (
                        "",
                        "",
                        ts,
                        Size2i( 320, 240 ),
                        fp.currentPath()
                      );

  ineq2.append_image( "Image0.png" );
  ineq2.append_image( "Image1.png" );
  ineq2.append_image( "Image2.png" );
  ineq2.append_image( "Image3.png" );

  VisualTestSet ineq3 (
                        "",
                        "",
                        ts,
                        Size2i( 320, 240 ),
                        fp.currentPath()
                      );

  ineq3.append_image( "Image0.png" );
  ineq3.append_image( "Image1.png" );
  ineq3.append_image( "Image2.png" );

  EXPECT_TRUE( eq1 == eq1 )
  << "Object's resolution and image list should match";

  EXPECT_TRUE( eq1 == eq2 )
  << "Object's resolution and image list should match";

  EXPECT_TRUE( eq1 == eq3 )
  << "Object's resolution and image list should match";

  EXPECT_TRUE( ineq1 == ineq2 )
  << "Object's resolution and image list should match";

  EXPECT_TRUE( ineq3 == ineq3 )
  << "Object's resolution and image list should match";

  EXPECT_FALSE( eq1 == ineq1 )
  << "Object's resolution and image list should not match";

  EXPECT_FALSE( eq1 == ineq2 )
  << "Object's resolution and image list should not match";

  EXPECT_FALSE( eq1 != eq1 )
  << "Object's resolution and image list should not match";

  EXPECT_FALSE( eq1 != eq2 )
  << "Object's resolution and image list should not match";

  EXPECT_FALSE( eq1 != eq3 )
  << "Object's resolution and image list should not match";

  EXPECT_TRUE( eq1 != ineq1 )
  << "Object's resolution and image list should not match";

  EXPECT_FALSE( ineq2 == ineq3 )
  << "Object's resolution and image list should not match";
}

TEST( VisualTestSetTest, TimestampComparison )
{
  std::string ts = nom::file_timestamp();
  VisualTestSet ts1;
  ts1.set_timestamp( ts );

  VisualTestSet ts2;
  ts2.set_timestamp( ts );

  // Note that these tests should fail if we delay less than 1,000ms (1s)
  nom::sleep( 1000 );

  ts = nom::file_timestamp();
  VisualTestSet ts3;
  ts3.set_timestamp( ts );

  EXPECT_FALSE( ts1 > ts2 )
  << "Timestamps should match to the accuracy of a second";

  EXPECT_FALSE( ts1 < ts2 )
  << "Timestamps should match to the accuracy of a second";

  EXPECT_TRUE( ts3 > ts1 )
  << "Timestamps should NOT match to the accuracy of a second";

  EXPECT_TRUE( ts3 > ts2 )
  << "Timestamps should NOT match to the accuracy of a second";

  EXPECT_TRUE( ts1 < ts3 )
  << "Timestamps should NOT match to the accuracy of a second";

  EXPECT_TRUE( ts2 < ts3 )
  << "Timestamps should NOT match to the accuracy of a second";
}

TEST( VisualTestSetTest, SaveFile )
{
  Path p;
  File fp;
  std::string ts = nom::file_timestamp();
  IValueSerializer* serializer = new JsonCppSerializer();
  Size2i resolution( 640, 480 );

  VisualTestSet obj (
                      "VisualTestSetTest",
                      "SaveVisualTestSet",
                      ts,
                      resolution,
                      fp.currentPath()
                    );

  obj.set_comment( "Output file sanity test" );

  obj.append_image( "FakeImageFile1_0.png" );
  obj.append_image( "FakeImageFile2_0.png" );
  obj.append_image( "FakeImageFile3_0.png" );
  obj.append_image( "FakeImageFile4_0.png" );

  EXPECT_TRUE( obj.save_file( serializer ) );
}

/// \note This test depends on SaveFile
TEST( VisualTestSetTest, LoadFile )
{
  Path p;
  File fp;
  std::string ts = nom::file_timestamp();
  IValueDeserializer* serializer = new JsonCppDeserializer();
  Size2i resolution( 640, 480 );

  VisualTestSet obj;
  obj.set_directory( fp.currentPath() );
  EXPECT_TRUE( obj.load_file( serializer ) );

  EXPECT_EQ( "VisualTestSetTest", obj.test_set() )
  << "Test set name should match";

  EXPECT_EQ( "SaveVisualTestSet", obj.test_name() )
  << "Test name should match";

  EXPECT_EQ( nom::revision(), obj.version() )
  << "Version string should match";

  EXPECT_EQ( ts, obj.timestamp() )
  << "Timestamp string should match";

  EXPECT_EQ( "Output file sanity test", obj.comment() )
  << "Test comment should match";

  EXPECT_EQ( resolution, obj.resolution() )
  << "Resolution should match";

  EXPECT_EQ( 4, obj.images().size() )
  << "Unexpected image file list size";

  EXPECT_EQ( fp.currentPath(), obj.directory() )
  << "Image file directory should be the current path";

  EXPECT_EQ( "FakeImageFile1_0.png", obj.image(0) )
  << "Unexpected image file path";

  EXPECT_EQ( "FakeImageFile2_0.png", obj.image(1) )
  << "Unexpected image file path";

  EXPECT_EQ( "FakeImageFile3_0.png", obj.image(2) )
  << "Unexpected image file path";

  EXPECT_EQ( "FakeImageFile4_0.png", obj.image(3) )
  << "Unexpected image file path";
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
