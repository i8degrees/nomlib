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

#include "nomlib/config.hpp"
#include "nomlib/version.hpp"
#include "nomlib/system/init.hpp"

using namespace nom;

TEST( VersionInfoTest, ValidVersionStrings )
{
  nom::VersionInfo info;

  EXPECT_TRUE( VersionInfo::convert_version_string("0.4.1", info) );
  EXPECT_TRUE( info.major() == 0 );
  EXPECT_TRUE( info.minor() == 4 );
  EXPECT_TRUE( info.patch() == 1 );
  EXPECT_EQ( "0.4.1", info.version_string() );

  info = VersionInfo();
  EXPECT_TRUE( VersionInfo::convert_version_string("0.11.0", info) );
  EXPECT_TRUE( info.major() == 0 );
  EXPECT_TRUE( info.minor() == 11 );
  EXPECT_TRUE( info.patch() == 0 );
  EXPECT_EQ( "0.11.0", info.version_string() );

  info = VersionInfo();
  EXPECT_TRUE( VersionInfo::convert_version_string( "9.9.9", info) );
  EXPECT_TRUE( info.major() == 9 );
  EXPECT_TRUE( info.minor() == 9 );
  EXPECT_TRUE( info.patch() == 9 );
  EXPECT_EQ( "9.9.9", info.version_string() );

  info = VersionInfo();
  EXPECT_TRUE( VersionInfo::convert_version_string( "10.9.99", info) );
  EXPECT_TRUE( info.major() == 10 );
  EXPECT_TRUE( info.minor() == 9 );
  EXPECT_TRUE( info.patch() == 99 );
  EXPECT_EQ( "10.9.99", info.version_string() );

  info = VersionInfo();
  EXPECT_TRUE( VersionInfo::convert_version_string("0.0.0", info) );
  EXPECT_TRUE( info.major() == 0 );
  EXPECT_TRUE( info.minor() == 0 );
  EXPECT_TRUE( info.patch() == 0 );
  EXPECT_EQ( "0.0.0", info.version_string() );
}

TEST( VersionInfoTest, InvalidVersionStrings )
{
  nom::VersionInfo info;

  EXPECT_FALSE( VersionInfo::convert_version_string("9", info) );
  EXPECT_TRUE( info.major() == 9 );
  EXPECT_TRUE( info.minor() == 0 );
  EXPECT_TRUE( info.patch() == 0 );
  EXPECT_EQ( "9.0.0", info.version_string() );

  info = VersionInfo();
  EXPECT_FALSE( VersionInfo::convert_version_string("9.", info) );
  EXPECT_TRUE( info.major() == 9 );
  EXPECT_TRUE( info.minor() == 0 );
  EXPECT_TRUE( info.patch() == 0 );
  EXPECT_EQ( "9.0.0", info.version_string() );

  info = VersionInfo();
  EXPECT_FALSE( VersionInfo::convert_version_string("ver", info) );
  EXPECT_TRUE( info.major() == 0 );
  EXPECT_TRUE( info.minor() == 0 );
  EXPECT_TRUE( info.patch() == 0 );
  EXPECT_EQ( "0.0.0", info.version_string() );
}

/// \todo Use EXPECT_TRUE macro instead of ASSERT_TRUE
TEST( VersionInfoTest, ComparisonEquality )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  ASSERT_TRUE( VersionInfo::convert_version_string( "10.9.99", cmp1 ) );
  ASSERT_TRUE( VersionInfo::convert_version_string( "0.11.0", cmp2 ) );
  ASSERT_TRUE( cmp1 != cmp2 );
}

/// \todo Use EXPECT_TRUE macro instead of ASSERT_TRUE
TEST( VersionInfoTest, ComparisonInequality )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  ASSERT_TRUE( VersionInfo::convert_version_string( "0.4.1", cmp1) );
  ASSERT_TRUE( VersionInfo::convert_version_string( "0.4.1", cmp2) );
  ASSERT_TRUE( cmp1 == cmp2 );
}

TEST( VersionInfoTest, ComparisonLesserThan )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.3.9", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp2 ) );
  EXPECT_EQ( true, cmp1 < cmp2 );
}

TEST( VersionInfoTest, ComparisonNotLesserThan )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp2 ) );
  EXPECT_EQ( false, cmp1 < cmp2 );
}

TEST( VersionInfoTest, ComparisonLesserThanOrEqualTo )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.1", cmp2 ) );
  EXPECT_EQ( true, cmp1 <= cmp2 );
}

TEST( VersionInfoTest, ComparisonGreaterThan )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.11.0", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.3.9", cmp2 ) );
  EXPECT_EQ( true, cmp1 > cmp2 );
}

TEST( VersionInfoTest, ComparisonNotGreaterThan )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.0", cmp2 ) );
  EXPECT_EQ( false, cmp1 > cmp2 );
}

TEST( VersionInfoTest, ComparisonGreaterThanOrEqualTo )
{
  nom::VersionInfo cmp1;
  nom::VersionInfo cmp2;

  EXPECT_TRUE( VersionInfo::convert_version_string( "0.4.1", cmp1 ) );
  EXPECT_TRUE( VersionInfo::convert_version_string( "0.1.0", cmp2 ) );
  EXPECT_EQ( true, cmp1 >= cmp2 );
}

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false )
  {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  // Hides the err messages generated by nom::convert_version_str when doing
  // invalid tests
  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_APPLICATION, NOM_LOG_PRIORITY_CRITICAL);

  return RUN_ALL_TESTS();
}
