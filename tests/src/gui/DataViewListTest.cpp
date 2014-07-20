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
      widget{ nullptr },
      col_style{ new UIStyle() },
      item_style{ new UIStyle() }
    {
      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    virtual ~DataViewListTest( void )
    {
      //
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
      NOM_DELETE_PTR( this->store );
    }

    nom::DataViewList::raw_ptr
    create_dataviewlist (
                          const UIWidget::raw_ptr window,
                          const Point2i& pos,
                          const Size2i& size,
                          const std::string& name
                        )
    {
      nom::DataViewList::raw_ptr dview = nullptr;

      dview = new nom::DataViewList( window, -1, pos, size );
      dview->set_decorator( new nom::FinalFantasyDecorator() );

      NOM_CONNECT_UIWIDGET_TREE_EVENT( dview, UIEvent::MOUSE_DOWN, this->on_click );

      return dview;
    }

  protected:

    void on_click( UIEvent* ev )
    {
      NOM_ASSERT( ev != nullptr );
      UIWidgetTreeEvent* event = NOM_DYN_PTR_CAST( UIWidgetTreeEvent*, ev );
      NOM_ASSERT( event != nullptr );

      Event evt = event->event();

      if( evt.type != SDL_MOUSEBUTTONDOWN ) return;

      // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, event->id() );
      NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, event->text() );
      NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, event->index() );

      String* str = NOM_DYN_PTR_CAST( String*, event->data() );
      if( str )
      {
        NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, str->str() );
      }
    }

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

    DataViewColumn col0;
    DataViewColumn col1;
    DataViewColumn col2;
    std::vector<IDataViewItem*> col0_items;
    std::vector<IDataViewItem*> col1_items;
    std::vector<IDataViewItem*> col2_items;
    DataViewListStore* store;
    UIStyle::shared_ptr col_style;
    UIStyle::shared_ptr item_style;
};

TEST_F( DataViewListTest, DataViewListWidgetAPI )
{
  Point2i widget_pos( Point2i::zero );
  Size2i widget_size( Size2i::zero );
  const std::string widget_name = this->test_name();

  DataViewListStore::ColumnNames cols;

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

  col0_items.push_back( new DataViewTextItem( "Geezard" ) );
  col0_items.push_back( new DataViewTextItem( "Bite Bug") );

  col1_items.push_back( new DataViewTextItem( "1" ) );
  col1_items.push_back( new DataViewTextItem( "1" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->insert_item( 0, col0_items );
  store->insert_item( 1, col1_items );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name
                                );

  widget->set_item_store( store );

  // Coordinate system sanity
  EXPECT_TRUE( widget->parent()->position() == this->gui_window->position() );
  EXPECT_TRUE( widget->size() == widget_size );

  // Total number of columns
  EXPECT_TRUE( store->columns_size() == 2 );

  // Column items total
  EXPECT_TRUE( store->items_size( 0 ) == 2 );
  EXPECT_TRUE( store->items_size( 1 ) == 2 );

  EXPECT_TRUE( col0.title() == "CARDS  PG. 1" );
  EXPECT_TRUE( col1.title() == "NUM." );

  cols = store->column_names();

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
  // Size2i widget_size( 204, 160 );
  Size2i widget_size( 204, 185 );
  const std::string widget_name = this->test_name();

  Font title_font = *nom::PlatformSettings::find_system_font( "VIII_small" );
  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );

  // Setup custom style for columns
  this->col_style->set_font( title_font );

  // Use custom style for tree items
  this->item_style->set_font( bitmap_font );

  col0 = DataViewColumn (
                          0,
                          "CARDS  PG. 1",
                          75,
                          IDataViewColumn::Alignment::Left
                          // IDataViewColumn::Alignment::Center
                        );

  col1 = DataViewColumn (
                          1,
                          "NUM.",
                          75,
                          // FIXME:
                          // IDataViewColumn::Alignment::Left
                          IDataViewColumn::Alignment::Right
                        );

  this->col0.set_style( this->col_style );
  this->col1.set_style( this->col_style );

  col0_items.push_back( new DataViewTextItem( "TEST_0" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_1" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_2" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_x" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_xx" ) );
  col0_items.push_back( new DataViewTextItem( "VALUE_0" ) );
  col0_items.push_back( new DataViewTextItem( "VALUE_1" ) );

  col1_items.push_back( new DataViewTextItem( "Test_3" ) );
  col1_items.push_back( new DataViewTextItem( "Test_4" ) );
  col1_items.push_back( new DataViewTextItem( "Test_5" ) );
  col1_items.push_back( new DataViewTextItem( "Test_6" ) );
  col1_items.push_back( new DataViewTextItem( "TEST_XXX" ) );
  col1_items.push_back( new DataViewTextItem( "VALUE_2" ) );
  col1_items.push_back( new DataViewTextItem( "value_3" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->insert_item( 0, col0_items );
  store->insert_item( 1, col1_items );

  // TODO: Relocate
  // EXPECT_TRUE( store->append_item( 1, new DataViewTextItem( "Testme") ) );
  // EXPECT_TRUE( store->erase_item( 1, store->items_size() ) );
  // store->clear();

  // Set custom style (font)
  store->set_item_style( 0, this->item_style );
  store->set_item_style( 1, this->item_style );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name
                                );

  widget->set_item_store( store );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( DataViewListTest, DataViewListWidgetEx1 )
{
  Point2i widget_pos( 25, 25 );
  // Size2i widget_size( 344, 190 );
  Size2i widget_size( 344, 250 );
  const std::string widget_name = this->test_name();

  Font title_font = *nom::PlatformSettings::find_system_font( "VIII_small" );

  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );
  Font truetype_font = *nom::PlatformSettings::get_system_font( SystemFontType::VariableTrueType );

  // Setup custom style for columns
  this->col_style->set_font( title_font );

  // Setup custom style for tree items
  this->item_style->set_font( bitmap_font );

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
                          IDataViewColumn::Alignment::Left
                          // FIXME:
                          // IDataViewColumn::Alignment::Center
                        );

  col2 = DataViewColumn (
                          2,
                          "NUM.",
                          75,
                          IDataViewColumn::Alignment::Left
                          // FIXME:
                          // IDataViewColumn::Alignment::Right
                        );

  // Set custom styles (font)
  this->col0.set_style( this->col_style );
  // Use UIWidget defaults for col1
  this->col2.set_style( this->col_style );

  // 7 items
  col0_items.push_back( new DataViewTextItem( "TEST_0" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_1" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_2" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_X" ) );
  col0_items.push_back( new DataViewTextItem( "TEST_XX" ) );
  col0_items.push_back( new DataViewTextItem( "VALUE_0" ) );
  col0_items.push_back( new DataViewTextItem( "VALUE_1" ) );

  // 8 items
  col1_items.push_back( new DataViewTextItem( "TEST_3" ) );
  col1_items.push_back( new DataViewTextItem( "TEST_4" ) );
  col1_items.push_back( new DataViewTextItem( "TEST_5" ) );
  col1_items.push_back( new DataViewTextItem( "TEST_6" ) );
  col1_items.push_back( new DataViewTextItem( "TEST_XXX" ) );
  col1_items.push_back( new DataViewTextItem( "VALUE_1" ) );
  col1_items.push_back( new DataViewTextItem( "VALUE_2" ) );
  col1_items.push_back( new DataViewTextItem( "VALUE_3" ) );

  // 7 items
  col2_items.push_back( new DataViewTextItem( "1" ) );
  col2_items.push_back( new DataViewTextItem( "1" ) );
  col2_items.push_back( new DataViewTextItem( "6" ) );
  col2_items.push_back( new DataViewTextItem( "3" ) );
  col2_items.push_back( new DataViewTextItem( "1" ) );
  col2_items.push_back( new DataViewTextItem( "4" ) );
  col2_items.push_back( new DataViewTextItem( "6" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->append_column( col2 );
  store->insert_item( 0, col0_items );
  store->insert_item( 1, col1_items );
  store->insert_item( 2, col2_items );

  // Set custom styles (font)
  store->set_item_style( 0, this->item_style );
  // Use UIWidget defaults for col1 items
  store->set_item_style( 2, this->item_style );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name
                                );

  widget->set_item_store( store );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( DataViewListTest, DataViewListWidgetEx2 )
{
  Point2i widget_pos( 25, 25 );

  // Minimum size requirements
  Size2i widget_size( 168, 200 );

  // Size2i widget_size( 200, 200 );
  // Size2i widget_size( 300, 200 );
  const std::string widget_name = this->test_name();

  Font title_font = *nom::PlatformSettings::find_system_font( "VIII_small" );
  Font bitmap_font = *nom::PlatformSettings::get_system_font( SystemFontType::FixedBitmap );

  // Setup custom style (font) for columns
  this->col_style->set_font( title_font );

  // Setup custom style (font) for tree items
  this->item_style->set_font( bitmap_font );

  col0 = DataViewColumn (
                          0,
                          "",
                          32,
                          // 0,
                          // 75,
                          IDataViewColumn::Alignment::Left
                        );

  col1 = DataViewColumn (
                          1,
                          "CARDS  P. 1 ",
                          // 50,
                          50,
                          IDataViewColumn::Alignment::Left
                          // IDataViewColumn::Alignment::Center
                          // IDataViewColumn::Alignment::Right
                        );

  col2 = DataViewColumn (
                          2,
                          "NUM.",
                          // 0,
                          25,
                          // 75,
                          // IDataViewColumn::Alignment::Left
                          IDataViewColumn::Alignment::Right
                        );

  // Set custom styles (font)
  this->col0.set_style( this->col_style );
  this->col1.set_style( this->col_style );
  this->col2.set_style( this->col_style );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame

  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );
  col0_items.push_back( new DataViewItem( this->menu_elements.clone() ) );

  // 11 items
  col1_items.push_back( new DataViewTextItem( "Geezard" ) );
  col1_items.push_back( new DataViewTextItem( "Fungar" ) );
  col1_items.push_back( new DataViewTextItem( "Bite Bug" ) );
  col1_items.push_back( new DataViewTextItem( "Red Bat" ) );
  col1_items.push_back( new DataViewTextItem( "Blobra" ) );
  col1_items.push_back( new DataViewTextItem( "Gayla" ) );
  col1_items.push_back( new DataViewTextItem( "Gesper" ) );
  col1_items.push_back( new DataViewTextItem( "Fastitocalon-F" ) );
  col1_items.push_back( new DataViewTextItem( "Blood Soul" ) );
  col1_items.push_back( new DataViewTextItem( "Caterchipillar" ) );
  col1_items.push_back( new DataViewTextItem( "Cockatrice" ) );

  // 11 items
  uint idx = 0;
  IDataViewItem* col0_item = nullptr;
  SpriteBatch* icon = nullptr;
  for( auto itr = col1_items.begin(); itr != col1_items.end(); ++itr )
  {
    if( idx < col0_items.size() )
    {
      IDataViewItem* col0_item = col0_items.at( idx );
      icon = dynamic_cast<SpriteBatch*>( col0_item->data() );

      if( icon->frame() == 0 )
      {
        col2_items.push_back( new DataViewTextItem( "1" ) );
      }
      else
      {
        col2_items.push_back( new DataViewTextItem( "0" ) );
      }
    }

    ++idx;
  }

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->append_column( col2 );
  store->insert_item( 0, col0_items );
  store->insert_item( 1, col1_items );
  store->insert_item( 2, col2_items );

  // Set custom styles (font)
  store->set_item_style( 1, this->item_style );
  store->set_item_style( 2, this->item_style );

  widget = create_dataviewlist  (
                                  this->gui_window,
                                  widget_pos,
                                  widget_size,
                                  widget_name
                                );

  widget->set_item_store( store );

  this->gui_window->insert_child( widget );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );

  NOM_DELETE_PTR( col0_item );
  NOM_DELETE_PTR( icon );
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
