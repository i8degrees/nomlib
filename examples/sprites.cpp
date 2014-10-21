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
#include <string>

// Pubic nomlib interface headers
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

const std::string APP_NAME = "Sprites";
const nom::int32 WINDOW_WIDTH = 640;
const nom::int32 WINDOW_HEIGHT = 480;

const std::string RESOURCE_ICON = "icon.png";

const std::string RESOURCE_SPRITE_SHEET = "cursors.json";

/// \brief Relative filename path to saved screen shot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] ) :
      SDLApp(OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES),
      sprite_angle ( -90.0f )
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO);
    }

    ~App()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO);
    }

    bool on_init()
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;
      nom::uint32 render_flags = SDL_RENDERER_ACCELERATED;
      int render_driver = -1;

      nom::SearchPath res;
      std::string res_file = "sprites.json";

      // Determine our resources path based on several possible locations;
      // this is dependent upon the build environment
      if( res.load_file(res_file,"resources") == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                     "Could not determine the resource path for", res_file );
        return false;
      }

      if( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not set scale quality to " + std::string ( "nearest" ) );
      }

      if( this->window.create( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags, render_driver, render_flags ) == false )
      {
        return false;
      }

      if( this->window.set_window_icon( res.path() + RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox(  APP_NAME,
                                "Could not load window icon: " +
                                res.path() + RESOURCE_ICON );
        return false;
      }

      // Allow for automatic rescaling of the output window based on aspect
      // ratio (i.e.: handle fullscreen resizing); this will use letterboxing
      // when the aspect ratio is greater than what is available, or side-bars
      // when the aspect ratio is less than.
      this->window.set_logical_size( this->window.size() );

      // Use no pixel unit scaling; this gives us one to one pixel ratio
      this->window.set_scale( nom::Point2f(1,1) );

      nom::SpriteSheet sprite_frames;

      // Load a sprite sheet, using the sheet_filename as the base path to load
      // the image file from disk
      if( sprite_frames.load_file(res.path() + RESOURCE_SPRITE_SHEET) == false ) {
        nom::DialogMessageBox(  APP_NAME,
                                "Could not load sprite sheet: " +
                                res.path() + RESOURCE_SPRITE_SHEET );
        return false;
      }

      if( this->sprite_tex.load( res.path() + sprite_frames.sheet_filename(), false, nom::Texture::Access::Streaming ) == false )
      {
        nom::DialogMessageBox(  APP_NAME,
                                "Could not load sprite texture: " +
                                res.path() + sprite_frames.sheet_filename() );
        return false;
      }

      this->sprite.set_texture(sprite_tex);
      this->sprite.set_sprite_sheet(sprite_frames);
      this->sprite_tex.resize(nom::Texture::ResizeAlgorithm::scale2x);
      this->sprite.set_frame(1); // Left-pointing cursor hand

      // Sharing the same texture for the animated sprite instead of loading
      // another texture source would be OK, too, if we didn't care about
      // preserving the original scale of the sprite here for testing purposes.
      // this->ani_sprite.set_texture( *this->sprite_tex.clone() );
      if( this->ani_sprite_tex.load( res.path() + sprite_frames.sheet_filename() ) == false )
      {
        nom::DialogMessageBox(  APP_NAME,
                                "Could not load sprite texture: " +
                                res.path() + sprite_frames.sheet_filename() );
        return false;
      }

      this->ani_sprite.set_texture(this->ani_sprite_tex);
      // Use the same sprite sheet source for the animated sprite
      this->ani_sprite.set_sprite_sheet(sprite_frames);

      nom::Point2i sprite_offset, ani_sprite_offset;
      sprite_offset.x = (WINDOW_WIDTH/2 - this->sprite.size().w)/2;
      sprite_offset.y = (WINDOW_HEIGHT - this->sprite.size().h)/2;
      NOM_DUMP(this->sprite.size().w); // 52
      NOM_DUMP(this->sprite.size().h); // 32
      ani_sprite_offset.x = WINDOW_WIDTH/2 + this->sprite.size().w;
      ani_sprite_offset.y = (WINDOW_HEIGHT - this->sprite.size().h)/2;

      this->sprite.set_position(sprite_offset);
      this->ani_sprite.set_position(ani_sprite_offset);

      return true;
    } // onInit

    nom::int32 on_run()
    {
      this->update.start();
      this->fps.start();

      // 1. Events
      // 2. Logic
      // 3. Render

      nom::Event ev;
      while ( this->running() == true )
      {
        while( this->poll_event( ev ) )
        {
          this->on_event( ev );
        }

        this->ani_sprite.play();

        this->window.update();
        this->fps.update();

        // Refresh the frames per second at 1 second intervals
        if( this->update.ticks() > 1000 ) {
          if( this->show_fps() == true ) {
            this->window.set_window_title( APP_NAME + " - " + this->fps.asString() + ' ' + "fps" );
          }
          else {
            this->window.set_window_title( APP_NAME + " [" + std::to_string(this->window.window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window.window_display_id() ) );
          }

          this->update.restart();
        } // end refresh cycle

        // Compute rotation angle to pass to renderer
        this->sprite_angle += 4.0f;
        if ( this->sprite_angle > 360.0f ) this->sprite_angle -= 360.0f;

        this->window.fill ( nom::Color4i::SkyBlue );
        this->sprite.draw ( this->window, this->sprite_angle );
        this->ani_sprite.draw ( this->window );

      } // end while SDLApp::running() is true

      return NOM_EXIT_SUCCESS;
    } // Run

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
        case SDLK_q: this->on_app_quit(ev); break;

        case SDLK_BACKSLASH:
        {
          if ( this->toggle_fps() )
          {
            // Stub for doing something cool here
          }
          else
          {
            // Stub for doing something cool here
          }
          break;
        }

        case SDLK_F1:
        {
          if( this->window.save_screenshot(OUTPUT_SCREENSHOT_FILENAME) == false )
          {
            nom::DialogMessageBox( APP_NAME, "Could not save screenshot");
            break;
          }
          break;
        }

        case SDLK_f:
        {
          this->window.toggle_fullscreen();
          break;
        } // end SDLK_f
      } // end switch key
    } // onKeyDown

  private:
    /// Window handles
    nom::RenderWindow window;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    /// Our spiffy sprites
    nom::Texture sprite_tex;
    nom::Texture ani_sprite_tex;
    nom::SpriteBatch sprite;
    double sprite_angle;
    nom::AnimatedSprite ani_sprite;
}; // class App

nom::int32 main( nom::int32 argc, char* argv[] )
{
  // FIXME: This must be done before calling nom::init because of a
  // dependency on SDL's video subsystem being initialized first.
  // nom::init_third_party should be made explicit (by the end-user).
  if( nom::set_hint( SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0" ) == false )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not disable Spaces support." );
  }

  // nom::init sets the working directory to this executable's directory
  // path
  if( nom::init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }

  atexit(nom::quit);

  App game( argc, argv );

  if ( game.on_init() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return game.on_run();

  // ...Goodbye cruel world..!
}
