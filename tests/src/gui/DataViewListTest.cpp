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
#include <vector>

#include "nomlib/tests/common.hpp"      // nom::UnitTest framework
#include "nomlib/tests/gui/common.hpp"  // GUI helpers

#include "nomlib/math.hpp"
#include "nomlib/system.hpp"
#include "nomlib/graphics.hpp"
#include "nomlib/gui.hpp"

using namespace nom;

class DataViewListTest: public VisualUnitTest
{
  public:
    DataViewListTest( void ) :
      // Use default resolution provided by VisualUnitTest
      WINDOW_WIDTH{ resolution().w },
      WINDOW_HEIGHT{ resolution().h },
      gui_window{ nullptr },
      widget{ nullptr }
    {
      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    virtual ~DataViewListTest( void )
    {
      NOM_DELETE_PTR( this->gui_window );
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      // Use default initializations provided by VisualUnitTest
      VisualUnitTest::SetUp();

      this->menu_elements = nom::SpriteBatch( RESOURCE_SPRITE );

      ASSERT_TRUE( this->menu_elements.load( APP_RESOURCES_DIR + p.native() + this->menu_elements.sheet_filename(), 0, nom::Texture::Access::Streaming ) == true )
      << "Could not load sprite" << this->menu_elements.sheet_filename();
      // this->menu_elemen.->resize ( nom::Texture::ResizeAlgorithm::scale2x );

      // Top-level (parent) window (relative to global "screen" coordinates):
      this->gui_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 30, WINDOW_HEIGHT - 30 ) );
      this->gui_window->set_title();

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->gui_window->set_decorator( new nom::MinimalDecorator() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // parent window
      this->gui_window->set_decorator( new nom::MinimalDecorator() );

      // Register GUI event listeners onto our main loop (::on_run).
      this->append_event_callback( [&] ( Event ev ) { this->gui_window->process_event( ev ); } );

      // Register GUI updates onto our main loop (::on_run).
      this->append_update_callback( [&] ( float delta ) { this->gui_window->update(); } );

      // Register GUI rendering onto our main loop (::on_run).
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->gui_window->draw( this->render_window() ); } );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them --
      NOM_DELETE_PTR( this->gui_window );
    }

    nom::DataViewList::raw_ptr
    create_dataviewlist (
                          const UIWidget::raw_ptr window,
                          const Point2i& pos,
                          const Size2i& size,
                          const std::string& name,
                          const UIStyle::shared_ptr style // Not used; reserved
                        )
    {
      nom::DataViewList::raw_ptr dview = nullptr;

      // FIXME:
      nom::Font title_font = *nom::SystemFonts::cache().load_resource("VIII_small");

      dview = new nom::DataViewList( window, -1, pos, size );
      dview->set_font( title_font );
      dview->set_decorator( new nom::FinalFantasyDecorator() );

      // FIXME:
      dview->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      return dview;
    }

  protected:
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;

    /// \brief File path name of the resources directory; this must be a relative file path.
    const Path p;
    const std::string APP_RESOURCES_DIR = "Resources" + p.native();
    const std::string RESOURCE_SPRITE = APP_RESOURCES_DIR + "gui" + p.native() + "menu_elements.json";

    nom::SpriteBatch menu_elements;

    // GUI resources
    nom::UIWidget::raw_ptr gui_window;
    nom::DataViewList::raw_ptr widget;
};

TEST_F( DataViewListTest, DataViewListWidgetAPI )
{
  Point2i widget_pos( Point2i::zero );
  Size2i widget_size( Size2i::zero );
  const std::string widget_name = this->test_name();

  DataViewColumn col0;
  DataViewColumn col1;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col0_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col1_items;
  DataViewListStore::ColumnNames cols;

  // FIXME:
  Font item_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );

  col0 = DataViewColumn (
                          0,
                          "CARDS  PG. 1",
                          75,
                          IDataViewColumn::Alignment::Left
                        );

  col1 = DataViewColumn (
                          1,
                          "NUM.",
                          75,
                          IDataViewColumn::Alignment::Right
                        );

  col0_items.push_back( new nom::Text( "Geezard", item_font ) );
  col0_items.push_back( new nom::Text( "Bite Bug", item_font ) );

  col1_items.push_back( new nom::Text( "1", item_font ) );
  col1_items.push_back( new nom::Text( "1", item_font ) );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name,
                                  nullptr     // Placeholder
                                );

  widget->append_column( col0 );
  widget->append_column( col1 );
  widget->insert_item( 0, col0_items );
  widget->insert_item( 1, col1_items );

  // Coordinate system sanity
  EXPECT_TRUE( widget->parent()->position() == this->gui_window->position() );
  EXPECT_TRUE( widget->size() == widget_size );

  // Total number of columns
  EXPECT_TRUE( widget->columns_size() == 2 );

  // Column items total
  EXPECT_TRUE( widget->items_size( 0 ) == 2 );
  EXPECT_TRUE( widget->items_size( 1 ) == 2 );

  EXPECT_TRUE( col0.title() == "CARDS  PG. 1" );
  EXPECT_TRUE( col1.title() == "NUM." );

  cols = widget->column_names();

  EXPECT_TRUE( cols[0] == "CARDS  PG. 1" );
  EXPECT_TRUE( cols[1] == "NUM." );

  // These should work as long as we ensure that drawable_items_ is cleared
  // within the update_items method.
  //
  // FIXME:
  // dview->insert_item( 0, 0, new nom::Text( "VALUE_HAXXEDDDDDDD", this->bitmap_font ) );
  // dview->insert_item( 1, 5, new nom::Text( "VALUE_HAXXED", this->bitmap_font ) );

  // nom::DataViewItem<nom::IDrawable::raw_ptr> item = dview->item( 2, 0 );
  // NOM_ASSERT( NOM_ISA( nom::SpriteBatch*, item.data() ) );
}

TEST_F( DataViewListTest, DataViewListWidgetEx0 )
{
  Point2i widget_pos( 25, 25 );
  Size2i widget_size( 204, 160 );
  const std::string widget_name = this->test_name();

  DataViewColumn col0;
  DataViewColumn col1;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col0_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col1_items;

  // FIXME:
  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );

  col0 = DataViewColumn (
                          0,
                          "CARDS  PG. 1",
                          75,
                          IDataViewColumn::Alignment::Left
                        );

  col1 = DataViewColumn (
                          1,
                          "NUM.",
                          75,
                          IDataViewColumn::Alignment::Right
                        );

  col0_items.push_back( new nom::Text( "TEST_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_1", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_2", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_x", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_xx", bitmap_font ) );
  col0_items.push_back( new nom::Text( "VALUE_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "VALUE_1", bitmap_font ) );

  col1_items.push_back( new nom::Text( "Test_3", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_4", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_5", bitmap_font ) );
  col1_items.push_back( new nom::Text( "TEST_6", bitmap_font ) );
  col1_items.push_back( new nom::Text( "TEST_XXX", bitmap_font ) );
  col1_items.push_back( new nom::Text( "VALUE_2", bitmap_font ) );
  col1_items.push_back( new nom::Text( "value_3", bitmap_font ) );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name,
                                  nullptr     // Placeholder
                                );

  widget->append_column( col0 );
  widget->append_column( col1 );
  widget->insert_item( 0, col0_items );
  widget->insert_item( 1, col1_items );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( DataViewListTest, DataViewListWidgetEx1 )
{
  Point2i widget_pos( 25, 25 );
  Size2i widget_size( 344, 190 );
  const std::string widget_name = this->test_name();

  DataViewColumn col0;
  DataViewColumn col1;
  DataViewColumn col2;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col0_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col1_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col2_items;

  // FIXME:
  // Font bitmap_font = *nom::SystemFonts::cache().load_resource("VIII");
  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );
  Font truetype_font = *nom::PlatformSettings::get_system_font( SystemFontType::VariableTrueType );

  col0 = DataViewColumn (
                          0,
                          "CARDS  PG. 1",
                          75,
                          IDataViewColumn::Alignment::Left
                        );

  col1 = DataViewColumn (
                          1,
                          "TCOL.",
                          75,
                          IDataViewColumn::Alignment::Center
                        );

  col2 = DataViewColumn (
                          2,
                          "NUM.",
                          75,
                          IDataViewColumn::Alignment::Right
                        );

  col0_items.push_back( new nom::Text( "TEST_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_1", truetype_font ) );
  col0_items.push_back( new nom::Text( "TEST_2", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_X", bitmap_font ) );
  col0_items.push_back( new nom::Text( "TEST_XX", bitmap_font ) );
  col0_items.push_back( new nom::Text( "VALUE_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "VALUE_1", truetype_font ) );

  col1_items.push_back( new nom::Text( "TEST_3", bitmap_font ) );
  col1_items.push_back( new nom::Text( "TEST_4", truetype_font ) );
  col1_items.push_back( new nom::Text( "TEST_5", bitmap_font ) );
  col1_items.push_back( new nom::Text( "TEST_6", bitmap_font ) );
  col1_items.push_back( new nom::Text( "TEST_XXX", truetype_font ) );
  col1_items.push_back( new nom::Text( "VALUE_1", bitmap_font ) );
  col1_items.push_back( new nom::Text( "VALUE_2", bitmap_font ) );
  col1_items.push_back( new nom::Text( "VALUE_3", truetype_font ) );
  col1_items.push_back( new nom::Text( "VALUE_4", bitmap_font ) );

  col2_items.push_back( new nom::Text( "1", truetype_font ) );
  col2_items.push_back( new nom::Text( "1", bitmap_font ) );
  col2_items.push_back( new nom::Text( "6", bitmap_font ) );
  col2_items.push_back( new nom::Text( "3", bitmap_font ) );
  col2_items.push_back( new nom::Text( "1", bitmap_font ) );
  col2_items.push_back( new nom::Text( "4", bitmap_font ) );
  col2_items.push_back( new nom::Text( "6", truetype_font ) );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name,
                                  nullptr     // Placeholder
                                );

  widget->append_column( col0 );
  widget->append_column( col1 );
  widget->append_column( col2 );
  widget->insert_item( 0, col0_items );
  widget->insert_item( 1, col1_items );
  widget->insert_item( 2, col2_items );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( DataViewListTest, DataViewListWidgetEx2 )
{
  Point2i widget_pos( 25, 25 );
  Size2i widget_size( 304, 190 );
  const std::string widget_name = this->test_name();

  DataViewColumn col0;
  DataViewColumn col1;
  DataViewColumn col2;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col0_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col1_items;
  std::vector<DataViewItem<IDrawable::raw_ptr>> col2_items;

  // FIXME:
  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );

  col0 = DataViewColumn (
                          0,
                          "CARDS  PG. 1",
                          55, // TODO: Match value of Ex0 test (75)..?
                          IDataViewColumn::Alignment::Left
                        );

  col1 = DataViewColumn (
                          1,
                          "TCOL.",
                          75,
                          IDataViewColumn::Alignment::Center
                        );

  col2 = DataViewColumn (
                          2,
                          "NUM.",
                          75,
                          IDataViewColumn::Alignment::Right
                        );

  col0_items.push_back( new nom::Text( "Test_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Test_1", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Test_2", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Test_X", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Test_XX", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Value_0", bitmap_font ) );
  col0_items.push_back( new nom::Text( "Value_1", bitmap_font ) );

  col1_items.push_back( new nom::Text( "Test_10", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_11", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_12", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_13", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Test_14", bitmap_font ) );
  col1_items.push_back( new nom::Text( "Value_15", bitmap_font ) );
  col1_items.push_back( new nom::Text( "VALUE_16", bitmap_font ) );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame

  // FIXME: Optimize the sprite cloning process below -- very similar in
  // concept to IFont copying; see notes in Text.hpp regarding ::set_font.
  col2_items.push_back( this->menu_elements.clone() );
  col2_items.push_back( this->menu_elements.clone() );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  col2_items.push_back( this->menu_elements.clone() );
  col2_items.push_back( this->menu_elements.clone() );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame
  col2_items.push_back( this->menu_elements.clone() );
  col2_items.push_back( this->menu_elements.clone() );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  col2_items.push_back( this->menu_elements.clone() );

  this->menu_elements.set_frame( 3 ); // "Right page" sprite frame
  col2_items.push_back( this->menu_elements.clone() );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name,
                                  nullptr     // Placeholder
                                );

  widget->append_column( col0 );
  widget->append_column( col1 );
  widget->append_column( col2 );
  widget->insert_item( 0, col0_items );
  widget->insert_item( 1, col1_items );
  widget->insert_item( 2, col2_items );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

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
