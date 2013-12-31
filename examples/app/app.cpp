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

// Pubic nomlib interface headers
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

/// Installation prefix of our application.
///
/// Defaults to a null terminated string -- no prefix -- which leaves us in the
/// file path origin wherever we are executed from.
const std::string APP_INSTALL_PREFIX = "\0";

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// Name of our application.
const std::string APP_NAME = "nomlib Demo | Multiple Windows";

/// Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 768;

/// Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 448;

/// Maximum number of active windows we will attempt to spawn in this example
const nom::int32 MAXIMUM_WINDOWS = 3;

/// Relative file path name of our resource example
const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";

const std::string RESOURCE_TRUETYPE_FONT = APP_RESOURCES_DIR + p.native() + "arial.ttf";
const std::string RESOURCE_BITMAP_FONT = APP_RESOURCES_DIR + p.native() + "VIII.png";

const std::string RESOURCE_SPRITE = APP_RESOURCES_DIR + p.native() + "cursors.json";
//const std::string RESOURCE_SPRITE = APP_RESOURCES_DIR + p.native() + "dots.png";

/// Copyright (c) 2013 Fielding Johnston. All rights reserved.
const std::string RESOURCE_STATIC_IMAGE = APP_RESOURCES_DIR + p.native() + "boardoutline.png";

/// Relative filename path to saved screenshot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \brief Usage example
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

      this->deg = 0;
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

      // Best quality is Nearest rescaling for low resolution artwork, such as
      // our bitmap fonts!
      if ( nom::set_hint ( SDL_HINT_RENDER_SCALE_QUALITY, "Nearest" ) == false )
      {
NOM_LOG_INFO ( NOM, "Could not set scale quality to " + std::string ( "nearest" ) );
      }

      for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
      {
        if ( this->window[idx].create ( APP_NAME, WINDOW_WIDTH/2, WINDOW_HEIGHT, window_flags ) == false )
        {
          return false;
        }

        this->window[idx].set_position ( 0+(WINDOW_WIDTH/2) * idx, WINDOW_HEIGHT/2 );

        if ( this->window[idx].set_window_icon ( RESOURCE_ICON ) == false )
        {
          nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
          return false;
        }
      }

      this->window[0].set_active();

      // FIXME: if ( this->bfont.load ( RESOURCE_BITMAP_FONT, nom::Color4u(110, 144, 190, 255) ) == false )
      if ( this->bfont.load ( RESOURCE_BITMAP_FONT, nom::Color4u(255, 0, 255, 0) ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT );
        return false;
      }
      // FIXME: this->bfont.resize ( nom::ResizeAlgorithm::scale2x );

      if ( this->font.load ( RESOURCE_TRUETYPE_FONT, NOM_COLOR4U_WHITE ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT );
        return false;
      }

      // Load a sprite sheet, using the sheet_filename as the base path to load
      // the image file from disk
      this->sprite = nom::SpriteBatch ( RESOURCE_SPRITE );
      if ( this->sprite.load ( APP_RESOURCES_DIR + p.native() + this->sprite.sheet_filename(), nom::Color4u(255, 0, 255, 0) ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load sprite: " + this->sprite.sheet_filename() );
        return false;
      }
      // FIXME: this->sprite.resize ( nom::ResizeAlgorithm::scale2x );
      this->sprite.set_frame ( 1 ); // Left-pointing cursor hand

      // Load the same sprite sheet -- but this time -- used for animation
      // effects!
      this->ani_sprite = nom::AnimatedSprite ( RESOURCE_SPRITE );
      if ( this->ani_sprite.load ( APP_RESOURCES_DIR + p.native() + this->ani_sprite.sheet_filename(), nom::Color4u(255, 0, 255, 0) ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load sprite: " + this->ani_sprite.sheet_filename() );
        return false;
      }

      this->window[1].set_active();
      if ( this->background.load ( RESOURCE_STATIC_IMAGE, 0 ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load image file: " + RESOURCE_STATIC_IMAGE );
        return false;
      }

      this->window[0].set_active();
      nom::Point2i window_size = this->window[0].size();

      this->font.setFontSize ( 18 );
      this->font.setRenderingStyle ( nom::IFont::RenderStyle::Blended );
      this->font.setColor ( NOM_COLOR4U_WHITE );
      this->font.setText ( "Use arrow keys to change cursor!" );
      this->font.setPosition ( nom::Coords ( ( window_size.x - 200 ) / 2, window_size.y - 100 ) );

      // Initialize the background to use in our ui_frame object as a gradient
      // filled background
      this->gradient.set_start_color ( NOM_COLOR4U_GRAY );
      this->gradient.set_end_color ( NOM_COLOR4U_LIGHT_GRAY );
      this->gradient.set_fill_direction ( nom::Gradient::FillDirection::Left );

      // Setup our fancy dangled user interface frame
      this->ui_frame = nom::ui::MessageBox ( ( window_size.x - 200 ) / 2, ( window_size.y - 48 ) / 2, 200, 48, nom::ui::FrameStyle::Gray, this->gradient );

      this->ui_frame.setLabelFont ( &this->bfont );
      this->ui_frame.setLabel ( "I am a Bitmap Font!" );
      this->ui_frame.setLabelTextAlignment ( nom::IFont::TextAlignment::MiddleCenter );
NOM_DUMP_VAR(this->sprite.size().x); // FIXME: should be 26 (sprite sheet width), but is 130 (total texture size)
      this->sprite.set_position ( this->ui_frame.position().x - 26, this->ui_frame.position().y );
      this->ani_sprite.set_position ( this->ui_frame.position().x + this->ui_frame.size().x + 26, this->ui_frame.position().y );
NOM_DUMP_VAR(this->sprite.size().y); // 16 is correct

      return true;
    } // onInit

    nom::int32 Run ( void )
    {
      for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
      {
        this->update[idx].start();
        this->fps[idx].start();
      }

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while ( this->PollEvents ( &this->event ) )
        {
          this->onEvent ( &this->event );
        }

        this->ui_frame.update();
        this->bfont.update();
        this->font.update();
        this->sprite.update();
        this->ani_sprite.play();

        for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
        {
          this->window[idx].update();
          this->fps[idx].update();

          // Refresh the frames per second at 1 second intervals
          if ( this->update[idx].ticks() > 1000 )
          {
            if ( this->show_fps() == true )
            {
              this->window[idx].set_window_title ( APP_NAME + " - " + this->fps[idx].asString() + ' ' + "fps" );
            }
            else
            {
              this->window[idx].set_window_title ( APP_NAME + " [" + std::to_string(this->window[idx].window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window[idx].window_display_id() ) );
            }

            this->update[idx].restart();
          } // end refresh cycle
        } // end for MAXIMUM_WINDOWS update loop

        this->window[0].fill ( NOM_COLOR4U_PRIMARY_COLORKEY );
        this->ui_frame.draw ( this->window[0] );
        this->sprite.draw ( this->window[0], this->deg );
        this->ani_sprite.draw ( this->window[0] );

        this->bfont.draw ( this->window[0] );
        this->font.draw ( this->window[0] );

        this->window[1].fill ( NOM_COLOR4U_BLACK );
        this->background.draw ( this->window[1] );

        this->window[2].fill ( NOM_COLOR4U_SECONDARY_COLORKEY );

        // Choose a random color for filling the window with as a backdrop when
        // MAXIMUM_WINDOWS is greater than 3
        for ( auto idx = 3; idx < MAXIMUM_WINDOWS; idx++ )
        {
          nom::int32 random_color = nom::rand ( 1, 11 );

          switch ( random_color )
          {
            default: this->window[idx].fill ( NOM_COLOR4U_BLACK ); break;

            case 1: this->window[idx].fill ( NOM_COLOR4U_WHITE ); break;
            case 2: this->window[idx].fill ( NOM_COLOR4U_RED ); break;
            case 3: this->window[idx].fill ( NOM_COLOR4U_GREEN ); break;
            case 4: this->window[idx].fill ( NOM_COLOR4U_BLUE ); break;
            case 5: this->window[idx].fill ( NOM_COLOR4U_YELLOW ); break;
            case 6: this->window[idx].fill ( NOM_COLOR4U_MAGENTA ); break;
            case 7: this->window[idx].fill ( NOM_COLOR4U_CYAN ); break;
            case 8: this->window[idx].fill ( NOM_COLOR4U_ORANGE ); break;
            case 9: this->window[idx].fill ( NOM_COLOR4U_LIGHT_GRAY ); break;
            case 10: this->window[idx].fill ( NOM_COLOR4U_PRIMARY_COLORKEY ); break;
            case 11: this->window[idx].fill ( NOM_COLOR4U_SECONDARY_COLORKEY ); break;
          }
        }

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

        case SDLK_LEFT:
        {
          if ( mod == KMOD_LSHIFT )
          {
            this->deg -= 90;
            break;
          }

          nom::int32 id = this->sprite.frame();

          if ( id > 0 ) id--;

          this->sprite.set_frame ( id );
          this->font.setText( "Light weight!" );
          break;
        }
        case SDLK_RIGHT:
        {
          if ( mod == KMOD_LSHIFT )
          {
            this->deg += 90;
            break;
          }

          nom::int32 id = this->sprite.frame();

          if ( id < this->sprite.frames() - 1 ) id++;

          this->sprite.set_frame ( id );
          this->font.setText( "Yeah buddy!" );
          break;
        }

        case SDLK_F1:
        {
          for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
          {
            if ( this->window[idx].window_id() == window_id )
            {
              if ( this->window[idx].save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
              {
                nom::DialogMessageBox( APP_NAME,  "Could not save screenshot: " +
                                                  OUTPUT_SCREENSHOT_FILENAME );
                break;
              }

              // Success!
              NOM_LOG_INFO( NOM,  "The screenshot has been saved at: " +
                                  OUTPUT_SCREENSHOT_FILENAME );
              break;
            }
          }
          break;
        }

        case SDLK_f:
        {
          for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
          {
            if ( this->window[idx].window_id() == window_id )
            {
              if ( this->window[idx].fullscreen() == true )
              {
                this->window[idx].toggle_fullscreen();
                break;
              }

              if ( this->window[idx].fullscreen() == false )
              {
                nom::Point2i window_size = this->window[idx].size();
                this->window[idx].toggle_fullscreen();

                // Scale window contents up by the new width & height
                this->window[idx].set_logical_size ( window_size.x, window_size.y );
                break;
              }
            } // end window_id match
          } // end for window loop
        }
        break;
      } // end switch key
    } // onKeyDown

  private:
    /// Window handles
    ///
    /// \todo Use std::vector
    nom::Window window[MAXIMUM_WINDOWS];

    /// Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    /// Input events
    Input::Event event;

    /// nom::ui::MessageBox utilizes nom::GrayFrame (which uses nom::Line),
    /// nom::Gradient and nom::IFont
    nom::ui::MessageBox ui_frame;

    /// nom::Gradient utilizes nom::Rectangle
    nom::Gradient gradient;

    /// Texture used as a static background image
    nom::Texture background;

    nom::BitmapFont bfont;
    nom::TrueTypeFont font;

    nom::SpriteBatch sprite;
    nom::AnimatedSprite ani_sprite;

    double deg;
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
