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
#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

// using namespace nom;

const nom::Path p;

/// \brief File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources" + p.native();

/// \brief Name of our application.
const std::string APP_NAME = "nomlib's System Mouse Cursors";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 640;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 480;

const std::string RESOURCE_ICON = APP_RESOURCES_DIR + "icon.png";

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] ) :
      SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES ),
      system_cursor_( -1 ),
      mouse_tracking_( false )
    {
      NOM_LOG_TRACE ( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if( nom::init( argc, argv ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not initialize nomlib." );
        exit( NOM_EXIT_FAILURE );
      }

      atexit(nom::quit);

      // Enable logging of variable states app execution
      nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_VIDEO, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
    }

    ~App( void )
    {
      NOM_LOG_TRACE ( NOM );
    }

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_SHOWN;

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if( this->window.create( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        return false;
      }

      if( this->window.set_window_icon( RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
        return false;
      }

      // Scale window contents up by the new width & height
      this->window.set_logical_size( this->window.size() );

      return true;
    }

    nom::int32 Run( void )
    {
      nom::MouseState mouse;

      this->update.start();
      this->fps.start();

      // this->cursor.set_system_cursor( 0 );
      // this->cursor.show_cursor( true );

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while( this->poll_event( this->event ) )
        {
          this->on_event( this->event );
        }

        this->window.update();
        this->fps.update();

        // Refresh the frames per second at 1 second intervals
        if( this->update.ticks() > 1000 )
        {
          if( this->show_fps() == true )
          {
            this->window.set_window_title( APP_NAME + " - " + this->fps.asString() + ' ' + "fps" );
          }
          else
          {
            this->window.set_window_title( APP_NAME + " [" + std::to_string(this->window.window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window.window_display_id() ) );
          }

          this->update.restart();
        } // end refresh cycle

        this->window.fill( nom::Color4i::Black );

        if( this->update_system_cursor_ == true )
        {
          this->cursor.set_system_cursor( NOM_SCAST( nom::Cursor::Type, this->system_cursor_ ) );

          NOM_DUMP_VAR( NOM_LOG_CATEGORY_VIDEO, "system_cursor_id: ", this->cursor.system_cursor() );

          this->update_system_cursor_ = false;
        }

        if( this->mouse_tracking_ == true )
        {
          mouse = this->cursor.mouse_state();

          NOM_DUMP_VAR( NOM_LOG_CATEGORY_VIDEO, "pos.x: ", mouse.pos.x );
          NOM_DUMP_VAR( NOM_LOG_CATEGORY_VIDEO, "pos.y: ", mouse.pos.y );
        }

      } // while app_state is true (running)

      return NOM_EXIT_SUCCESS;
    }

  private:
    /// \brief Event handler for key down actions.
    ///
    /// Implements the nom::Input::on_key_down method.
    void on_key_down( const nom::Event& ev )
    {
      switch ( ev.key.sym )
      {
        default: break;

        // Use inherited SDLApp::on_app_quit() method -- you may also provide
        // your own event handler for this.
        case SDLK_ESCAPE:
        case SDLK_q: this->on_app_quit( ev ); break;

        // Toggle mouse state tracking
        case SDLK_s:
        case SDLK_m:
        {
          if( mouse_tracking_ == true )
          {
            this->mouse_tracking_ = false;
          }
          else
          {
            this->mouse_tracking_ = true;
          }
          break;
        }

        // Previous system cursor
        case SDLK_LEFT:
        {
          --this->system_cursor_;
          if( this->system_cursor_ < 0 )
          {
            this->system_cursor_ = nom::Cursor::Type::NumSystemCursors - 1;
          }

          this->update_system_cursor_ = true;
          break;
        }

        // Next system cursor
        case SDLK_RIGHT:
        {
          ++this->system_cursor_;
          if( this->system_cursor_ >= nom::Cursor::Type::NumSystemCursors )
          {
            this->system_cursor_ = 0;
          }

          this->update_system_cursor_ = true;
          break;
        }
      } // end switch key
    } // on_key_down

  private:
    nom::Event event;

    /// Window handles
    nom::RenderWindow window;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    /// \brief The system cursor.
    nom::Cursor cursor;

    /// \brief The selected cursor.
    int system_cursor_;

    /// \brief Update the system cursor only once, so that we do not flood the
    /// debug logging.
    bool update_system_cursor_;

    /// \brief Toggle mouse tracking (constant polling of mouse state); this
    /// will flood the debug log with the updates.
    bool mouse_tracking_;
};

nom::int32 main( nom::int32 argc, char* argv[] )
{
  App app( argc, argv );

  if( app.on_init() == false )
  {
    nom::DialogMessageBox( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world..!
}
