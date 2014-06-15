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
#include "gui/common.hpp"           // nom::priv helpers

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \note The resources and variables used derived from examples/gui_listbox.cpp
/// and tests/gui/VBoxLayoutTest.cpp.
class ListBoxLayoutTest: public ::testing::Test
{
  public:
    /// \remarks Initialization of resources.
    ListBoxLayoutTest( void ) :
      app_state{ true }
    {
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

      this->input_mapper.insert( "ListBoxLayoutTest", state, true );
    }

    virtual ~ListBoxLayoutTest( void )
    {
      // Nothing to clean up!
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      // if( nom::UnitTest::interactive() )
      // {
      //   window_flags |= SDL_WINDOW_SHOWN;
      // }
      // else
      // {
      //   window_flags |= SDL_WINDOW_HIDDEN;
      // }

      // Necessary for loading font resources
      ASSERT_TRUE( this->window.create( "ListBoxLayoutTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) );

      // Scale window contents up by the new width & height
      this->window.set_logical_size( WINDOW_WIDTH, WINDOW_HEIGHT );

      // Top-level (parent) window (relative to global "screen" coordinates):
      this->main_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 30, WINDOW_HEIGHT - 30 ) );
      this->main_window->set_title();

      // Draw a frame so that we can visually see the maximal bounds of the
      // parent window
      this->main_window->set_decorator( new nom::MinimalDecorator() );

      // FIXME: Re-enable the underlying event code in UIWidget for the
      // WINDOW_MOUSE_DOWN event.
      //
      // this->main_window->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      this->layout_widget = new nom::UIWidget( this->main_window );

      // The initial width and height of the layout does not matter; it is
      //resized per test.
      this->layout_widget->set_geometry( 12, 25, 100, 100 );
      // this->layout_widget->set_title();

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->layout_widget->set_decorator( new nom::MinimalDecorator() );

      labels[0].push_back( "item_0" );
      labels[0].push_back( "item_1" );
      labels[0].push_back( "item_2" );

      labels[1].push_back( "ITEM_0" );
      labels[1].push_back( "ITEM_1" );

      labels[2].push_back( "item_0" );
      labels[2].push_back( "item_1" );
      labels[2].push_back( "item_2" );
      labels[2].push_back( "item_3" );

      labels[3].push_back( "item_0" );
      labels[3].push_back( "item_1" );
      labels[3].push_back( "item_2" );

      // Our widgets to be used in the layout:

      // this->listbox0 = this->create_listbox( this->main_window, Point2i::null, Size2i(50,24), "listbox0", labels[0] );
      this->listbox0 = this->create_listbox( this->layout_widget, Point2i::null, Size2i::null, "listbox0", labels[0] );

      // this->listbox0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->listbox0->set_decorator( new nom::MinimalDecorator() );
      this->listbox0->set_selected_text_color( nom::Color4i::Gray );
      // this->listbox0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );
      // this->listbox0->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      this->listbox1 = this->create_listbox( this->layout_widget, Point2i::null, Size2i(60, 36), "listbox1", labels[1] );

      this->listbox1->set_decorator( new nom::MinimalDecorator() );
      // this->listbox1->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );
      // this->listbox1->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      // this->listbox2 = this->create_listbox( this->layout_widget, Point2i::null, Size2i(50,24), "listbox2", labels[2] );
      this->listbox2 = this->create_listbox( this->layout_widget, Point2i::null, Size2i::null, "listbox2", labels[2] );

      this->listbox2->set_decorator( new nom::MinimalDecorator() );
      // this->listbox2->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );
      // this->listbox2->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      // this->listbox3 = this->create_listbox( this->layout_widget, Point2i::null, Size2i(50,24), "listbox3", labels[3] );
      this->listbox3 = this->create_listbox( this->layout_widget, Point2i::null, Size2i::null, "listbox3", labels[3] );

      this->listbox3->set_decorator( new nom::MinimalDecorator() );
      // this->listbox3->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );
      // this->listbox3->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      this->spacers.push_back( 8 );
      this->items.push_back( this->listbox0 );
      this->spacers.push_back( 8 );
      this->items.push_back( this->listbox1 );
      this->spacers.push_back( 40 );
      this->items.push_back( this->listbox2 );
      this->items.push_back( this->listbox3 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them --
      // listbox0, listbox1, listbox2, listbox3.
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
          this->layout_widget->process_event( this->ev );
        }

        this->window.update();
        this->main_window->update();
        this->layout_widget->update();

        // Background color fill
        this->window.fill( nom::Color4i::SkyBlue );

        this->main_window->draw( this->window );
        this->layout_widget->draw( this->window );
      }

      return NOM_EXIT_SUCCESS;
    }

    /// \brief Test the creation of a button widget.
    nom::ListBox::raw_ptr create_listbox( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const nom::ItemStrings& labels, int sel = -1 )
    {
      nom::UIItemContainer::raw_ptr storage = new nom::UIItemContainer();
      storage->append( labels );

      if( sel != -1 )
      {
        storage->set_selection( sel );
      }

      nom::ListBox::raw_ptr listbox = new nom::ListBox( window, -1, pos, size, storage );

      listbox->set_name( name );
      NOM_ASSERT( listbox->name() == name );

      NOM_ASSERT( listbox->parent()->position() == window->position() );
      NOM_ASSERT( listbox->size() == size );

      NOM_ASSERT( listbox->font().valid() );

      return listbox;
    }

  protected:
    const nom::int32 WINDOW_WIDTH = 640;
    const nom::int32 WINDOW_HEIGHT = 480;

    // Game loop support
    bool app_state;
    RenderWindow window;
    Event ev;

    EventHandler evt;
    InputStateMapper input_mapper;

    // GUI resources

    nom::UIWidget::raw_ptr main_window;
    nom::UIWidget::raw_ptr layout_widget;

    /// \brief Item strings for our ListBox widgets.
    nom::ItemStrings labels[4];

    nom::ListBox::raw_ptr listbox0;
    nom::ListBox::raw_ptr listbox1;
    nom::ListBox::raw_ptr listbox2;
    nom::ListBox::raw_ptr listbox3;

    /// \brief Expected position results (per test).
    std::vector<Point2i> pos;

    /// \brief Expected with and height results (per test).
    std::vector<Size2i> dims;

    /// \remarks The stored pointers are not owned by us; do *not* free them!
    std::vector<UIWidget*> items;

    /// \brief The width and height of the UISpacerItem items to test in a
    /// layout (per test).
    std::vector<int> spacers;
};

TEST_F( ListBoxLayoutTest, HorizontalLayoutUsingTrueTypeFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->resize( Size2i( 350, 100 ) );

  this->layout_widget->set_font( SystemFonts::cache().load_resource( "Arial" ) );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, "HorizontalLayoutUsingTrueTypeFont", Orientations::Horizontal );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 116, 40 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 275, 67 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 271, 67 ), layout->total_size_hint() );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (listbox0)
  // priv::expected_layout_widget_pos( layout, 1, Point2i( 45, 50 ) );
  // priv::expected_layout_widget_dims( layout, 1, Size2i( 53, 52 ) );
  priv::expected_layout_widget_dims( layout, 1, Size2i( 56, 67 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 2, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (listbox1)
  priv::expected_layout_widget_dims( layout, 3, Size2i( 60, 36 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 4, Point2i(-1,-1), Size2i(40,40) );

  // Widget item (listbox2)
  // priv::expected_layout_widget_dims( layout, 5, Size2i( 53, 67 ) );
  priv::expected_layout_widget_dims( layout, 5, Size2i( 56, 67 ) );

  // Widget item (listbox3)
  // priv::expected_layout_widget_pos( layout, 6, Point2i( 252, 50 ) );
  // priv::expected_layout_widget_dims( layout, 6, Size2i( 53, 52 ) );
  // priv::expected_layout_widget_dims( layout, 6, Size2i( 53, 67 ) );
  priv::expected_layout_widget_dims( layout, 6, Size2i( 56, 67 ) );

  if( nom::UnitTest::interactive() )
  {
    EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  }
}

TEST_F( ListBoxLayoutTest, HorizontalLayoutUsingBitmapFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->resize( Size2i( 350, 100 ) );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, "HorizontalLayoutUsingBitmapFont", Orientations::Horizontal );

  this->layout_widget->set_font( SystemFonts::cache().load_resource( "VIII" ) );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 116, 40 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 266, 58 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 262, 58 ), layout->total_size_hint() );

  if( nom::UnitTest::interactive() )
  {
    EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  }
}

TEST_F( ListBoxLayoutTest, VerticalLayoutUsingTrueTypeFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->set_font( SystemFonts::cache().load_resource( "Arial" ) );

  this->layout_widget->resize( Size2i( 100, 350 ) );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, "VerticalLayoutUsingTrueTypeFont", Orientations::Vertical );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 60, 92 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 60, 263 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 56, 264 ), layout->total_size_hint() );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (listbox0)
  // priv::expected_layout_widget_pos( layout, 1, Point2i( 37, 58 ) );
  // priv::expected_layout_widget_dims( layout, 1, Size2i( 53, 52 ) );
  priv::expected_layout_widget_dims( layout, 1, Size2i( 56, 67 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 2, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (listbox1)
  priv::expected_layout_widget_dims( layout, 3, Size2i( 60, 36 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 4, Point2i(-1,-1), Size2i(40,40) );

  // Widget item (listbox2)
  // priv::expected_layout_widget_dims( layout, 5, Size2i( 53, 67 ) );
  priv::expected_layout_widget_dims( layout, 5, Size2i( 56, 67 ) );

  // Widget item (listbox3)
  // priv::expected_layout_widget_pos( layout, 6, Point2i( 37, 261 ) );
  // priv::expected_layout_widget_dims( layout, 6, Size2i( 53, 52 ) );
  priv::expected_layout_widget_dims( layout, 6, Size2i( 56, 67 ) );

  if( nom::UnitTest::interactive() )
  {
    EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  }
}

TEST_F( ListBoxLayoutTest, VerticalLayoutUsingBitmapFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->resize( Size2i( 100, 350 ) );
  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, "VerticalLayoutUsingBitmapFont", Orientations::Vertical );

  // FIXME: This currently has to be set after the creation of the layout.
  this->layout_widget->set_font( SystemFonts::cache().load_resource( "VIII" ) );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 60, 92 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 60, 240 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 56, 236 ), layout->total_size_hint() );

  if( nom::UnitTest::interactive() )
  {
    EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  }
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
