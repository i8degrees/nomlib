#include <iostream>
#include <string>

#include "gtest/gtest.h"

// nom::UnitTest framework
#include "nomlib/tests/common.hpp"

#include <nomlib/config.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

namespace nom {

class GradientTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    GradientTest()
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~GradientTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE( NOM );

      // VisualUnitTest environment init...
      VisualUnitTest::SetUp();

      EventCallback debug_info( [&] ( const Event& evt )
        {
          NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "x:", evt.mouse.x, "y:", evt.mouse.y );
        }
      );

      InputActionMapper state;

      state.insert( "debug_info", nom::MouseButtonAction(
                    SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT ),
                    debug_info );

      this->input_mapper_.insert( "debug_info", state, true );

      this->colors[0] = {
                          Color4iColors{
                            Color4i::Gray,
                            Color4i::LightGray
                          }
                        };

      int w = this->resolution().w;
      int h = this->resolution().h;
      this->pos1 = Point2i( 0, 0 );
      this->dims1 = Size2i( w, h / 2 );

      this->pos2 = Point2i( 0, h / 2 );
      this->dims2 = Size2i( w, h / 2 );

      // Not used
      this->pos3 = Point2i( w/3, h/2 );
      this->dims3 = Size2i( w/3, h/3 );

      this->grad1.set_colors( this->colors[0] );
      this->grad1.set_position( this->pos1 );
      this->grad1.set_size( this->dims1 );

      this->grad2.set_colors( this->colors[0] );
      this->grad2.set_position( this->pos2 );
      this->grad2.set_size( this->dims2 );

      // Not used
      this->grad3.set_colors( this->colors[0] );
      this->grad3.set_position( this->pos3 );
      this->grad3.set_size( this->dims3 );

      // Register our main loop
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad1.draw( this->render_window() ); } );
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad2.draw( this->render_window() ); } );
      // this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad3.draw( this->render_window() ); } );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

  protected:
    Gradient grad1;
    Gradient grad2;
    // Not used
    Gradient grad3;

    Point2i pos1;
    Size2i dims1;

    Point2i pos2;
    Size2i dims2;

    // Not used
    Point2i pos3;
    Size2i dims3;

    /// \brief Colors used to create the gradient

    /// \remarks One of two used
    Color4iColors colors[2];
};

TEST_F( GradientTest, TopToBottomLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Top );
  this->grad2.set_fill_direction( Gradient::FillDirection::Top );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Top );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, BottomToTopLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Bottom );
  this->grad2.set_fill_direction( Gradient::FillDirection::Bottom );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Bottom );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, LeftToRightLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Left );
  this->grad2.set_fill_direction( Gradient::FillDirection::Left );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Left );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, RightToLeftLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Right );
  this->grad2.set_fill_direction( Gradient::FillDirection::Right );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Right );

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

  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_RENDER, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
