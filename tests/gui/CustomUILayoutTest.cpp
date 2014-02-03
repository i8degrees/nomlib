#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "CustomUILayout.hpp"

namespace nom {

const std::string RESOURCE_BITMAP_FONT[2] = {
                                              "Resources/gui/VIII.png",
                                              "Resources/gui/VIII_small.png"
                                            };

const std::string RESOURCE_TRUETYPE_FONT[4] = {
                                                "Resources/gui/arial.ttf",
                                                "Resources/gui/TimesNewRoman.ttf",
                                                "Resources/gui/LucidaGrande.ttf",
                                                "Resources/gui/LucidaGrande-Bold.ttf"
                                              };

class CustomUILayoutTest: public ::testing::Test
{
  public:
    CustomUILayoutTest( void ) :
      gui_window{ nullptr }
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

      // Necessary for loading font resources
      if( window.create( "CustomUILayoutTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        // return false;
      }

      // Top-right
      this->gui_window_pos[1] = nom::Point2i( 562, 25 );
      this->gui_window_size[1] = nom::Size2i( WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 25 );

      if( this->bitmap_font[0].load( RESOURCE_BITMAP_FONT[0] ) == false )
      {
        // nom::DialogMessageBox( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT[0] );
      }

      if( this->bitmap_font[1].load( RESOURCE_BITMAP_FONT[1] ) == false )
      {
        // nom::DialogMessageBox( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT[1] );
      }

      for( auto idx = 0; idx != TRUETYPE_FONTS; ++idx )
      {
        if( this->truetype_font[idx].load( RESOURCE_TRUETYPE_FONT[idx] ) == false )
        {
          // nom::DialogMessageBox( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT[idx] );
        }
      }
    }

    ~CustomUILayoutTest( void )
    {
      // delete this->gui_window[0];
      delete this->gui_window[1];
      // delete this->gui_window[2];
      // delete this->gui_window[3];
      // delete this->gui_window[4];
      // delete this->gui_window[5];

      // this->gui_window[0] = nullptr;
      this->gui_window[1] = nullptr;
      // this->gui_window[2] = nullptr;
      // this->gui_window[3] = nullptr;
      // this->gui_window[4] = nullptr;
      // this->gui_window[5] = nullptr;
    }

  protected:
    const nom::int32 WINDOW_WIDTH = 1024;
    const nom::int32 WINDOW_HEIGHT = 768;

    static const nom::sint BITMAP_FONTS = 2;
    static const nom::sint TRUETYPE_FONTS = 4;
    static const nom::sint GUI_WINDOWS = 5;

    RenderWindow window;

    // Font resources

    BitmapFont bitmap_font[BITMAP_FONTS];
    TrueTypeFont truetype_font[TRUETYPE_FONTS];

    // GUI resources

    Point2i gui_window_pos[GUI_WINDOWS];
    Size2i gui_window_size[GUI_WINDOWS];

    nom::UIWidget* gui_window[GUI_WINDOWS];
};

/// \remarks Based on examples/gui.cpp -- App::create_listbox_ex0
///
/// \todo UILayout::add_item unit testing
TEST_F( CustomUILayoutTest, CustomUILayoutTest_0 )
{
  nom::ListBox* listbox0 = nullptr;
  nom::ListBox* listbox1 = nullptr;
  nom::ListBox* listbox2 = nullptr;
  nom::ListBox* listbox3 = nullptr;

  // ListBox dimensions
  nom::Point2i pos( 100, 100 );
  nom::Size2i size( 50, 24 );

  this->gui_window[1] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[1], this->gui_window_size[1] );
  this->gui_window[1]->set_decorator( new nom::MinimalDecorator() );

  this->gui_window[1]->set_font( &this->bitmap_font[0] );

  listbox0 = new nom::ListBox( this->gui_window[1], -1, pos, size, new nom::UIItemContainer() );
  listbox1 = new nom::ListBox( this->gui_window[1], -1, pos, size, new nom::UIItemContainer() );
  listbox2 = new nom::ListBox( this->gui_window[1], -1, pos, size, new nom::UIItemContainer() );
  listbox3 = new nom::ListBox( this->gui_window[1], -1, pos, size, new nom::UIItemContainer() );

  listbox1->set_font( &this->truetype_font[0] );
  listbox2->set_font( &this->truetype_font[1] );
  listbox3->set_font( &this->bitmap_font[1] );

  listbox0->set_minimum_size( size );
  listbox0->set_size_hint( size );

  nom::CustomUILayout* layout = new nom::CustomUILayout();
  layout->add_widget( listbox0 );
  layout->add_widget( listbox1 );
  layout->add_widget( listbox2 );
  layout->add_widget( listbox3 );
  listbox0->set_layout( layout );

  // Added four widgets in the layout container
  EXPECT_EQ( 4, layout->count() );

  // This is expected only when listbox is not setting the minimum size:
  // EXPECT_EQ( Size2i( 0, 40 ), layout->minimum_size() );

  // Initial listbox dimensions + ( the number of items * layout spacing )
  EXPECT_EQ( Size2i( 50, 64 ), layout->minimum_size() );

  // FIXME: the size hint dimensions should not be set within the layout
  // manager?
  //
  // Internal layout size hint dimensions + ( the number of items * layout spacing )
  EXPECT_EQ( Size2i( 100, 110 ), layout->size_hint() );

  // FIXME:
  EXPECT_EQ( IntRect( -1, -1, -1, -1 ), layout->bounds() );

  layout->set_alignment( Alignment::TopLeft );
  EXPECT_EQ( Alignment::TopLeft, layout->alignment() );

  layout->set_alignment( layout, Alignment::TopLeft );
  EXPECT_EQ( Alignment::TopLeft, layout->alignment() );

  layout->set_alignment( listbox0, Alignment::TopLeft );
  UILayoutItem* i = layout->at( 0 );
  if( i != nullptr )
  {
    EXPECT_EQ( Alignment::TopLeft, i->alignment() );
  }

  // Layout's parent is listbox0.
  ASSERT_TRUE( layout->parent() );

  // Top-level window for the layout (auto-generated tag name).
  EXPECT_EQ( "window", layout->parent()->name() );

  // Layout is *NOT* a widget
  ASSERT_TRUE( layout->widget() == nullptr );

  // Positioning index of added widgets
  EXPECT_EQ( 0, layout->find_widget( listbox0 ) );
  EXPECT_EQ( 1, layout->find_widget( listbox1 ) );
  EXPECT_EQ( 2, layout->find_widget( listbox2 ) );
  EXPECT_EQ( 3, layout->find_widget( listbox3 ) );

  // NULL test
  EXPECT_EQ( -1, layout->find_widget( nullptr ) );

  layout->erase( 2 );
  EXPECT_EQ( 3, layout->count() );

  EXPECT_EQ( 0, layout->erase_widget( listbox0 ) );
  EXPECT_EQ( 2, layout->count() );

  // Item is already erased; container should remain unmodified.
  EXPECT_EQ( -1, layout->erase_widget( listbox0 ) );
  EXPECT_EQ( 2, layout->count() );

  // NULL erasure test
  EXPECT_EQ( -1, layout->erase_widget( nullptr ) );

  // Not empty
  ASSERT_TRUE( layout->empty() == false );

  // Destroyed already
  EXPECT_EQ( -1, layout->find_widget( listbox0 ) );

// FAIL();
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  nom::init( argc, argv );

  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
