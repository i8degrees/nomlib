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

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include "nomlib/math.hpp"
#include "nomlib/system.hpp"
#include "nomlib/graphics.hpp"

namespace nom {

/// \brief Create a suitable rendering for the unit tests.
///
/// \remarks Filled rectangles make nice, simple to visualize image
/// differentials.
Rectangle generate_frame(const Point2i& pos, const Size2i& size)
{
  Rectangle rect( IntRect(pos, size), Color4i::Magenta );

  return rect;
}

class VisualUnitTestFrameworkNonDefaultCtorTest: public VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    VisualUnitTestFrameworkNonDefaultCtorTest( void ) :
      VisualUnitTest( Size2i( 800, 600 ) )
    {
      // NOM_LOG_TRACE( NOM );

      if( NOM_TEST_FLAG( interactive ) == true )
      {
        // We re-implement ::SetUp without any input mappings, and so the
        // end-user would have no way of politely exiting the app if we were to
        // allow this.
        NOM_LOG_ERR( NOM, "This unit test does not support interactive execution." );
        exit( NOM_EXIT_FAILURE );
      }

      // Always use the same backdrop color on each unit test
      NOM_TEST_ADD_RENDER( const RenderWindow& win, this->render_window().fill( Color4i::SkyBlue ) );

      // Take a snapshot on the first and 50th rendered frames
      this->append_screenshot_frame( 0 );
      this->append_screenshot_frame( 50 );
    }

    /// \brief Re-implements VisualUnitTest::SetUp with a custom, minimal
    /// initialization.
    ///
    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp( void )
    {
      // Initialize the rendering window
      EXPECT_TRUE( this->window_.create( this->test_set(), this->resolution(), 0 ) );

      // No input mappings

      // this->append_update_callback( [&] ( float delta ) { this->render_window().update(); } );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Ensure that we can use this virtual method without any ill effects.
    }
};

TEST_F( VisualUnitTestFrameworkNonDefaultCtorTest, RenderTestImage1 )
{
  nom::Rectangle f = nom::generate_frame( Point2i( 25, 25 ), Size2i( 80, 60 ) );
  NOM_TEST_ADD_RENDER( const RenderWindow& win, f.draw( this->render_window() ) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( VisualUnitTestFrameworkNonDefaultCtorTest, RenderTestImage2 )
{
  nom::Rectangle f = nom::generate_frame( Point2i( 25, 25 ), Size2i( 160, 120 ) );
  NOM_TEST_ADD_RENDER( const RenderWindow& win, f.draw( this->render_window() ) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

/// \remarks This test is setup to fail on the second run.
TEST_F( VisualUnitTestFrameworkNonDefaultCtorTest, RenderTestImage3 )
{
  nom::Rectangle f;

  // Intentionally create an image differential (test failure) if we are not
  // creating reference screen dumps.
  if( NOM_TEST_FLAG(reference_screenshot) == true )
  {
    f = nom::generate_frame( Point2i( 25, 25 ), Size2i( 320, 240 ) );
  }
  else
  {
    f = nom::generate_frame( Point2i( 25, 25 ), Size2i( 320, 200 ) );
  }

  NOM_TEST_ADD_RENDER( const RenderWindow& win, f.draw( this->render_window() ) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );

  // This test should fail when compared to the reference image
  if( NOM_TEST_FLAG(reference_screenshot) == true )
  {
    EXPECT_TRUE( this->compare() )
    << "The reference and comparison images should be identical";
  }
  else
  {
    // EXPECT_TRUE( this->compare() )
    // << "This test is a success if the reference and comparison images differ by 755 pixels";
    EXPECT_FALSE( this->compare() )
    << "Comparison image should NOT match the reference image.";
  }
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  // nom::UnitTest framework integration; NOM_TEST_FLAG and
  // nom::test_output_directory depends on this function call being made.
  nom::init_test( argc, argv );

  if( NOM_TEST_FLAG( reference_screenshot ) == true )
  {
    NOM_TEST_FLAG( test_comment ) = "Reference image set";
  }
  else
  {
    NOM_TEST_FLAG( test_comment ) = "The last image should fail";
  }

  return RUN_ALL_TESTS();
}
