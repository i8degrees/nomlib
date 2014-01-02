/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include <cstdlib>
#include <cassert>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

const nom::Path p;
const std::string RESOURCE_BITMAP_FONT = APP_RESOURCES_DIR + p.native() + "VIII.png";

/// Name of our application.
const std::string APP_NAME = "nom::BitmapFont";

/// Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 768;

/// Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 448;

/// Relative filename path to saved screenshot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot_fonts.png";

/// \brief nom::BitmapFont usage example
class App: public nom::SDL_App
{
  public:
    App ( nom::int32 argc, char* argv[] )
    {
      NOM_LOG_TRACE ( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if ( nom::nomlib_init ( argc, argv ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
        exit ( NOM_EXIT_FAILURE );
      }
    } // App

    ~App ( void )
    {
      NOM_LOG_TRACE ( NOM );
    } // ~App

    bool onInit ( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      if ( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if ( this->window.create ( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        return false;
      }

      nom::Point2i window_size = this->window.size();
/*
      if ( this->window.set_window_icon ( RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
        return false;
      }
*/
      if ( this->bfont.load ( RESOURCE_BITMAP_FONT, nom::Color4u(255, 0, 255, 0) ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT );
        return false;
      }
      this->bfont.setPosition ( nom::Coords ( ( window_size.x - this->bfont.getFontWidth() ) / 2, ( window_size.y - this->bfont.getFontHeight() ) / 2 ) );
      this->bfont.setText ( "Bitmap Font" );
/*
      if ( this->font.load ( RESOURCE_TRUETYPE_FONT, NOM_COLOR4U_WHITE ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT );
        return false;
      }
*/
/*
      this->font.setFontSize ( 18 );
      this->font.setRenderingStyle ( nom::IFont::RenderStyle::Blended );
      this->font.setColor ( NOM_COLOR4U_WHITE );
      this->font.setText ( "Use arrow keys to change cursor!" );
      this->font.setPosition ( nom::Coords ( ( window_size.x - 200 ) / 2, window_size.y - 100 ) );
*/

      return true;
    } // onInit

    nom::int32 Run ( void )
    {
      this->update.start();
      this->fps.start();

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while ( this->PollEvents ( &this->event ) )
        {
          this->onEvent ( &this->event );
        }

        this->window.update();
        this->fps.update();

        // Refresh the frames per second at 1 second intervals
        if ( this->update.ticks() > 1000 )
        {
          if ( this->show_fps() == true )
          {
            this->window.set_window_title ( APP_NAME + " - " + this->fps.asString() + ' ' + "fps" );
          }
          else
          {
            this->window.set_window_title ( APP_NAME + " [" + std::to_string(this->window.window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window.window_display_id() ) );
          }

          this->update.restart();
        } // end refresh cycle

        this->bfont.update();
        //this->font.update();

        this->window.fill ( NOM_COLOR4U_PRIMARY_COLORKEY );

        this->bfont.draw ( this->window );
        //this->font.draw ( this->window );
      } // end while isRunning() is true
      } // end while SDL_App::running() is true

      return NOM_EXIT_SUCCESS;
    } // Run

  private:
    /// Event handler for key down actions
    ///
    /// Re-implements nom::Input::onKeyDown()
    void onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
    {
      switch ( key )
      {
        default: break;

        // Use inherited SDL_App::onQuit() method -- you may also provide your own
        // event handler for this.
        case SDLK_ESCAPE:
        case SDLK_q: this->onQuit(); break;

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
          if ( this->window.save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
          {
            nom::DialogMessageBox( APP_NAME,  "Could not save screenshot");
          }
          break;
        }

        case SDLK_f:
        {
          if ( this->window.fullscreen() == true )
          {
            this->window.toggle_fullscreen();
          }
          else if ( this->window.fullscreen() == false )
          {
            nom::Point2i window_size = this->window.size();
            this->window.toggle_fullscreen();

            // Scale window contents up by the new width & height
            this->window.set_logical_size ( window_size.x, window_size.y );
          }
        }
        break;
      } // end switch key
    } // onKeyDown

  private:
    /// Window handle
    nom::Window window;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    /// Input events
    Input::Event event;

    nom::BitmapFont bfont;
    //nom::TrueTypeFont font;
}; // class App

nom::int32 main ( nom::int32 argc, char* argv[] )
{
  App game ( argc, argv );

  if ( game.onInit() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return game.Run();

  // ...Goodbye cruel world..!
}

