#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/gui.hpp>

namespace nom {

using namespace nom;

class UIItemContainerTest: public ::testing::Test
{
  public:
    UIItemContainerTest( void )
    {
      // ...
    }

    ~UIItemContainerTest( void )
    {
      // ...
    }

  protected:
    // Member methods & variables
    // UIItemContainer i;
};

TEST_F( UIItemContainerTest, UIItemContainerTest_0 )
{
  UIItemContainer i;
  EXPECT_EQ( 0, i.size() );
  EXPECT_EQ( true, i.empty() );

  i.append( "hello, there!" );

  EXPECT_EQ( 1, i.size() );
  EXPECT_EQ( false, i.empty() );

  EXPECT_EQ( "hello, there!", i.item_label( 0 ) );

  i.erase( 0 );

  EXPECT_EQ( 0, i.size() );

  i.append( { "hello, there", "hello, there", "hello, there", "hello" } );
  EXPECT_EQ( 4, i.size() );
  EXPECT_EQ( "hello, there", i.item_label( 0 ) );
  EXPECT_EQ( "hello", i.item_label( 3 ) );

  i.clear();

  EXPECT_EQ( 0, i.size() );
}

TEST_F( UIItemContainerTest, UIItemContainerTest_1 )
{
  UIItemContainer i;

  i.append( { "hello, there", "hello, thar", "there", "goodbye" } );

  i.insert( 1, "g" );

  EXPECT_EQ( "g", i.item_label( 1 ) );
  EXPECT_EQ( "goodbye", i.item_label( 4 ) );

  // EXPECT_EQ( 0, i.find( "hello, there" ) );
}

TEST_F( UIItemContainerTest, UIItemContainerTest_2 )
{
  UIItemContainer i;
  i.insert( 0, "h" );
  i.insert( 1, "e" );
  int pos = i.append( { "l", "l", "e" } );

  i.set_selection( pos );

  EXPECT_EQ( pos, i.selection() );

  EXPECT_EQ( true, i.set_selection( "e" ) );
}

TEST_F( UIItemContainerTest, UIItemContainerTest_3 )
{
  UIItemContainer i;
  i.append( { "h", "e", "l", "l", "o" } );

  nom::ItemStrings labels = i.item_labels();

  NOM_ASSERT( labels[0] == "h" );
  NOM_ASSERT( labels[1] == "e" );
  NOM_ASSERT( labels[2] == "l" );
  NOM_ASSERT( labels[3] == "l" );
  NOM_ASSERT( labels[4] == "o" );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
