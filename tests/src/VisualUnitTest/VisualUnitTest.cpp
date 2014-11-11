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
#include "nomlib/tests/VisualUnitTest/VisualUnitTest.hpp"

// Private headers
#include "nomlib/system/init.hpp"
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"
#include "nomlib/serializers/IValueSerializer.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/serializers/JsonCppSerializer.hpp"
#include "nomlib/serializers/JsonCppDeserializer.hpp"
#include "nomlib/tests/VisualUnitTest/VisualUnitTestResultWriter.hpp"

namespace nom {

// Static initializations
std::string VisualUnitTest::timestamp_ = "\0";
bool VisualUnitTest::timestamp_initialized_ = false;
ImageTestSet VisualUnitTest::visual_test_;
ImageDiffResultBatch VisualUnitTest::results_;

void VisualUnitTest::initialize( const Size2i& res )
{
  this->set_window_flags(0);
  this->show_fps(true);

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
}

VisualUnitTest::VisualUnitTest( const Size2i& res )
{
  this->initialize( res );
}

bool VisualUnitTest::init_rendering()
{
  // Summon SDL2 & friends.
  nom::init_third_party( InitHints::DefaultInit );

  if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
  {
    NOM_LOG_INFO( NOM, "Could not disable vertical refresh." );
  }

  // Initialize rendering window (and its GL context)
  if( this->window_.create( this->test_set(),
      this->resolution(), this->window_flags() ) == false )
  {
    return false;
  }

  // Allow for automatic rescaling of the output window based on aspect
  // ratio (i.e.: handle fullscreen resizing); this will use letterboxing
  // when the aspect ratio is greater
  // than what is available, or side-bars when the aspect ratio is less
  // than.
  this->render_window().set_logical_size( this->resolution() );

  // Use no pixel unit scaling; this gives us one to one pixel ratio
  this->render_window().set_scale( nom::Point2f(1,1) );

  return true;
}

void VisualUnitTest::SetUp( void )
{
  if( this->init_rendering() == false )
  {
    FAIL()
    << "Initialization of the rendering subsystem failed.";
  }

  // this->input_mapper_.clear();

  // Register default input bindings
  InputActionMapper state;

  EventCallback ev_quit( [&] ( const Event& evt ) { this->quit(); } );

  EventCallback ev_screenshot( [&] ( const Event& evt )
    {
      this->render_window().save_screenshot( this->test_name() + ".png" );
    }
  );

  EventCallback ev_fullscreen( [&] ( const Event& evt )
    {
      this->render_window().toggle_fullscreen();
    }
  );

  // A convenient key binding for *quickly* terminating all unit tests and test
  // cases within the executable immediately.
  //
  // NOTE: The returned exit code relies on the underlying testing framework's
  // failure totals. It is intended that this termination does not conflict
  // with said framework; try not to manipulate previously collected test
  // results or otherwise.
  //
  // NOTE: This key binding (along with any other registered binding) is not
  // available during non-interactive test executions.
  EventCallback term_tests( [&] ( const Event& evt )
    {
      std::exit( this->failed_test_count() );
    }
  );

  state.insert  (
                  "ev_quit",
                  nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE ),
                  ev_quit
                );

  state.insert  (
                  "ev_quit",
                  nom::KeyboardAction( SDL_KEYDOWN, SDLK_q ),
                  ev_quit
                );

  state.insert  (
                  "ev_screenshot",
                  nom::KeyboardAction( SDL_KEYDOWN, SDLK_F1 ),
                  ev_screenshot
                );

  state.insert  (
                  "ev_fullscreen",
                  nom::KeyboardAction( SDL_KEYDOWN, SDLK_f ),
                  ev_fullscreen
                );

  state.insert  (
                  "term_tests",

                  // Try to use native platform key modifiers
                  #if defined( NOM_PLATFORM_OSX ) // Use CMD key modifier
                    nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE, KMOD_LGUI ),
                  #else
                    // Use SHIFT key modifier; KMOD_GUI is probably going
                    // to be the Windows logo key...
                    //
                    // FIXME: Find a better key modifier to use here...?
                    // On Win7, ALT+ESCAPE switches windows and CTRL+ESCAPE
                    // brings up the Start menu.
                    nom::KeyboardAction( SDL_KEYDOWN, SDLK_BACKQUOTE, KMOD_LCTRL ),
                  #endif // defined NOM_PLATFORM_OSX

                  term_tests
                );

  this->input_mapper_.insert( this->test_set(), state, true );

  // Register the default callbacks to be used within the game loop
  this->append_event_callback( [&] ( Event ev ) { this->input_mapper_.on_event( ev ); } );
  this->append_update_callback( this->default_update_callback() );

  // Default background color to use for the render window
  this->append_render_callback( this->default_render_callback() );

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
  nom::size_type end = r.image_filename.find_last_of( "_" );
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

  // Save info file only if we have one or more images successfully captured
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
      // This plumbs in proper quit functionality for when the end-user requests
      // program termination through the window manager's close button; this
      // includes Windows ALT+F4 signal.
      //
      // Perhaps the event handling system needs a bit of rethinking...
      if( ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE )
      {
        this->set_app_state(false);
      }

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

    // Check to see if termination conditions for the main loop are met
    if( this->on_frame_end( elapsed_frames ) == true )
    {
      if( NOM_TEST_FLAG(interactive) == false )
      {
        this->quit();
      }
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

  if( this->screenshot_frames_.empty() )
  {
    return true;
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
      // Potentially terminates main loop
      return true;
    }
  }

  // Not ready to terminate loop yet
  return false;
}

bool VisualUnitTest::fps( void ) const
{
  return this->show_fps_;
}

uint32 VisualUnitTest::window_flags() const
{
  return this->window_flags_;
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

void VisualUnitTest::set_window_flags(uint32 flags)
{
  this->window_flags_ = flags;
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

VisualUnitTest::update_callback_type
VisualUnitTest::default_update_callback()
{
  return( [&] ( float delta ) {
    this->render_window().update();
   });
}

VisualUnitTest::render_callback_type
VisualUnitTest::default_render_callback()
{
  return( [&] ( const RenderWindow& win ) {
    this->render_window().fill( Color4i::SkyBlue );
   });
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

void VisualUnitTest::clear_event_callbacks()
{
  this->event_callbacks_.clear();
}

void VisualUnitTest::clear_update_callbacks()
{
  this->update_callbacks_.clear();
}

void VisualUnitTest::clear_render_callbacks()
{
  this->render_callbacks_.clear();
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
