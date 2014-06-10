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

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \note The resources and variables used derived from examples/gui.cpp,
/// App::create_listbox_ex0.
class VBoxLayoutTest: public ::testing::Test
{
  public:
    /// \remarks Initialization of resources.
    VBoxLayoutTest( void ) :
      app_state{ true }
    {
      this->gui_window[0] = nullptr;

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

      this->input_mapper.insert( "VBoxLayoutTest", state, true );
    }

    virtual ~VBoxLayoutTest( void )
    {
      // NOM_LOG_TRACE( NOM );

      // FIXME:
      // delete this->listbox0;
      // delete this->listbox1;
      // delete this->listbox2;
      // delete this->listbox3;

      this->listbox0 = nullptr;
      this->listbox1 = nullptr;
      this->listbox2 = nullptr;
      this->listbox3 = nullptr;

      delete this->gui_window[0];
      // delete this->gui_window[1];

      this->gui_window[0] = nullptr;
      // this->gui_window[1] = nullptr;
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      // nom::uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
      nom::uint32 window_flags = SDL_WINDOW_SHOWN;

      // Necessary for loading font resources
      ASSERT_TRUE( this->window.create( "VBoxLayoutTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) );

      // Scale window contents up by the new width & height
      this->window.set_logical_size( WINDOW_WIDTH, WINDOW_HEIGHT );

      this->choice_selection[0] = {
                                    std::string( "item_0" ),
                                    std::string( "item_1" ),
                                  };

      this->choice_selection[1] = {
                                    std::string( "ITEM_0" ),
                                    std::string( "ITEM_1" )
                                  };

      this->choice_selection[2] = {
                                    std::string( "item_0" ),
                                    std::string( "item_1" ),
                                    std::string( "item_2" ),
                                    std::string( "item_3" )
                                  };

      // Top-left origin; relative to global (screen) coordinates.
      this->gui_window_pos[0] = nom::Point2i( 25, 25 );
      this->gui_window_size[0] = nom::Size2i  (
                                                // 1024 / 2 = 512
                                                WINDOW_WIDTH/2,
                                                // 768 / 2 = 384 - 25 = 359
                                                WINDOW_HEIGHT/2 - 25
                                              );

      this->gui_window[0] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[0], this->gui_window_size[0] );

      // Display a window frame (border) with a title to help show global bounds
      this->gui_window[0]->set_decorator( new MinimalDecorator() );

      // Debugging aid
      this->gui_window[0]->set_name( "VBoxLayout" );

      // Layout managed coordinates; size coordinates become set as
      // widget's minimum_size requirements.
      this->listbox_pos[0] = Point2i::null;
      this->listbox_size[0] = Size2i( 50, 48 );

      this->listbox_pos[1] = Point2i::null;
      this->listbox_size[1] = Size2i( 50, 48 );

      this->listbox_pos[2] = Point2i::null;
      this->listbox_size[2] = Size2i( 50, 48 );

      this->listbox_pos[3] = Point2i::null;
      this->listbox_size[3] = Size2i( 50, 48 );

      // Initialize listbox widgets with no storage -- no text label items for
      // generating a selection list from.
      this->listbox0 = new nom::ListBox( this->gui_window[0], -1, this->listbox_pos[0], this->listbox_size[0], new nom::UIItemContainer() );
      this->listbox1 = new nom::ListBox( this->gui_window[0], -1, this->listbox_pos[1], this->listbox_size[1], new nom::UIItemContainer() );
      this->listbox2 = new nom::ListBox( this->gui_window[0], -1, this->listbox_pos[2], this->listbox_size[2], new nom::UIItemContainer() );
      this->listbox3 = new nom::ListBox( this->gui_window[0], -1, this->listbox_pos[3], this->listbox_size[3], new nom::UIItemContainer() );

      this->listbox0->set_decorator( new nom::MinimalDecorator() );
      this->listbox1->set_decorator( new nom::MinimalDecorator() );
      this->listbox2->set_decorator( new nom::MinimalDecorator() );
      this->listbox3->set_decorator( new nom::MinimalDecorator() );

      // Initialize each widget with a unique name tag to aid debugging output.
      this->listbox0->set_name( "listbox_ex0" );
      this->listbox1->set_name( "listbox_ex1" );
      this->listbox2->set_name( "listbox_ex2" );
      this->listbox3->set_name( "listbox_ex3" );

      // ListBox with two items
      nom::UIItemContainer storage = nom::UIItemContainer();
      storage.append( this->choice_selection[0] );

      this->listbox0->set_item_store( storage.clone() );
      this->listbox1->set_item_store( storage.clone() );
      this->listbox2->set_item_store( storage.clone() );
      this->listbox3->set_item_store( storage.clone() );

      this->gui_window[0]->insert_child( listbox0 );
      this->gui_window[0]->insert_child( listbox1 );
      this->gui_window[0]->insert_child( listbox2 );
      this->gui_window[0]->insert_child( listbox3 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // FIXME:
      // delete this->listbox0;
      // delete this->listbox1;
      // delete this->listbox2;
      // delete this->listbox3;

      this->listbox0 = nullptr;
      this->listbox1 = nullptr;
      this->listbox2 = nullptr;
      this->listbox3 = nullptr;

      // FIXME:
      // delete this->gui_window[0];

      this->gui_window[0] = nullptr;
    }

    sint on_run( void )
    {
      while( this->app_state == true )
      {
        while( this->evt.poll_event( this->ev ) )
        {
          // this->evt.process_event( this->ev );

          this->input_mapper.on_event( this->ev );

          this->gui_window[0]->process_event( this->ev );
        }

        this->window.update();
        this->gui_window[0]->update();

        // Background color fill
        this->window.fill( nom::Color4i::SkyBlue );

        this->gui_window[0]->draw( this->window );
      }

      return NOM_EXIT_SUCCESS;
    }

  protected:
    const nom::int32 WINDOW_WIDTH = 1024;
    const nom::int32 WINDOW_HEIGHT = 768;

    static const nom::sint GUI_WINDOWS = 5;

    // Game loop support
    bool app_state;
    RenderWindow window;
    Event ev;

    EventHandler evt;
    InputStateMapper input_mapper;

    // GUI resources

    Point2i gui_window_pos[GUI_WINDOWS];
    Size2i gui_window_size[GUI_WINDOWS];

    nom::UIWidget* gui_window[GUI_WINDOWS];

    Point2i listbox_pos[4];
    Size2i listbox_size[4];

    /// \brief Item strings for our ListBox widgets.
    nom::ItemStrings choice_selection[4];

    nom::ListBox* listbox0;
    nom::ListBox* listbox1;
    nom::ListBox* listbox2;
    nom::ListBox* listbox3;
};

/// \todo UILayout::add_item unit testing
TEST_F( VBoxLayoutTest, CoreAPI )
{
  // Ensure that we do hold valid font resources; an invalid font resource
  // would certainly skew the test results considerably!
  // ASSERT_TRUE( this->gui_window[0]->font().valid() );
  ASSERT_TRUE( this->listbox0->font().valid() );
  ASSERT_TRUE( this->listbox1->font().valid() );
  ASSERT_TRUE( this->listbox2->font().valid() );
  ASSERT_TRUE( this->listbox3->font().valid() );

  nom::UIVBoxLayout* layout = new nom::UIVBoxLayout();

  layout->add_widget( this->listbox0 );
  layout->add_widget( this->listbox1 );
  layout->add_widget( this->listbox2 );
  layout->add_widget( this->listbox3 );

  // Our widget items must fit within these bounds.
  IntRect layout_bounds( 25, 25, 50, 24 );
  layout->set_bounds( layout_bounds );

  this->gui_window[0]->set_layout( layout );

  // Total of four widgets in the layout.
  EXPECT_EQ( 4, layout->count() );

  layout->set_alignment( Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( layout, Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( this->listbox0, Anchor::TopLeft );
  UILayoutItem* i = layout->at( 0 );
  if( i != nullptr )
  {
    EXPECT_EQ( Anchor::TopLeft, i->alignment() );
  }

  // Layout's parent is listbox0.
  ASSERT_TRUE( layout->parent() );

  // Top-level window for the layout (auto-generated tag name).
  EXPECT_EQ( "VBoxLayout", layout->parent()->name() );

  // Layout is *NOT* a widget
  ASSERT_TRUE( layout->widget() == nullptr );

  // Positioning index of added widgets
  EXPECT_EQ( 0, layout->find_widget( this->listbox0 ) );
  EXPECT_EQ( 1, layout->find_widget( this->listbox1 ) );
  EXPECT_EQ( 2, layout->find_widget( this->listbox2 ) );
  EXPECT_EQ( 3, layout->find_widget( this->listbox3 ) );

  // NULL test
  EXPECT_EQ( -1, layout->find_widget( nullptr ) );

  layout->erase( 2 );
  EXPECT_EQ( 3, layout->count() );

  EXPECT_EQ( 0, layout->erase_widget( this->listbox0 ) );
  EXPECT_EQ( 2, layout->count() );

  // Item is already erased; container should remain unmodified.
  EXPECT_EQ( -1, layout->erase_widget( this->listbox0 ) );
  EXPECT_EQ( 2, layout->count() );

  // NULL erasure test
  EXPECT_EQ( -1, layout->erase_widget( nullptr ) );

  // Not empty
  ASSERT_TRUE( layout->empty() == false );

  // Destroyed already
  EXPECT_EQ( -1, layout->find_widget( this->listbox0 ) );

  // Default spacing between items
  EXPECT_EQ( 1, layout->spacing() );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks A simple layout, using nom::ListBox widgets, without an initialized
/// font. This is not a realistic use case, but is intended to verify the sanity
/// of the essential layout calculations involved.
TEST_F( VBoxLayoutTest, CoreLayoutAPI )
{
  // Ensure that we do hold valid font resources; an invalid font resource
  // would certainly skew the test results considerably!
  // ASSERT_TRUE( this->gui_window[0]->font().valid() );
  ASSERT_TRUE( this->listbox0->font().valid() );
  ASSERT_TRUE( this->listbox1->font().valid() );
  ASSERT_TRUE( this->listbox2->font().valid() );
  ASSERT_TRUE( this->listbox3->font().valid() );

  nom::UIVBoxLayout* layout = new nom::UIVBoxLayout();

  layout->add_widget( this->listbox0 );
  layout->add_widget( this->listbox1 );
  layout->add_widget( this->listbox2 );
  layout->add_widget( this->listbox3 );

  // Our widget items must fit within these bounds.
  IntRect layout_bounds( 50, 50, 50, 24 );
  layout->set_bounds( layout_bounds );

  this->gui_window[0]->set_layout( layout );

  // Total of four widgets in the layout.
  EXPECT_EQ( 4, layout->count() );

  NOM_DUMP( layout->size_hint() );
  NOM_DUMP( layout->minimum_size() );

  // FIXME: dimensions are different for reasons unknown (guessing it may be
  // differences in font metrics?)
  //
  // See also: FIXME note regarding ListBox point size within the unit test
  // 'LayoutAPIUsingArialFont'. (This could possibly be related to the problem,
  // too!)
  #if defined( NOM_PLATFORM_OSX )
    EXPECT_EQ( Size2i( 50, 30 ), layout->size_hint() );
  #elif defined( NOM_PLATFORM_WINDOWS )
    EXPECT_EQ( Size2i( 50, 28 ), layout->size_hint() );
  #endif

  // Should be the absolute dimensions specified in the initialization of unit
  // tests.
  EXPECT_EQ( Size2i( 62, 60 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  layout->set_alignment( Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( layout, Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( this->listbox0, Anchor::TopLeft );
  UILayoutItem* i = layout->at( 0 );
  if( i != nullptr )
  {
    EXPECT_EQ( Anchor::TopLeft, i->alignment() );
  }

  EXPECT_EQ( 1, layout->spacing() );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks Uses the bitmap font resource: VIII.png.
TEST_F( VBoxLayoutTest, LayoutAPIUsingBitmapFont )
{
  this->gui_window[0]->set_font( SystemFonts::cache().load_resource( "VIII" ) );

  // Ensure that we do hold valid font resources; an invalid font resource
  // would certainly skew the test results considerably!
  ASSERT_TRUE( this->gui_window[0]->font().valid() );
  ASSERT_TRUE( this->listbox0->font().valid() );
  ASSERT_TRUE( this->listbox1->font().valid() );
  ASSERT_TRUE( this->listbox2->font().valid() );
  ASSERT_TRUE( this->listbox3->font().valid() );

  nom::UIItemContainer storage0 = nom::UIItemContainer();
  storage0.append( this->choice_selection[0] );

  nom::UIItemContainer storage2 = nom::UIItemContainer();
  storage2.append( this->choice_selection[2] );

  // ListBox with four items
  this->listbox0->set_item_store( storage2.clone() );

  // ListBox with two items
  this->listbox1->set_item_store( storage0.clone() );
  this->listbox2->set_item_store( storage0.clone() );

  // ListBox with four items
  this->listbox3->set_item_store( storage2.clone() );

  // FIXME: Widgets must not be added to the layout until *after* a) font
  // initialization (widget's size hint); b) item store initialization
  // (internal update is necessary within ListBox::set_item_store).
  nom::UIVBoxLayout* layout = new nom::UIVBoxLayout();

  // FIXME:
  // layout->set_spacing( 0 );

  layout->add_widget( this->listbox0 );
  layout->add_widget( this->listbox1 );
  layout->add_widget( this->listbox2 );
  layout->add_widget( this->listbox3 );

  // Our widget items must fit within these bounds.
  IntRect layout_bounds( 50, 4, 50, -1 );
  layout->set_bounds( layout_bounds );

  this->gui_window[0]->set_layout( layout );

  // Total of four widgets in the layout.
  EXPECT_EQ( 4, layout->count() );

  EXPECT_EQ( Size2i( 50, 52 ), layout->size_hint() );

  // Should be the absolute dimensions specified in the initialization of unit
  // tests.
  EXPECT_EQ( Size2i( 62, 60 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  layout->set_alignment( Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( layout, Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( this->listbox0, Anchor::TopLeft );
  UILayoutItem* i = layout->at( 0 );
  if( i != nullptr )
  {
    EXPECT_EQ( Anchor::TopLeft, i->alignment() );
  }

  EXPECT_EQ( 1, layout->spacing() );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( VBoxLayoutTest, LayoutAPIUsingArialFont )
{
  // FIXME: ListBox has a hard-coded 12 point size for the font.
  // this->gui_window[0]->set_font( this->truetype_font[0] ); // 14 pt size
  this->gui_window[0]->set_font( SystemFonts::cache().load_resource( "Arial" ) );

  // Ensure that we do hold valid font resources; an invalid font resource
  // would certainly skew the test results considerably!
  ASSERT_TRUE( this->gui_window[0]->font().valid() );
  ASSERT_TRUE( this->listbox0->font().valid() );
  ASSERT_TRUE( this->listbox1->font().valid() );
  ASSERT_TRUE( this->listbox2->font().valid() );
  ASSERT_TRUE( this->listbox3->font().valid() );

  nom::UIItemContainer storage0 = nom::UIItemContainer();
  storage0.append( this->choice_selection[0] );

  nom::UIItemContainer storage2 = nom::UIItemContainer();
  storage2.append( this->choice_selection[2] );

  // ListBox with four items
  this->listbox0->set_item_store( storage2.clone() );

  // ListBox with two items
  this->listbox1->set_item_store( storage0.clone() );
  this->listbox2->set_item_store( storage0.clone() );

  // ListBox with four items
  this->listbox3->set_item_store( storage2.clone() );

  // FIXME: Widgets must not be added to the layout until *after* a) font
  // initialization (widget's size hint); b) item store initialization
  // (internal update is necessary within ListBox::set_item_store).
  nom::UIVBoxLayout* layout = new nom::UIVBoxLayout();

  // FIXME:
  // layout->set_spacing( 8 );

  layout->add_widget( this->listbox0 );
  layout->add_widget( this->listbox1 );
  layout->add_widget( this->listbox2 );
  layout->add_widget( this->listbox3 );

  // Our widget items must fit within these bounds.
  IntRect layout_bounds( 50, 8, 50, -1 );
  layout->set_bounds( layout_bounds );

  this->gui_window[0]->set_layout( layout );

  // Total of four widgets in the layout.
  EXPECT_EQ( 4, layout->count() );

  EXPECT_EQ( Size2i( 50, 56 ), layout->size_hint() );

  // Should be the absolute dimensions specified in the initialization of unit
  // tests.
  EXPECT_EQ( Size2i( 62, 60 ), layout->minimum_size() );

  EXPECT_EQ( 1, layout->spacing() );

  layout->set_alignment( Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( layout, Anchor::TopLeft );
  EXPECT_EQ( Anchor::TopLeft, layout->alignment() );

  layout->set_alignment( this->listbox0, Anchor::TopLeft );
  UILayoutItem* i = layout->at( 0 );
  if( i != nullptr )
  {
    EXPECT_EQ( Anchor::TopLeft, i->alignment() );
  }

  EXPECT_EQ( 1, layout->spacing() );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom

int main( int argc, char** argv )
{
  atexit( nom::quit );

  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
