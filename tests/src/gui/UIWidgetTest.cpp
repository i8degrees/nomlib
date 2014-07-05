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

#include "nomlib/tests/common.hpp"      // nom::UnitTest framework
// #include "nomlib/tests/gui/common.hpp"  // GUI helpers

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \note The resources and variables used derived from examples/gui_button.cpp
/// and tests/gui/ListBoxLayoutTest.cpp.
class UIWidgetTest: public VisualUnitTest
{
  public:
    UIWidgetTest( void ) :
      // Use default resolution provided by VisualUnitTest
      WINDOW_WIDTH{ resolution().w },
      WINDOW_HEIGHT{ resolution().h }
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      // this->append_screenshot_frame( 0 );

      NOM_TEST_FLAG(disable_comparison) = true;
    }

    virtual ~UIWidgetTest( void )
    {
      // Nothing to clean up
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      // Use default initializations provided by VisualUnitTest
      VisualUnitTest::SetUp();

      // Register GUI event listeners onto our main loop (::on_run).
      this->append_event_callback( [&] ( Event ev ) { this->main_window->process_event( ev ); } );

      // Register GUI updates onto our main loop (::on_run).
      this->append_update_callback( [&] ( float delta ) { this->main_window->update(); } );

      // Register GUI rendering onto our main loop (::on_run).
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->main_window->draw( this->render_window() ); } );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them as well.
      delete this->main_window;
      this->main_window = nullptr;
    }

    /// \brief Default callback method used in registering event listeners.
    void on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

    void expected_window_output( const nom::UIWidget* window, const Point2i& pos, const Size2i& size, const std::string& name = "Window" )
    {
      EXPECT_EQ( pos, window->position() );
      EXPECT_EQ( size, window->size() );

      EXPECT_EQ( name, window->name() );
    }

  protected:
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;

    // GUI resources
    nom::UIWidget::raw_ptr main_window;
};

TEST_F( UIWidgetTest, CoreAPI )
{
  Point2i pos;
  Size2i size;

  pos.x = 25;
  pos.y = 25;
  size.w = WINDOW_WIDTH - 100;
  size.h = WINDOW_HEIGHT / 2;

  // Top-level (parent) widget (relative to global "screen" coordinates):
  this->main_window = new nom::UIWidget( pos, size );
  this->main_window->set_name( "Parent" );
  this->main_window->set_decorator( new nom::MinimalDecorator() );
  this->expected_window_output( this->main_window, pos, size, "Parent" );
  EXPECT_NE( nullptr, this->main_window->decorator() );

  // Test UIWidget construction with object pointers:
  nom::UIWidget::raw_ptr w0 = nullptr;
  nom::UIWidget::raw_ptr w1 = nullptr;

  // Absolute coordinates
  pos.x = 25;
  pos.y = 25;
  size.w = 200;
  size.h = 200;

  w0 = new nom::UIWidget( pos, size );
  w1 = new nom::UIWidget( w0 );

  this->expected_window_output( w0, pos, size );
  this->expected_window_output( w1, pos, size );
  EXPECT_EQ( true, w0->visible() );
  EXPECT_EQ( true, w1->visible() );
  EXPECT_EQ( nullptr, w0->decorator() );
  EXPECT_EQ( nullptr, w1->decorator() );

  this->main_window = new nom::UIWidget( Point2i::null, Size2i::null );

  // Absolute coordinates
  pos.x = 50;
  pos.y = 50;
  size.w = 350;
  size.h = 100;
  this->main_window->set_position( pos );
  this->main_window->set_size( size );

  this->expected_window_output( this->main_window, pos, size );
  EXPECT_EQ( nullptr, this->main_window->decorator() );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( UIWidgetTest, Layouts )
{
  UIWidget* layout_widget = nullptr;

  Point2i pos( 25, 25 );
  Size2i size( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 );

  // Top-level parent widget (relative to global "screen" coordinates):
  this->main_window = new nom::UIWidget( pos, size );
  this->main_window->set_decorator( new nom::MinimalDecorator() );

  this->expected_window_output( this->main_window, pos, size );
  EXPECT_NE( nullptr, this->main_window->decorator() );

  layout_widget = new nom::UIWidget( this->main_window );

  // Relative coordinates (to parent widget):
  Point2i rpos( 12, 25 );
  Size2i lsize;
  lsize.w = 350;
  lsize.h = 100;
  layout_widget->set_geometry( IntRect( rpos, lsize ) );

  this->expected_window_output( layout_widget, pos + rpos, lsize );
  ASSERT_FALSE( layout_widget->decorator() != nullptr );

  // Absolute coordinates
  this->expected_window_output( this->main_window, pos, size );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  // Allows us to toggle interactive test runs
  nom::init_test( argc, argv );

  return RUN_ALL_TESTS();
}
