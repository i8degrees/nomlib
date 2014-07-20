#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/gui.hpp>

namespace nom {

using namespace nom;

void bar( void )
{
  NOM_LOG_TRACE( NOM );
}

class UIEventDispatcherTest: public ::testing::Test
{
  public:
    UIEventDispatcherTest( void )
    {
      // ...
    }

    ~UIEventDispatcherTest( void )
    {
      // ...
    }

  protected:
    void foo( int x )
    {
      NOM_DUMP( x );
    }

    void foo2( UIEvent* ev )
    {
      NOM_ASSERT( ev != nullptr );
      UIWidgetEvent* event = NOM_DYN_PTR_CAST( UIWidgetEvent*, ev );
      NOM_ASSERT( event != nullptr );

      // Event evt = event->event();

      NOM_DUMP( event->index() );
      NOM_DUMP( event->text() );
    }
};

TEST_F( UIEventDispatcherTest, UIEventDispatcherTest_0 )
{
  // Not tested; just used to get things compiling
  nom::UIWidgetEvent ev;

  nom::UIEventDispatcher e;
  e.register_event_listener( nom::UIEvent::KEY_DOWN, std::make_shared<UIWidgetListener>( [&] ( UIEvent* ev ) { this->foo( 2 ); } ) );
  e.register_event_listener( nom::UIEvent::MOUSE_LEFT_UP, std::make_shared<UIWidgetListener>( std::bind( bar ) ) );
  e.register_event_listener( nom::UIEvent::MOUSE_WHEEL, std::make_shared<UIWidgetListener>( [&] ( UIEvent* ev ) { this->foo2( ev ); } ) );

  // Should never emit; never found.
  e.register_event_listener( nom::UIEvent::DROP_FILE, std::make_shared<UIWidgetListener>( [&] ( UIEvent* ev ) { this->foo2( ev ); } ) );
  ASSERT_TRUE( e.remove_event_listener( nom::UIEvent::DROP_FILE ) == true );

  // Should emit
  ASSERT_TRUE( e.emit( nom::UIEvent::KEY_DOWN, ev ) );
  ASSERT_TRUE( e.emit( nom::UIEvent::MOUSE_LEFT_UP, ev  ) );

  // Should not emit; removed.
  ASSERT_TRUE( e.emit( nom::UIEvent::DROP_FILE, ev ) == false );

  // Should emit
  ASSERT_TRUE( e.emit( nom::UIEvent::MOUSE_WHEEL, ev ) );

  // Should not emit; should not ever be found.
  ASSERT_TRUE( e.emit( nom::UIEvent::WINDOW_FOCUS, ev ) == false );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
