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
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

      // Necessary for loading font resources
      ASSERT_TRUE( this->window.create( "ListBoxLayoutTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) );

      // Scale window contents up by the new width & height
      this->window.set_logical_size( WINDOW_WIDTH, WINDOW_HEIGHT );

      // Window-scope mouse button click events
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->main_window->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // Top-level (parent) window (relative to global "screen" coordinates):
      this->main_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 ) );
      this->main_window->set_name( "Layout" );
      this->main_window->set_title( this->main_window->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->main_window->set_decorator( new nom::MinimalDecorator() );

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
      this->listbox0 = this->create_listbox( this->main_window, Point2i::null, Size2i(50,24), "listbox0", labels[0] );
      // this->listbox0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->listbox0->set_decorator( new nom::MinimalDecorator() );
      this->listbox0->set_selected_text_color( nom::Color4i::Gray );
      // this->listbox0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
      // this->listbox0->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->listbox1 = this->create_listbox( this->main_window, Point2i::null, Size2i(50,24), "listbox1", labels[1] );
      this->listbox1->set_decorator( new nom::MinimalDecorator() );
      // this->listbox1->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
      // this->listbox1->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->listbox2 = this->create_listbox( this->main_window, Point2i::null, Size2i(50,24), "listbox1", labels[2] );
      this->listbox2->set_decorator( new nom::MinimalDecorator() );
      // this->listbox2->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
      // this->listbox2->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->listbox3 = this->create_listbox( this->main_window, Point2i::null, Size2i(50,24), "listbox1", labels[3] );
      this->listbox3->set_decorator( new nom::MinimalDecorator() );
      // this->listbox3->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
      // this->listbox3->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->main_window->insert_child( listbox0 );
      this->main_window->insert_child( listbox1 );
      this->main_window->insert_child( listbox2 );
      this->main_window->insert_child( listbox3 );
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
        }

        this->window.update();
        this->main_window->update();

        // Background color fill
        this->window.fill( nom::Color4i::SkyBlue );

        this->main_window->draw( this->window );
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

    nom::UIBoxLayout::raw_ptr create_layout( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, Orientations dir )
    {
      nom::UIBoxLayout* layout = nullptr;

      // FIXME: Widgets must not be added to the layout until *after* a) font
      // initialization (widget's size hint); b) item store initialization
      // (internal update is necessary within ListBox::set_item_store).
      if( dir == Orientations::Horizontal )
      {
        layout = new nom::UIHBoxLayout( window );
      }
      else // Orientations::Vertical
      {
        layout = new nom::UIVBoxLayout( window );
      }

      // TODO: Add unit tests for each size policy variation
      // if( this->listbox0 != nullptr )
      // {
        // FIXME:
        // this->listbox0->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );
      // }

      // TODO: Add unit tests for this
      // layout->set_spacing( 1 );

      layout->append_spacer( 8 );
      layout->add_widget( this->listbox0 );
      layout->append_spacer( 8 );
      layout->add_widget( this->listbox1 );
      layout->append_spacer( 40 );
      layout->add_widget( this->listbox2 );
      layout->append_spacer( 0 );
      layout->add_widget( this->listbox3 );

      // FIXME (?): Our widget items must fit within these bounds.
      // IntRect layout_bounds( pos, size );
      // layout->set_bounds( layout_bounds );

      // Relative to parent widget
      layout->set_position( pos );

      // TODO: Add unit tests for this
      // this->button_layout->set_position( nom::Point2i( 0, 0 ) );

      // FIXME:
      // layout->set_alignment( Anchor::TopLeft );
      // EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

      // layout->set_alignment( layout, Anchor::TopLeft );
      // EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

      // layout->set_alignment( this->listbox0, Anchor::TopLeft );
      // UILayoutItem* i = layout->at( 0 );
      // if( i != nullptr )
      // {
      //   EXPECT_EQ( Anchor::TopLeft, i->alignment() );
      // }

      NOM_ASSERT( window->layout() == layout );

      // These coordinates should be relative to the parent window that the
      // layout is attached to.
      EXPECT_EQ( 12, layout->bounds().x );
      EXPECT_EQ( 25, layout->bounds().y );

      // TODO: Unit test for layout size dimensions

      return layout;
    }

    /// \brief Callback method.
    void on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
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

    /// \brief Item strings for our ListBox widgets.
    nom::ItemStrings labels[4];

    nom::ListBox::raw_ptr listbox0;
    nom::ListBox::raw_ptr listbox1;
    nom::ListBox::raw_ptr listbox2;
    nom::ListBox::raw_ptr listbox3;
};

TEST_F( ListBoxLayoutTest, LayoutAPIUsingBitmapFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->main_window->set_font( SystemFonts::cache().load_resource( "VIII" ) );

  layout = this->create_layout( this->main_window, Point2i(12,25), Size2i(50,-1), "LayoutAPIUsingBitmapFont", Orientations::Vertical );

  // Total number of items (including spacers) in our layout
  EXPECT_EQ( 8, layout->count() );

  EXPECT_EQ( Size2i( 50, 52 ), layout->size_hint() );

  // Should be the absolute dimensions specified in the initialization of unit
  // tests.
  EXPECT_EQ( Size2i( 62, 36 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  UILayoutItem* item = nullptr;

  item = layout->at( 0 );

  ASSERT_FALSE( item->spacer_item() == nullptr );

  if( item->spacer_item() != nullptr )
  {
    // NOM_DUMP( item->spacer_item()->bounds().x );
    // NOM_DUMP( item->spacer_item()->bounds().y );
    // NOM_DUMP( item->spacer_item()->size().w );
    // NOM_DUMP( item->spacer_item()->size().h );

    EXPECT_EQ( -1, item->spacer_item()->bounds().x );
    EXPECT_EQ( -1, item->spacer_item()->bounds().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 8, item->spacer_item()->size().w );
    EXPECT_EQ( 8, item->spacer_item()->size().h );
  }

  // listbox0
  item = layout->at( 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox0_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox0_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox0_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox0_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing, but not internal layout
    // spacing (because it is the first item).
    EXPECT_EQ( 37, item->widget()->position().x );
    EXPECT_EQ( 58, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 52, item->widget()->size().h );
  }

  // Skip to the end of the layout -- listbox3
  item = layout->at( layout->count() - 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox3_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox3_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox3_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox3_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing & internal layout spacing
    EXPECT_EQ( 37, item->widget()->position().x );

    EXPECT_EQ( 265, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 52, item->widget()->size().h );
  }

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( ListBoxLayoutTest, VerticalLayoutUsingArialFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->main_window->set_font( SystemFonts::cache().load_resource( "Arial" ) );

  layout = this->create_layout( this->main_window, Point2i(12,25), Size2i(50,-1), "VerticalLayoutUsingTrueTypeFont", Orientations::Vertical );

  // Total number of items (including spacers) in our layout
  EXPECT_EQ( 8, layout->count() );
  EXPECT_EQ( Size2i( 50, 56 ), layout->size_hint() );
  EXPECT_EQ( Size2i( 62, 36 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  UILayoutItem* item = nullptr;

  item = layout->at( 0 );

  ASSERT_FALSE( item->spacer_item() == nullptr );

  if( item->spacer_item() != nullptr )
  {
    // NOM_DUMP( item->spacer_item()->bounds().x );
    // NOM_DUMP( item->spacer_item()->bounds().y );
    // NOM_DUMP( item->spacer_item()->size().w );
    // NOM_DUMP( item->spacer_item()->size().h );

    EXPECT_EQ( -1, item->spacer_item()->bounds().x );
    EXPECT_EQ( -1, item->spacer_item()->bounds().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 8, item->spacer_item()->size().w );
    EXPECT_EQ( 8, item->spacer_item()->size().h );
  }

  // listbox0
  item = layout->at( 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox0_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox0_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox0_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox0_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing, but not internal layout
    // spacing (because it is the first item).
    EXPECT_EQ( 37, item->widget()->position().x );
    EXPECT_EQ( 58, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 56, item->widget()->size().h );
  }

  // Skip to the end of the layout -- listbox3
  item = layout->at( layout->count() - 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox3_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox3_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox3_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox3_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing & internal layout spacing
    EXPECT_EQ( 37, item->widget()->position().x );
    EXPECT_EQ( 277, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 56, item->widget()->size().h );
  }

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( ListBoxLayoutTest, HorizontalLayoutUsingArialFont )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->main_window->set_font( SystemFonts::cache().load_resource( "Arial" ) );

  layout = this->create_layout( this->main_window, Point2i(12,25), Size2i(50,-1), "HorizontalLayoutUsingTrueTypeFont", Orientations::Horizontal );

  // Total number of items (including spacers) in our layout
  EXPECT_EQ( 8, layout->count() );
  EXPECT_EQ( Size2i( 50, 56 ), layout->size_hint() );
  EXPECT_EQ( Size2i( 62, 36 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  UILayoutItem* item = nullptr;

  item = layout->at( 0 );

  ASSERT_FALSE( item->spacer_item() == nullptr );

  if( item->spacer_item() != nullptr )
  {
    // NOM_DUMP( item->spacer_item()->bounds().x );
    // NOM_DUMP( item->spacer_item()->bounds().y );
    // NOM_DUMP( item->spacer_item()->size().w );
    // NOM_DUMP( item->spacer_item()->size().h );

    EXPECT_EQ( -1, item->spacer_item()->bounds().x );
    EXPECT_EQ( -1, item->spacer_item()->bounds().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 8, item->spacer_item()->size().w );
    EXPECT_EQ( 8, item->spacer_item()->size().h );
  }

  // listbox0
  item = layout->at( 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox0_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox0_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox0_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox0_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing, but not internal layout
    // spacing (because it is the first item).
    EXPECT_EQ( 45, item->widget()->position().x );
    EXPECT_EQ( 50, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 56, item->widget()->size().h );
  }

  // Skip to the end of the layout -- listbox3
  item = layout->at( layout->count() - 1 );

  ASSERT_FALSE( item->widget() == nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP_VAR( "listbox3_pos.x: ", item->widget()->position().x );
    // NOM_DUMP_VAR( "listbox3_pos.y: ", item->widget()->position().y );
    // NOM_DUMP_VAR( "listbox3_size.w: ", item->widget()->size().w );
    // NOM_DUMP_VAR( "listbox3_size.h: ", item->widget()->size().h );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing & internal layout spacing
    EXPECT_EQ( 246, item->widget()->position().x );
    EXPECT_EQ( 50, item->widget()->position().y );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( 50, item->widget()->size().w );
    EXPECT_EQ( 56, item->widget()->size().h );
  }

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom

int main( int argc, char** argv )
{
  // Only used for setting the working directory path to that of the executable,
  // so we can quickly get to saved screen-shots.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
