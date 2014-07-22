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

      NOM_CONNECT_UIWIDGET_TREE_EVENT( dview, UIEvent::MOUSE_UP, this->on_click );

      return dview;
    }

  protected:
    void on_click( const UIWidgetTreeEvent& ev )
    {
      // Event evt = ev.event();

      // // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, ev.id() );
      // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, ev.text() );
      NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "selection: ", ev.selection() );
      NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "column: ", ev.column() );
      NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "row: ", ev.row() );

      Text* str = NOM_DYN_PTR_CAST( Text*, ev.data() );
      SpriteBatch* icon = NOM_DYN_PTR_CAST( SpriteBatch*, ev.data() );

      if( str )
      {
        NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "text: ", str->text() );

        if( ev.selection() == true && ev.column() == 1 )
        {
          str->set_color( Color4i(195, 209, 228, 255) );

          // DataViewItem* item = this->store->item( 0, ev.row() );
          // SpriteBatch* sp_icon = dynamic_cast<SpriteBatch*>( item->data() );
          SpriteBatch* sp_icon = this->store->item_sprite( 0, ev.row() );

          if( sp_icon )
          {
            // Disabled frame
            sp_icon->set_frame( 1 );
          }

          // item = this->store->item( 2, ev.row() );
          // Text* num_text = dynamic_cast<Text*>( item->data() );
          Text* num_text = this->store->item_text( 2, ev.row() );

          if( num_text )
          {
            num_text->set_text( "0" );
          }
        }
        else if( ev.selection() == false && ev.column() == 1 )
        {
          str->set_color( Color4i::White );

          // DataViewItem* item = this->store->item( 0, ev.row() );
          // SpriteBatch* sp_icon = dynamic_cast<SpriteBatch*>( item->data() );
          SpriteBatch* sp_icon = this->store->item_sprite( 0, ev.row() );

          if( sp_icon )
          {
            // Enabled frame
            sp_icon->set_frame( 0 );
          }

          // DataViewTextItem* item = this->store->item_text( 2, ev.row() );
          // Text* num_text = dynamic_cast<Text*>( item->data() );
          Text* num_text = this->store->item_text( 2, ev.row() );

          if( num_text )
          {
            num_text->set_text( "1" );
          }
        }
      }
      else if( icon )
      {
        NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "sprite_sheet: ", icon->sheet_filename() );
        NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "frame: ", icon->frame() );
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
    std::vector<std::shared_ptr<DataViewItem>> col0_items;
    std::vector<std::shared_ptr<DataViewItem>> col1_items;
    std::vector<std::shared_ptr<DataViewItem>> col2_items;
    DataViewListStore* store;
    UIStyle::shared_ptr col_style;
    UIStyle::shared_ptr item_style;
};

TEST_F( DataViewListTest, DataViewListStoreAPI )
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

  col0_items.push_back( std::make_shared<DataViewTextItem>( "Geezard" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "Bite Bug") );

  col1_items.push_back( std::make_shared<DataViewTextItem>( "1" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "1" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->insert_items( 0, col0_items );
  store->insert_items( 1, col1_items );

  // Number of columns in store
  EXPECT_EQ( 2, store->columns_size() );

  // Items in store per column
  EXPECT_EQ( 2, store->items_size( 0 ) );
  EXPECT_EQ( 2, store->items_size( 1 ) );

  EXPECT_EQ( "CARDS  PG. 1", col0.title() );
  EXPECT_EQ( "NUM.", col1.title() );

  cols = store->column_names();

  EXPECT_EQ( "CARDS  PG. 1", cols[0] );
  EXPECT_EQ( "NUM.", cols[1] );

  EXPECT_TRUE( store->erase_item( 0, 0 ) )
  << "Column zero (0) should exist";

  EXPECT_TRUE( store->erase_item( 1, 0 ) )
  << "Column one (1) should exist";

  EXPECT_FALSE( store->erase_item( 2, 0 ) )
  << "Column two (2) should not exist";

  EXPECT_EQ( 1, store->items_size( 0 ) );
  EXPECT_EQ( 1, store->items_size( 1 ) );

  store->clear_items( 0 );
  store->clear_items( 1 );

  EXPECT_EQ( 0, store->items_size( 0 ) )
  << "All stored items from all columns should be cleared";
  EXPECT_EQ( 0, store->items_size( 1 ) )
  << "All stored items from all columns should be cleared";

  store->clear_columns();

  EXPECT_EQ( 0, store->columns_size() )
  << "All stored columns should be cleared";

  EXPECT_FALSE( store->insert_item( 0, 0, new DataViewTextItem( "Hello" ) ) )
  << "Row zero (0) at column zero (0) should not exist";

  store->append_column( this->col0 );

  EXPECT_EQ( 1, store->columns_size() );

  cols = store->column_names();

  EXPECT_EQ( "CARDS  PG. 1", cols[0] );

  EXPECT_TRUE( store->append_item( 0, new DataViewTextItem( "Hello" ) ) );
  EXPECT_TRUE( store->insert_item( 0, 1, new DataViewTextItem( "Hello" ) ) );

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

  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_0" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_1" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_2" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_x" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_xx" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_0" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_1" ) );

  col1_items.push_back( std::make_shared<DataViewTextItem>( "Test_3" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "Test_4" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "Test_5" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "Test_6" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_XXX" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_2" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "value_3" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->insert_items( 0, col0_items );
  store->insert_items( 1, col1_items );

  // Set custom style (font)
  store->set_items_style( 0, this->item_style );
  store->set_items_style( 1, this->item_style );

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
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_0" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_1" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_2" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_X" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "TEST_XX" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_0" ) );
  col0_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_1" ) );

  // 8 items
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_3" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_4" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_5" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_6" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "TEST_XXX" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_1" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_2" ) );
  col1_items.push_back( std::make_shared<DataViewTextItem>( "VALUE_3" ) );

  // 7 items
  col2_items.push_back( std::make_shared<DataViewTextItem>( "1" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "1" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "6" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "3" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "1" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "4" ) );
  col2_items.push_back( std::make_shared<DataViewTextItem>( "6" ) );

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->append_column( col2 );
  store->insert_items( 0, col0_items );
  store->insert_items( 1, col1_items );
  store->insert_items( 2, col2_items );

  // Set custom styles (font)
  store->set_items_style( 0, this->item_style );
  // Use UIWidget defaults for col1 items
  store->set_items_style( 2, this->item_style );

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
                          48,
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

  store = new DataViewListStore();

  store->append_column( col0 );
  store->append_column( col1 );
  store->append_column( col2 );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame

  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame
  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );

  this->menu_elements.set_frame( 1 ); // "Disabled" sprite frame
  store->append_bitmap_item( 0, this->menu_elements.clone() );

  this->menu_elements.set_frame( 0 ); // "Enabled" sprite frame
  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );
  store->append_bitmap_item( 0, this->menu_elements.clone() );

  EXPECT_EQ( 11, store->items_size( 0 ) );

  // 11 items
  store->append_text_item( 1, "Geezard" );
  store->append_text_item( 1, "Fungar" );
  store->append_text_item( 1, "Bite Bug" );
  store->append_text_item( 1, "Red Bat" );
  store->append_text_item( 1, "Blobra" );
  store->append_text_item( 1, "Gayla" ) ;
  store->append_text_item( 1, "Gesper" );
  store->append_text_item( 1, "Fastitocalon-F" );
  store->append_text_item( 1, "Blood Soul" );
  store->append_text_item( 1, "Caterchipillar" );
  store->append_text_item( 1, "Cockatrice" );

  EXPECT_EQ( 11, store->items_size( 1 ) );

  DataViewItem* col0_item = nullptr;
  SpriteBatch* icon = nullptr;
  uint col1_size = store->items_size( 1 );

  for( auto idx = 0; idx != col1_size; ++idx )
  {
    if( idx < col1_size )
    {
      DataViewItem* col0_item = store->item( 0, idx );
      icon = dynamic_cast<SpriteBatch*>( col0_item->data() );

      if( icon && icon->frame() == 0 )
      {
        store->append_text_item( 2, "1" );
      }
      else if( icon && icon->frame() == 1 )
      {
        store->append_text_item( 2, "0" );
      }
    }
  }

  EXPECT_EQ( 11, store->items_size( 2 ) );

  EXPECT_TRUE( store->erase_item( 2, 10 ) );

// Relocate:
// EXPECT_TRUE( store->insert_item( 2, 10, new DataViewTextItem("2") ) );
// EXPECT_TRUE( store->insert_item( 2, new DataViewTextItem("666") ) );

  EXPECT_TRUE( store->insert_text_item( 2, 10, "2" ) );
  EXPECT_TRUE( store->append_text_item( 2, "666" ) );
  EXPECT_TRUE( store->insert_bitmap_item( 1, 11, this->menu_elements.clone() ) );

  // Set custom styles (font)
  store->set_items_style( 1, this->item_style );
  store->set_items_style( 2, this->item_style );

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
