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

#include "nomlib/tests/common.hpp"  // nom::UnitTest

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \note The resources and variables used derived from examples/gui_button.cpp
/// and tests/gui/ListBoxLayoutTest.cpp.
class UIWidgetTest: public ::testing::Test
{
  public:
    /// \remarks Initialization of resources.
    UIWidgetTest( void ) :
      app_state{ true }
    {
      this->main_window = nullptr;

      nom::init_third_party( InitHints::DefaultInit );

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not disable vertical refresh." );
      }

      // this->input_mapper.clear();

      InputActionMapper state;
      KeyboardAction action;

      EventCallback ev_quit( [&] () { this->app_state = false; } );
      EventCallback ev_screenshot( [&] () { this->window.save_screenshot( OUTPUT_SCREENSHOT_FILENAME ); } );
      EventCallback ev_fullscreen( [&] () { this->window.toggle_fullscreen(); } );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE );
      state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_q );
      state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_F1 );
      state.insert( "ev_screenshot", nom::InputAction( action, ev_screenshot ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_f );
      state.insert( "ev_fullscreen", nom::InputAction( action, ev_fullscreen ) );

      this->input_mapper.insert( "ButtonLayoutTest", state, true );
    }

    virtual ~UIWidgetTest( void )
    {
      // Nothing to clean up
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

      // Necessary for loading font resources
      ASSERT_TRUE( this->window.create( "UIWidgetTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) );

      // Scale window contents up by the new width & height
      this->window.set_logical_size( WINDOW_WIDTH, WINDOW_HEIGHT );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them as well.
      delete this->main_window;
      this->main_window = nullptr;
    }

    sint on_run( void )
    {
      while( this->app_state == true )
      {
        while( this->evt.poll_event( this->ev ) )
        {
          // this->evt.process_event( this->ev );

          this->input_mapper.on_event( this->ev );
          this->main_window->process_event( this->ev );
        }

        this->window.update();
        this->main_window->update();

        // Background color fill
        this->window.fill( nom::Color4i::SkyBlue );

        this->main_window->draw( this->window );
      }

      return NOM_EXIT_SUCCESS;
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
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;

    // Game loop support
    bool app_state;
    RenderWindow window;
    Event ev;

    EventHandler evt;
    InputStateMapper input_mapper;

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
  // Only used for setting the working directory path to that of the executable,
  // so we can quickly get to saved screen-shots.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  ::testing::InitGoogleTest( &argc, argv );

  // Allows us to toggle interactive test runs
  nom::UnitTest::init( argc, argv );

  return RUN_ALL_TESTS();
}
