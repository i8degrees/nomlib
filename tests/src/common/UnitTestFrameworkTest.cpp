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

#include "nomlib/tests/common.hpp"

namespace nom {

class UnitTestFrameworkTest: public UnitTest
{
  public:
    virtual void SetUp( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }

    virtual void TearDown( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }

    static void SetUpTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }

    static void TearDownTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }
};

TEST_F( UnitTestFrameworkTest, GetterMethods )
{
  EXPECT_EQ( true, this->app_state() )
  << "App state should default to boolean TRUE";

  EXPECT_EQ( "UnitTestFrameworkTest", this->test_set() )
  << "Test set name should be the same as the test case name";

  EXPECT_EQ( "GetterMethods", this->test_name() )
  << "Test name should be the same as the test description";
}

TEST_F( UnitTestFrameworkTest, SetterMethods )
{
  EXPECT_EQ( true, this->app_state() )
  << "App state should default to boolean TRUE";

  EXPECT_EQ( "UnitTestFrameworkTest", this->test_set() )
  << "Test set name should be the same as the test case name";

  EXPECT_EQ( "SetterMethods", this->test_name() )
  << "Test name should be the same as the test description";

  // Note that these are private members of the nom::UnitTest interface
  this->set_test_set( "FrameworkTest" );
  this->set_test_name( "TestDescription" );

  EXPECT_EQ( "FrameworkTest", this->test_set() )
  << "Test set name should be the same as the test case name";

  EXPECT_EQ( "TestDescription", this->test_name() )
  << "Test name should be the same as the test description";
}

TEST_F( UnitTestFrameworkTest, AppState )
{
  EXPECT_EQ( true, this->app_state() )
  << "App state should default to boolean TRUE";

  this->set_app_state( false );
  EXPECT_EQ( false, this->app_state() )
  << "App state should be boolean FALSE";
}

TEST_F( UnitTestFrameworkTest, InvalidInit )
{
  nom::init_test( -1, nullptr );
}

TEST_F( UnitTestFrameworkTest, DefaultUnitTestFlags )
{
  nom::init_test( -1, nullptr );

  EXPECT_EQ( false, NOM_TEST_FLAG(interactive) )
  << "Interactive state flag should default to boolean FALSE";

  EXPECT_EQ( false, NOM_TEST_FLAG(reference_screenshot) )
  << "Reference state flag should default to boolean FALSE";

  EXPECT_EQ( "", NOM_TEST_FLAG(test_comment) )
  << "Test comment string should default to boolean FALSE";

  EXPECT_EQ( "Reference", NOM_TEST_FLAG(comparison_dir) )
  << "Comparison directory path string should default to \"Reference\"";

  EXPECT_EQ( false, NOM_TEST_FLAG(disable_comparison) )
  << "Image set comparison should be enabled by default";

  EXPECT_EQ( false, NOM_TEST_FLAG(no_html_output) )
  << "HTML test results should be enabled by default";
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Parse command line options supported by our framework
  // nom::init_test( argc, argv );

  return RUN_ALL_TESTS();
}
