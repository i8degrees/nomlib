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

#include "nomlib/tests/common.hpp"      // nom::UnitTest framework
#include "nomlib/tests/gui/common.hpp"  // GUI helpers

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \note The resources and variables used derived from examples/gui_button.cpp
/// and tests/gui/ListBoxLayoutTest.cpp.
class ButtonLayoutTest: public VisualUnitTest
{
  public:
    ButtonLayoutTest( void ) :
      // Use default resolution provided by VisualUnitTest
      WINDOW_WIDTH{ resolution().w },
      WINDOW_HEIGHT{ resolution().h }
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    virtual ~ButtonLayoutTest( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      // Use default initializations provided by VisualUnitTest
      VisualUnitTest::SetUp();

      this->main_window = nullptr;

      // BitmapButton (button2) resources
      ASSERT_TRUE( this->button_bg[0].load( RESOURCE_BUTTON_IMAGE, 0 ) == true );
      ASSERT_TRUE( this->button_bg[1].load( RESOURCE_AQUA_BUTTON_IMAGE_0, 0 ) == true );
      ASSERT_TRUE( this->button_bg[2].load( RESOURCE_AQUA_BUTTON_IMAGE_1, 0 ) == true );

      // Top-level (parent) window (relative to global "screen" coordinates):
      this->main_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 30, WINDOW_HEIGHT - 30 ) );
      this->main_window->set_title();

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->main_window->set_decorator( new nom::MinimalDecorator() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // parent window
      this->main_window->set_decorator( new nom::MinimalDecorator() );

      // FIXME: Re-enable the underlying event code in UIWidget for the
      // WINDOW_MOUSE_DOWN event.
      //
      // this->main_window->dispatcher()->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      this->layout_widget = new nom::UIWidget( this->main_window );

      // The initial width and height of the layout does not matter; it is
      //resized per test.
      this->layout_widget->set_geometry( 12, 25, 100, 100 );
      // this->layout_widget->set_title();

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->layout_widget->set_decorator( new nom::MinimalDecorator() );

      // Our widgets to be used in the layout:

      // Note that each button's size (other than the bitmap button) will be
      // too small to fit the text used (which was intentional), but with layout
      // management, the size dimensions will be calculated for us automatically
      // (meaning that the size dimensions used will be from the widget's
      // size_hint method, when the size policy is set to Preferred).

      // this->button0 = this->create_button( this->main_window, Point2i::null, Size2i(50,16), "button0", "Click me!" );
      this->button0 = this->create_button( this->layout_widget, Point2i::null, Size2i::null, "button0", "Click me!" );

      this->button0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->button0->set_decorator( new nom::FinalFantasyDecorator() );

      NOM_CONNECT_UIWIDGET_EVENT( this->button0, nom::UIEvent::MOUSE_DOWN, this->button0_on_click );
      NOM_CONNECT_UIWIDGET_EVENT( this->button0, nom::UIEvent::MOUSE_UP, this->button0_on_click );

      // this->button1 = this->create_button( this->layout_widget, Point2i::null, Size2i::null, "button1", "button1" );

      // Create a button using explicit size dimensions; this internally sets
      // the size policy to use these dimensions within a layout.
      this->button1 = this->create_button( this->layout_widget, Point2i::null, Size2i(100,16), "button1", "button1" );

      this->button1->set_decorator( new nom::MinimalDecorator() );

      NOM_CONNECT_UIWIDGET_EVENT( this->button1, nom::UIEvent::MOUSE_DOWN, this->button1_on_click );
      NOM_CONNECT_UIWIDGET_EVENT( this->button1, nom::UIEvent::MOUSE_UP, this->button1_on_click );

      // Style is used for button2
      nom::UIStyle::shared_ptr custom_style = nullptr;
      custom_style.reset( new nom::UIStyle() );

      custom_style->set_font_color( nom::Color4i::Black );
      custom_style->set_font_size( 13 );
      custom_style->set_font_style( nom::Text::Style::Bold );

      // this->button2 = this->create_bitmap_button( this->layout_widget, Point2i::null, Size2i(50,16), "bitmap_button", "Hello", this->button_bg[1], custom_style );
      this->button2 = this->create_bitmap_button( this->layout_widget, Point2i::null, Size2i::null, "bitmap_button", "Hello", this->button_bg[1], custom_style );

      this->button2->set_pressed_bitmap( this->button_bg[2] );

      NOM_CONNECT_UIWIDGET_EVENT( this->button2, nom::UIEvent::MOUSE_UP, priv::on_click );

      // this->button3 = this->create_button( this->layout_widget, Point2i::null, Size2i::null, "button3", "button3" );
      this->button3 = this->create_button( this->layout_widget, Point2i::null, Size2i(56,16), "button3", "button3" );
      this->button3->set_decorator( new nom::MinimalDecorator() );

      this->spacers.push_back( 8 );
      this->items.push_back( this->button0 );
      this->spacers.push_back( 8 );
      this->items.push_back( this->button1 );
      this->spacers.push_back( 40 );
      this->items.push_back( this->button2 );
      this->items.push_back( this->button3 );

      // Register GUI event listeners onto our main loop (::on_run).
      this->append_event_callback( [&] ( Event ev ) { this->main_window->process_event( ev ); } );
      this->append_event_callback( [&] ( Event ev ) { this->layout_widget->process_event( ev ); } );

      // Register GUI updates onto our main loop (::on_run).
      this->append_update_callback( [&] ( float delta ) { this->main_window->update(); } );
      this->append_update_callback( [&] ( float delta ) { this->layout_widget->update(); } );

      // Register GUI rendering onto our main loop (::on_run).
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->main_window->draw( this->render_window() ); } );
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->layout_widget->draw( this->render_window() ); } );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them --
      // button0, button1, button2.
      delete this->main_window;
      this->main_window = nullptr;
    }

    /// \brief Helper method for creating button widgets.
    nom::Button::raw_ptr create_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label )
    {
      nom::Button::raw_ptr button = new nom::Button( window, -1, pos, size );
      // nom::Button::raw_ptr button = new nom::Button( window );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );

      // NOM_DUMP( button->name() );
      // NOM_DUMP( size );           // layout dims (?)
      // NOM_DUMP( button->size() ); // ::size_hint dims (correct)

      NOM_ASSERT( button->size() == size );

      NOM_ASSERT( button->font().valid() );

      return button;
    }

    /// \brief Helper method for creating bitmap button widgets.
    nom::BitmapButton::raw_ptr create_bitmap_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label, const Texture& tex, const UIStyle::shared_ptr style )
    {
      // TODO: Create separate examples (including a nullptr test):
      // button = new nom::BitmapButton( window, pos, size, nullptr );
      // button = new nom::BitmapButton( this->main_window, -1, pos, size, &this->button_bg[0] );
      // button = new nom::BitmapButton( this->main_window, -1, pos, size, &this->button_bg[0] );

      nom::BitmapButton::raw_ptr button = new nom::BitmapButton( window, -1, pos, size, tex );
      // nom::BitmapButton::raw_ptr button = new nom::BitmapButton( window, tex );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      // FIXME: This has to be set before the text label; this is the same
      // problem as we had with nom::Text before relocating everything to
      // ::update.
      button->set_style( style );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );

      // NOM_DUMP( button->name() );
      // NOM_DUMP(size);           // layout dims (?)
      // NOM_DUMP(button->size()); // ::size_hint dims (correct)

      NOM_ASSERT( button->size() == button->bitmap_size() );

      NOM_ASSERT( button->font().valid() );

      return button;
    }

    /// \brief Alternative callback method used in registering event listeners.
    void button0_on_click( const UIWidgetEvent& ev ) const
    {
      Event evt = ev.event();

      NOM_ASSERT( this->button0 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button0->set_label( "Click me!" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button0->set_label( "Release me!" );
      }

      // Only dump event info upon mouse click release
      if( evt.type != SDL_MOUSEBUTTONUP ) return;

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

    /// \brief Alternative callback method used in registering event listeners.
    void button1_on_click( const UIWidgetEvent& ev ) const
    {
      Event evt = ev.event();

      NOM_ASSERT( this->button1 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button1->set_label( "button1" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button1->set_label( "Yeah buddy!" );
      }

      // Only dump event info upon mouse click release
      if( evt.type != SDL_MOUSEBUTTONUP ) return;

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

  protected:
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;

    /// \brief File path name of the resources directory; this must be a relative file path.
    const Path p;
    const std::string APP_RESOURCES_DIR = "Resources" + p.native();
    const std::string RESOURCE_BUTTON_IMAGE = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "button.png";
    const std::string RESOURCE_AQUA_BUTTON_IMAGE_0 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_0.png";
    const std::string RESOURCE_AQUA_BUTTON_IMAGE_1 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_1.png";

    // GUI resources
    nom::UIWidget::raw_ptr main_window;
    nom::UIWidget::raw_ptr layout_widget;

    /// \brief Image resource for our button widget.
    nom::Texture button_bg[3];

    nom::Button::raw_ptr button0;
    nom::Button::raw_ptr button1;
    nom::BitmapButton::raw_ptr button2;
    nom::Button::raw_ptr button3;

    /// \brief The widget items to test in a layout (per test).
    ///
    /// \remarks The stored pointers are not owned by us; do *not* free them!
    std::vector<UIWidget*> items;

    /// \brief The width and height of the UISpacerItem items to test in a
    /// layout (per test).
    std::vector<int> spacers;
};

TEST_F( ButtonLayoutTest, HorizontalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->resize( Size2i( 525, 75 ) );
  // this->layout_widget->resize( Size2i( 300, 75 ) );

  // Use custom, absolute dimensions that we provide, with relative positioning
  // of the layout from the parent widget's coordinates.
  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, this->test_name(), Orientations::Horizontal );

  // this->layout_widget->set_font( SystemFonts::cache().load_resource( "VIII" ) );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 314, 40 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 426, 40 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 478, 40 ), layout->total_size_hint() );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (button0)
  // priv::expected_layout_widget_pos( layout, 1, Point2i( 45, 50 ) );
  // priv::expected_layout_widget_dims( layout, 1, Size2i( 112, 26 ) );
  priv::expected_layout_widget_dims( layout, 1, Size2i( 112, 26 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 2, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (button1)
  priv::expected_layout_widget_dims( layout, 3, Size2i( 100, 16 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 4, Point2i(-1,-1), Size2i(40,40) );

  // Widget item (button2)
  priv::expected_layout_widget_dims( layout, 5, Size2i( 102, 25 ) );

  // Widget item (button3)
  // priv::expected_layout_widget_pos( layout, 6, Point2i( 410, 50 ) );
  priv::expected_layout_widget_dims( layout, 6, Size2i( 56, 16 ) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( ButtonLayoutTest, VerticalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  this->layout_widget->resize( Size2i( 150, 200 ) );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, this->test_name(), Orientations::Vertical );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  // Should include the minimum size requirements of the layout (dependent upon
  // its items).
  // EXPECT_EQ( Size2i( 102, 113 ), layout->total_minimum_size() );
  EXPECT_EQ( Size2i( 112, 139 ), layout->total_minimum_size() );

  // Should include the recommended size of the layout (dependent upon its
  // items). Note that this is not necessarily the rendered dimensions, but is
  // what would be considered mathematically "optimal".
  EXPECT_EQ( Size2i( 112, 167 ), layout->total_size_hint() );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (button0)
  // priv::expected_layout_widget_pos( layout, 1, Point2i( 37, 58 ) );
  // priv::expected_layout_widget_dims( layout, 1, Size2i( 112, 29 ) );
  priv::expected_layout_widget_dims( layout, 1, Size2i( 112, 26 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 2, Point2i(-1,-1), Size2i(8,8) );

  // Widget item (button1)
  // priv::expected_layout_widget_dims( layout, 3, Size2i( 100, 19 ) );
  priv::expected_layout_widget_dims( layout, 3, Size2i( 100, 16 ) );

  // Spacer item
  priv::expected_layout_spacer_output( layout, 4, Point2i(-1,-1), Size2i(40,40) );

  // Widget item (button2)
  priv::expected_layout_widget_dims( layout, 5, Size2i( 102, 25 ) );

  // Widget item (button3)
  // priv::expected_layout_widget_pos( layout, 6, Point2i( 37, 182 ) );
  priv::expected_layout_widget_dims( layout, 6, Size2i( 56, 16 ) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( ButtonLayoutTest, HorizontalLayoutAlignmentsMiddleCenter )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  Point2i expected_pos( 67, 212 );
  Size2i expected_size( 525, 75 );
  uint32 align = Anchor::MiddleCenter;

  this->layout_widget->resize( expected_size );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, this->test_name(), Orientations::Horizontal );
  layout->set_alignment( align );

  priv::test_layout_alignment( layout, this->layout_widget, align, expected_pos, expected_size );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( ButtonLayoutTest, VerticalLayoutAlignmentsMiddleCenter )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  Point2i expected_pos( 255, 150 );
  Size2i expected_size( 150, 200 );
  uint32 align = Anchor::MiddleCenter;

  this->layout_widget->resize( expected_size );

  layout = priv::create_layout( this->layout_widget, this->items, this->spacers, this->test_name(), Orientations::Vertical );
  layout->set_alignment( align );

  priv::test_layout_alignment( layout, this->layout_widget, align, expected_pos, expected_size );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  // nom::UnitTest framework integration
  nom::init_test( argc, argv );

  return RUN_ALL_TESTS();
}
