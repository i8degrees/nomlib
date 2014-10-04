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

#include <iostream>
#include <string>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";
// const std::string RESOURCE_BITMAP_FONT = APP_RESOURCES_DIR + p.native() + "VIII.png";
// const std::string RESOURCE_BITMAP_SMALL_FONT = APP_RESOURCES_DIR + p.native() + "VIII_small.png";
// const std::string RESOURCE_TRUETYPE_FONT = APP_RESOURCES_DIR + p.native() + "arial.ttf";

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
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

const std::string RESOURCE_FONT_TEXT_STRING = "!\"#$%&'()*+,-.\n//0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~";
//const std::string RESOURCE_FONT_TEXT_STRING = "I am a Bitmap Font";

const int MAX_FONT_POINT_SIZE = 41;
const int MIN_FONT_POINT_SIZE = 9;

/// \brief nom::BitmapFont usage example
class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] )
    {
      NOM_LOG_TRACE ( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if( nom::init( argc, argv ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
        exit ( NOM_EXIT_FAILURE );
      }

      atexit( nom::quit );
    }

    ~App( void )
    {
      NOM_LOG_TRACE ( NOM );
    } // ~App

    bool on_init ( void )
    {
      nom::uint32 window_flags = 0; //SDL_WINDOW_RESIZABLE
      if ( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }
/*
      if ( nom::set_hint ( SDL_HINT_RENDER_SCALE_QUALITY, "Nearest" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not set scale quality." );
      }
*/
      if ( this->window.create ( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        return false;
      }

      // Scale window contents up by the new width & height
      this->window.set_logical_size ( this->window.size() );

      if ( this->window.set_window_icon ( RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
        return false;
      }

      this->load_bitmap_font();
      this->load_truetype_font();
      this->load_truetype_font2();

      nom::Font* font = nom::SystemFonts::cache().load_resource("VIII");

      NOM_ASSERT( font != nullptr );
      NOM_ASSERT( font->valid() == true );

      nom::Font label_font0 = this->label_tfont.font();
      nom::Font label_font1 = this->label_tfont2.font();

      // Ensure that nom::Font's copy-on-write functionality works; the
      // resource "VIII" should be unique (cloned).
      NOM_ASSERT( label_font0 != label_font1 );

      return true;
    } // onInit

    nom::int32 Run ( void )
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

        this->window.fill ( nom::Color4i::SkyBlue );

        this->label_bfont.draw( this->window );
        this->label_tfont.draw( this->window );
        this->label_tfont2.draw( this->window );

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

        // Use inherited SDLApp::on_app_quit method -- you may also provide your
        // own event handler for this.
        case SDLK_ESCAPE:
        case SDLK_q: this->on_app_quit( ev ); break;

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
          this->window.toggle_fullscreen();
        }
        break;
      } // end switch key
    } // onKeyDown

    void increase_font_size( nom::uint point_size )
    {
      nom::uint current_size = this->label_tfont.text_size();

      if( current_size < MAX_FONT_POINT_SIZE )
      {
        this->label_tfont.set_text_size( current_size += point_size );
      }
    }

    void decrease_font_size( nom::uint point_size )
    {
      nom::uint current_size = this->label_tfont.text_size();

      if( current_size >= MIN_FONT_POINT_SIZE )
      {
        this->label_tfont.set_text_size( current_size -= point_size );
      }
    }

    void on_mouse_wheel( const nom::Event& ev )
    {
      // Filter out non-wheel events (otherwise we can receive false positives)
      if( ev.type != SDL_MOUSEWHEEL ) return;

      if( ev.wheel.y > 0 )  // Up
      {
        this->increase_font_size( 1 );
      }
      else if( ev.wheel.y < 0 ) // Down
      {
        this->decrease_font_size( 1 );
      }
    }

  private:
    /// Window handle
    nom::RenderWindow window;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    // nom::BitmapFont bitmap_font;
    // nom::TrueTypeFont truetype_font;

    nom::Text label_bfont;
    nom::Text label_tfont;
    nom::Text label_tfont2;

    bool load_bitmap_font( void )
    {
      // if ( this->bitmap_font.load( RESOURCE_BITMAP_FONT ) == false )
      // {
      //   nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT );
      //   return false;
      // }

      // this->label_bfont.set_font( this->bitmap_font );

      this->label_bfont.set_font( nom::SystemFonts::cache().load_resource( "VIII" ) );

      this->label_bfont.set_position  ( nom::Point2i(
                                        ( this->window.size().w
                                          -
                                          this->label_bfont.width()
                                        ) / 2,
                                        ( this->window.size().h
                                          -
                                          this->label_bfont.height()
                                        ) / 2
                                                    )
                                      );

      this->label_bfont.set_text ( RESOURCE_FONT_TEXT_STRING );
      this->label_bfont.set_color ( nom::Color4i::White );
      //this->label_bfont.set_color ( nom::Color4i(195,209,228) );
      this->label_bfont.set_text_size ( 24 ); // NO-OP
      this->label_bfont.set_alignment ( nom::Anchor::TopLeft );

      return true;
    }

    bool load_truetype_font( void )
    {
      // if ( this->truetype_font.load ( RESOURCE_TRUETYPE_FONT ) == false )
      // {
      //   nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT );
      //   return false;
      // }

      // this->label_tfont.set_font( this->truetype_font );

      nom::Font font = *nom::SystemFonts::cache().load_resource( "Arial" );

      // Font kerning defaults to true.
      //
      // Note that if you disable kerning, be sure to update the text with
      // a text string that is known to have kerning applied, such as "WAV".
      //
      // font->set_font_kerning( false );

      this->label_tfont.set_font( font );

      this->label_tfont.set_position(nom::Point2i(24,24));
      //this->label_tfont.set_color( nom::Color4i(195,209,228) );

      this->label_tfont.set_text( RESOURCE_FONT_TEXT_STRING );
      // this->label_tfont.set_text( "WAV" );

      // this->label_tfont.set_alignment( nom::Anchor::TopLeft );

      // Cache the glyphs of the font's point size range that we plan on using;
      // offloads the cost of re-generating the glyph cache when the end-user
      // requests an increase or decrease. (An increase in load-time for a
      // decrease in latency upon rescaling, which is especially noticeable on
      // older platforms and *presumably* mobile devices).
      for( auto idx = MIN_FONT_POINT_SIZE - 1; idx != MAX_FONT_POINT_SIZE; ++idx )
      {
        this->label_tfont.set_text_size( idx + 1 );
      }

      this->label_tfont.set_text_size( nom::DEFAULT_FONT_SIZE );

      return true;
    }

    /// \remarks Test copy-on-write implementation in nom::Font.
    bool load_truetype_font2( void )
    {
      // this->label_tfont2.set_font( this->truetype_font );

      // A copy has to be made (handled internally by nom::Font), otherwise the
      // font's point size for label_tfont would be modified when we make the
      // call to set the text size on label_tfont2.
      this->label_tfont2.set_text( RESOURCE_FONT_TEXT_STRING );
      this->label_tfont2.set_style( nom::Text::Style::Bold );
      this->label_tfont2.set_font( nom::SystemFonts::cache().load_resource("Arial") );
      this->label_tfont2.set_text_size( 24 );
      this->label_tfont2.set_position( nom::Point2i( WINDOW_WIDTH / 2, 24 ) );
      this->label_tfont2.set_color( nom::Color4i( 195,209,228 ) );
      // this->label_tfont2.set_alignment( nom::Anchor::TopLeft );

      return true;
    }
}; // class App

nom::int32 main ( nom::int32 argc, char* argv[] )
{
  App game ( argc, argv );

  if ( game.on_init() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return game.Run();

  // ...Goodbye cruel world..!
}
