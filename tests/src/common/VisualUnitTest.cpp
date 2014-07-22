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
#include "nomlib/tests/common/VisualUnitTest.hpp"

// Private headers
#include "nomlib/system/init.hpp"
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"
#include "nomlib/serializers/IValueSerializer.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/serializers/JsonCppSerializer.hpp"
#include "nomlib/serializers/JsonCppDeserializer.hpp"
#include "nomlib/tests/common/VisualUnitTestResultWriter.hpp"

namespace nom {

// Static initializations
std::string VisualUnitTest::timestamp_ = "\0";
bool VisualUnitTest::timestamp_initialized_ = false;
VisualTestSet VisualUnitTest::visual_test_;
ImageDiffResultBatch VisualUnitTest::results_;

void VisualUnitTest::initialize( const Size2i& res )
{
  Path p;
  File fp;

  // NOM_LOG_TRACE( NOM );

  // Use the same timestamp string across multiple object instances to prevent
  // multiple output directories for screen-shots intended for the same test
  // set.
  if( VisualUnitTest::timestamp_initialized_ == false )
  {
    // Use underscore char instead of a space char (filename friendly)
    VisualUnitTest::timestamp_ = nom::file_timestamp();

    VisualUnitTest::timestamp_initialized_ = true;
  }

  this->set_test_set_directory  (
                                  nom::test_output_directory + p.native() +
                                  this->test_set()
                                );

  this->set_test_reference_directory  (
                                        this->test_set_directory() + p.native() +
                                        "Reference"
                                      );

  // Note that this will ensure that the output directory tree is valid --
  // every time this constructor is called. In the context of unit tests,
  /// this means at the beginning of every test within a test case.
  this->initialize_directories();

  VisualUnitTestResultWriter::initialize  (
                                            this->test_reference_directory(),
                                            this->output_directory()
                                          );

  this->set_resolution( res );

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "VisualUnitTest test_name: ", this->test_name() );
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "VisualUnitTest test_set: ", this->test_set() );
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "VisualUnitTest path: ", this->output_directory() );

  // Initialize the visual test set batch
  VisualUnitTest::visual_test_.set_test_set( this->test_set() );
  // // VisualUnitTest::visual_test_.set_test_name( this->test_name() );
  VisualUnitTest::visual_test_.set_timestamp( VisualUnitTest::timestamp() );
  VisualUnitTest::visual_test_.set_comment( NOM_TEST_FLAG(test_comment) );
  VisualUnitTest::visual_test_.set_resolution( res );
  VisualUnitTest::visual_test_.set_directory( this->output_directory() );

  if( NOM_TEST_FLAG( no_html_output ) == false )
  {
    // Register tests start & end event listener (used for HTML results output)
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Append( new VisualUnitTestResultWriter() );
  }
}

VisualUnitTest::VisualUnitTest( void )
{
  this->initialize( Size2i( 640, 480 ) );
  this->show_fps( true );
}

VisualUnitTest::VisualUnitTest( const Size2i& res )
{
  this->initialize( res );
}

void VisualUnitTest::SetUp( void )
{
  uint32 window_flags = 0;

  // Summon SDL2 & friends
  nom::init_third_party( InitHints::DefaultInit );

  if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
  {
    NOM_LOG_INFO( NOM, "Could not disable vertical refresh." );
  }

  // Initialize rendering window (and its GL context)
  EXPECT_TRUE( this->window_.create( this->test_set(), this->resolution(), window_flags ) );
  this->render_window().set_logical_size( this->resolution() );

  // this->input_mapper_.clear();

  // Register default input bindings
  InputActionMapper state;
  KeyboardAction action;

  EventCallback ev_quit( [&] () { this->quit(); } );

  EventCallback ev_screenshot( [&] () { this->render_window().save_screenshot( this->test_name() + ".png" ); } );
  EventCallback ev_fullscreen( [&] () { this->render_window().toggle_fullscreen(); } );

  action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE );
  state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

  action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_q );
  state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

  action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_F1 );
  state.insert( "ev_screenshot", nom::InputAction( action, ev_screenshot ) );

  action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_f );
  state.insert( "ev_fullscreen", nom::InputAction( action, ev_fullscreen ) );

  this->input_mapper_.insert( this->test_set(), state, true );

  // Register the callbacks to be used within the game loop
  this->append_event_callback( [&] ( Event ev ) { this->input_mapper_.on_event( ev ); } );
  this->append_update_callback( [&] ( float delta ) { this->render_window().update(); } );

  // Default background color to use for the render window
  this->append_render_callback( [&] ( const RenderWindow& win ) { this->render_window().fill( Color4i::SkyBlue ); } );

  // Initialize the visual test set batch
  // VisualUnitTest::visual_test_.set_test_set( this->test_set() );
  // // // VisualUnitTest::visual_test_.set_test_name( this->test_name() );
  // VisualUnitTest::visual_test_.set_comment( NOM_TEST_FLAG(test_comment) );
  // VisualUnitTest::visual_test_.set_resolution( this->resolution() );
  // VisualUnitTest::visual_test_.set_directory( this->output_directory() );
}

::testing::AssertionResult VisualUnitTest::compare( void )
{
  File fp;
  ImageDiffResult r;
  std::stringstream result_msg;

  if( NOM_TEST_FLAG( disable_comparison ) == true )
  {
    NOM_LOG_INFO( NOM, "Image set comparison has been explicitly disabled." );
    return ::testing::AssertionSuccess();
  }

  // TODO: Add support for specifying output directory via NOM_TEST_FLAG --
  // including command line.

  ImageDiff diff( this->test_reference_directory(), this->output_directory() );

  if( fp.is_dir( this->test_reference_directory() ) == false )
  {
    NOM_LOG_ERR( NOM, "Reference directory path does not exist; did you forget to generate a reference image set?" );
    return ::testing::AssertionFailure();
  }

  r = diff.compare( this->output_filename() );

  r.test_name = this->test_name();
  r.image_filename = this->output_filename();

  // FIXME:
  size_t end = r.image_filename.find_last_of( "_" );
  r.frame = std::atoi( r.image_filename.substr( end + 1 ).c_str() );

  VisualUnitTest::results_.push_back( r );

  result_msg
  << r.image_filename
  << " has "
  << r.incorrect_pixels << " differing pixels";

  if( r.passed == false )
  {
    // FIXME: See notes in ImageDiff
    // if( r.image.valid() )
    // {
      // Path p;
      // File fp;
      // std::string ext = fp.extension( this->output_filename() );
      // std::string file = fp.basename( this->output_filename() );
      // std::string output_path = this->output_directory() + p.native() + file + "_diff." + ext;

      // EXPECT_TRUE( r.image.save_png( output_path ) )
      // << "Could not generate image diff file: "
      // << output_path;
    // }

    return ::testing::AssertionFailure()
    << result_msg.str();
  }

  return ::testing::AssertionSuccess()
  << result_msg.str();
}

VisualUnitTest::~VisualUnitTest( void )
{
  // NOM_LOG_TRACE( NOM );

  IValueSerializer* fp = new JsonCppSerializer();

  if( fp != nullptr && VisualUnitTest::visual_test_.images().size() > 0 )
  {
    VisualUnitTest::visual_test_.save_file( fp );
  }
}

int VisualUnitTest::on_run( void )
{
  uint elapsed_frames = 0;
  std::string title = this->test_set() + "::" + this->test_name();

  this->render_window().set_window_title( title );

  this->fps_counter_.start();
  this->fps_counter_update_.start();

  // Screen-shots are garbage if the SDL_Window is not shown
  NOM_ASSERT( this->window_.window_flags() & SDL_WINDOW_SHOWN );

  if( this->screenshot_frames_.size() < 1 && NOM_TEST_FLAG(disable_comparison) == false )
  {
    NOM_LOG_ERR( NOM, "Did you forget to append screen-shot frames to the unit test before calling ::on_run?" );
  }

  Event ev;
  while( this->app_state() == true )
  {
    while( this->evt_.poll_event( ev ) )
    {
      for( auto itr = this->event_callbacks_.begin(); itr != this->event_callbacks_.end(); ++itr )
      {
        itr->operator()( ev );
      }
    }

    this->fps_counter_.update();

    // Refresh the FPS display at one (1) second intervals
    if( this->fps_counter_update_.ticks() > 1000 )
    {
      if( this->fps() == true )
      {
        // Show test title plus FPS counter
        this->render_window().set_window_title( title + " - " + this->fps_counter_.asString() + ' ' + "fps" );
      }
      else
      {
        // Restore window title to defaults
        this->render_window().set_window_title( title );
      }

      this->fps_counter_update_.restart();
    } // end FPS refresh cycle

    for( auto itr = this->update_callbacks_.begin(); itr != this->update_callbacks_.end(); ++itr )
    {
      itr->operator()( 0 );
    }

    for( auto itr = this->render_callbacks_.begin(); itr != this->render_callbacks_.end(); ++itr )
    {
      itr->operator()( this->render_window() );
    }

    if( this->on_frame_end( elapsed_frames ) == true )
    {
      // Terminate the loop (end of test) once the conditional requirements
      // within the method are met
      this->quit();
    }

    ++elapsed_frames;
  } // end while loop

  return NOM_EXIT_SUCCESS;
}

bool VisualUnitTest::on_frame_end( uint elapsed_frames )
{
  bool ret = false;
  Path p;
  File fp;

  // if( NOM_TEST_FLAG(interactive) == true )
  // {
  //   // Do not terminate loop
  //   return false;
  // }

  if( this->screenshot_frames_.empty() )
  {
    if( NOM_TEST_FLAG(interactive) == true )
    {
      // Continue looping
      return false;
    }
    else
    {
      // Terminate loop
      return true;
    }
  }

  // Do not terminate the loop until we have captured all of the requested
  // frames
  else if( elapsed_frames == *( this->screenshot_frames_.begin() ) && NOM_TEST_FLAG( disable_comparison) == false )
  {
    // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "elapsed_frames: ", elapsed_frames );

    this->set_output_filename (
                                this->test_name() +
                                "_" +
                                std::to_string( elapsed_frames ) +
                                ".png"
                              );

    std::string absolute_file_path = this->output_directory() +
      p.native() + this->output_filename();

    // Ensure that we do not overwrite image sets (in particular, reference
    // image sets!)
    if( fp.exists( absolute_file_path ) == true )
    {
      if( NOM_TEST_FLAG( force_overwrite ) == true )  // --force command option
      {
        ret = this->render_window().save_png_file( absolute_file_path );
      }
      else
      {
        NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "File path for output image exists (not overwriting): ", absolute_file_path );
        ret = false;  // Assignment for peace of mind
      }
    }
    else  // All is well -- file output path does not exist
    {
      ret = this->render_window().save_png_file( absolute_file_path );
    }

    if( ret )
    {
      VisualUnitTest::visual_test_.append_image( this->output_filename() );
    }

    this->screenshot_frames_.erase( this->screenshot_frames_.begin() );

    if( this->screenshot_frames_.empty() == true )
    {
      if( NOM_TEST_FLAG(interactive) == true )
      {
        // Continue looping
        return false;
      }
      else
      {
        // Terminate loop
        return true;
      }
    }
  }

  // Do not terminate loop
  return false;
}

bool VisualUnitTest::fps( void ) const
{
  return this->show_fps_;
}

const Size2i& VisualUnitTest::resolution( void ) const
{
  return this->resolution_;
}

const std::string& VisualUnitTest::output_directory( void ) const
{
  return this->output_directory_;
}

const std::string& VisualUnitTest::test_set_directory( void ) const
{
  return this->test_set_directory_;
}

const std::string& VisualUnitTest::test_reference_directory( void ) const
{
  return this->test_reference_directory_;
}

const std::string& VisualUnitTest::output_filename( void ) const
{
  return this->output_filename_;
}

RenderWindow& VisualUnitTest::render_window( void )
{
  NOM_ASSERT( this->window_.valid() );

  return this->window_;
}

void VisualUnitTest::show_fps( bool state )
{
  this->show_fps_ = state;
}

void VisualUnitTest::set_resolution( const Size2i& res )
{
  this->resolution_ = res;
}

void VisualUnitTest::set_output_directory( const std::string& dir_path )
{
  this->output_directory_ = dir_path;
}

void VisualUnitTest::append_screenshot_frame( uint frame )
{
  this->screenshot_frames_.push_back( frame );
}

int VisualUnitTest::append_event_callback( const std::function<void( Event )>& func )
{
  this->event_callbacks_.push_back( func );

  return this->event_callbacks_.size();
}

int VisualUnitTest::append_update_callback( const std::function<void( float )>& func )
{
  this->update_callbacks_.push_back( func );

  return this->update_callbacks_.size();
}

int VisualUnitTest::append_render_callback( const std::function<void( const RenderWindow& )>& func )
{
  this->render_callbacks_.push_back( func );

  return this->render_callbacks_.size();
}

// Private scope

const std::string& VisualUnitTest::timestamp( void )
{
  return VisualUnitTest::timestamp_;
}

void VisualUnitTest::initialize_directories( void )
{
  Path p;
  File fp;

  if( NOM_TEST_FLAG( disable_comparison ) == true )
  {
    // NOM_LOG_INFO( NOM, "Image set comparison has been explicitly disabled." );
    return;
  }

  if( NOM_TEST_FLAG(reference_screenshot) == true )
  {
    std::string test_dir = nom::test_output_directory + p.native() + this->test_set();
    std::string ref_dir = test_dir + p.native() + "Reference";

    if( fp.recursive_mkdir( ref_dir ) == false )
    {
      // NOM_LOG_ERR( NOM, "Could not create directory for reference ", this->test_set(), " at: ", ref_dir );
      // exit( NOM_EXIT_FAILURE );
    }

    this->set_output_directory( ref_dir );
  }
  else
  {
    std::string test_name_dir = nom::test_output_directory + p.native() + this->test_set() + p.native() + this->test_set() + "_" + VisualUnitTest::timestamp();

    if( fp.recursive_mkdir( test_name_dir ) == false )
    {
      // NOM_LOG_ERR( NOM, "Could not create directory for test name ", this->test_name(), " at: ", test_name_dir );
      // exit( NOM_EXIT_FAILURE );
    }

    this->set_output_directory( test_name_dir );
  }
}

void VisualUnitTest::set_test_set_directory( const std::string& dir_path )
{
  this->test_set_directory_ = dir_path;
}

void VisualUnitTest::set_test_reference_directory( const std::string& dir_path )
{
  this->test_reference_directory_ = dir_path;
}

void VisualUnitTest::set_output_filename( const std::string& filename )
{
  this->output_filename_ = filename;
}

} // namespace nom
