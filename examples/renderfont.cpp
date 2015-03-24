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
#include <cstdlib>
#include <string>

#include "tclap/CmdLine.h"

#include <nomlib/math.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

using namespace nom;

/// Name of our application.
const std::string APP_NAME = "renderfont";

/// Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 640;

/// Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 480;

const auto WINDOW_RESOLUTION = Size2i(WINDOW_WIDTH, WINDOW_HEIGHT);

/// Relative filename path to saved screenshot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \brief Defaults for text rendering.
///
/// \remarks See program usage by passing --help
struct AppFlags
{
  /// --text argument; optional
  std::string text = "!\"#$%&'()*+,-.\n//0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~";

  /// -b -i -s -u arguments; optional
  nom::uint32 style = nom::Text::Style::Normal;

  /// --type argument; optional
  // std::string type;
  // nom::IFont::FontType font_type = nom::IFont::FontType::NotDefined;

  /// --no-kerning
  bool use_kerning = true;

  /// --hinting
  bool use_hinting = false;

  /// last argument 1/2; required
  std::string filename;

  /// last argument 2/2; optional
  int pt_size = nom::DEFAULT_FONT_SIZE;
};

typedef std::vector<std::string> StringList;

/// \brief Text rendering with nom::Font
class FontRenderingApp: public nom::SDLApp
{
  public:
    FontRenderingApp(const AppFlags& opts)
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);

      nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, nom::NOM_LOG_PRIORITY_CRITICAL);

      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.style:", this->opts.style);
      // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.font_type:", this->opts.font_type);
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.text:", this->opts.text);
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.use_kerning:", this->opts.use_kerning);
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.use_hinting:", this->opts.use_hinting);
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.filename:", this->opts.filename);
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "opts.pt_size:", this->opts.pt_size);

      this->opts = opts;
    }

    virtual ~FontRenderingApp()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
    }

    bool on_init() override
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

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
      if ( this->window.create (APP_NAME, WINDOW_RESOLUTION, window_flags) == false )
      {
        return false;
      }

      // Scale window contents up by the new width & height
      this->window.set_logical_size ( this->window.size() );

      // FIXME:
      // if ( this->window.set_window_icon ( RESOURCE_ICON ) == false )
      // {
      //   nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
      //   return false;
      // }

      nom::File fp;
      nom::Path p;
      // Used by nom::BMFont interface; this sets the relative file root for
      // filenames given in .FNT files to be loaded from, using the pages tag's
      // 'file' field.
      nom::set_file_root( fp.path(this->opts.filename) + p.native() );

      this->load_font();

      return true;
    } // onInit

    nom::int32 Run ( void ) override
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

        // Use a color slightly darker than anything we might be using for
        // a color key, so that rendering problems will bleed through for us
        this->window.fill( nom::Color4i::SkyBlue - nom::Color4i(10,10,10,255) );

        if( this->rendered_text.valid() ) {
          this->rendered_text.draw(this->window);
        }

      } // end while SDLApp::running() is true
      return NOM_EXIT_SUCCESS;
    } // Run

  private:
    AppFlags opts;

    /// \brief Event handler for key down actions.
    ///
    /// Implements the nom::Input::on_key_down method.
    void on_key_down( const nom::Event& ev ) override
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

  private:
    /// Window handle
    nom::RenderWindow window;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    nom::Font font;
    nom::Text rendered_text;

    bool load_font()
    {
      if( this->font.load(this->opts.filename) == false ) {

        nom::DialogMessageBox(  APP_NAME,
                                "Could not load font file:" +
                                this->opts.filename );
        return false;
      }

      // TODO: support additional hinting types; see TrueTypeFont.hpp
      // FIXME: Not working; methinks the problem might be in TrueTypeFont
      // class, although it would not hurt to verify SDL2_ttf functionality
      // using the included 'showfont' binary.
      // if( this->opts.use_hinting == true ) {
      //   this->font->set_hinting(TTF_HINTING_LIGHT);
      // }

      this->rendered_text.set_font(this->font);
      this->rendered_text.set_position( nom::Point2i(0,0) );
      this->rendered_text.set_text(this->opts.text);
      this->rendered_text.set_text_size(this->opts.pt_size);
      this->rendered_text.set_style(this->opts.style);
      this->rendered_text.set_text_kerning(this->opts.use_kerning);

      // TODO:
      //this->rendered_text.set_color( nom::Color4i(195,209,228) );

      nom::set_alignment( &this->rendered_text, this->rendered_text.position(),
                          this->window.size(), nom::Anchor::MiddleCenter );

      return true;
    }
}; // end class FontRenderingApp

// ./renderfont --text 'Hello, World!' ~/Projects/nomlib.git/Resources/tests/graphics/BitmapFontTest/VIII.png 72
// ./renderfont --text 'Hello, World!' ~/Projects/nomlib.git/Resources/tests/graphics/TrueTypeFontTest/OpenSans-Regular.ttf 72
// ./renderfont --text 'Hello, World!' ~/Projects/nomlib.git/Resources/tests/graphics/BMFontTest/gameover.fnt 72
// ./renderfont --text 'WAV' --no-kerning ~/Library/Fonts/OpenSans-Regular.ttf 72
nom::int32 main ( nom::int32 argc, char* argv[] )
{
  using namespace TCLAP;

  AppFlags opts;

  if( argc < 0 ) {
    return NOM_EXIT_FAILURE;
  }

  try
  {
    CmdLine cmd( APP_NAME, ' ', nom::NOM_VERSION.version_string() );

    std::string kerning_desc = "Enable use of font kerning; defaults to TRUE";
    // std::string hinting_desc = "Enable use of font hinting; defaults to FALSE";
    std::string hinting_desc = "FIXME: Enable use of font hinting; defaults to FALSE";

    SwitchArg bold_arg( "b", "bold", "Render bold text", cmd, 0);
    SwitchArg italics_arg( "i", "italics", "Render italicized text", cmd, 0);
    SwitchArg underline_arg( "u", "underline", "Render underlined text", cmd, 0);
    SwitchArg strikethrough_arg(  "s", "strikethrough", "Render strikethrough text",
                                  cmd, 0);

    SwitchArg use_kerning("", "no-kerning", kerning_desc, cmd, true);
    SwitchArg use_hinting("", "hinting", hinting_desc, cmd, opts.use_hinting);

    // std::string type_arg_desc =
      // "Override the file extension with explicit font type";

    // ValueArg<std::string> type_arg( "", "type", type_arg_desc, false, opts.type,
                                    // "bitmap|truetype|bm", cmd );

    ValueArg<std::string> text_arg( "", "text", "Text string to render", false,
                                    opts.text, "Rendered string", cmd );

    // NOTE: These must always be added to the command parser last
    UnlabeledMultiArg<std::string> font_args( "font",
                                              "The font file path to use", true,
                                              "<font>.[png|fnt|ttf] [pt size]",
                                              cmd, false );

    cmd.parse(argc, argv);

    if( bold_arg.getValue() == true ) {
      opts.style |= nom::Text::Style::Bold; // 0x2
    }

    if( italics_arg.getValue() == true ) {
      opts.style |= nom::Text::Style::Italic; // 0x4
    }

    if( underline_arg.getValue() == true ) {
      opts.style |= nom::Text::Style::Underline;  // 0x8
    }

    if( strikethrough_arg.getValue() == true ) {
      opts.style |= nom::Text::Style::Strikethrough; // 0x16
    }

    opts.use_kerning = use_kerning.getValue();
    opts.use_hinting = use_hinting.getValue();

    // Use explicitly defined font type instead of file name extension
    // if( type_arg.getValue() != "" ) {
    //   std::string type_val = type_arg.getValue();

    //   if( type_val == "bitmap" || type_val == "BITMAP" ) {
    //     opts.font_type = nom::IFont::FontType::BitmapFont; // 1

    //   } else if( type_val == "truetype" || type_val == "TRUETYPE" ) {
    //     opts.font_type = nom::IFont::FontType::TrueTypeFont; // 2

    //   } else if( type_val == "bm" || type_val == "BM" ) {
    //     opts.font_type = nom::IFont::FontType::BMFont; // 3
    //   }

    //   // Leave font_type as-is (default value)
    // }

    if( text_arg.getValue() != "" ) {
      opts.text = text_arg.getValue();
    }

    StringList args = font_args.getValue();

    int pt_size_arg = 0;
    nom::size_type next_to_last = args.size() - 1;
    for( auto i = 0; i != args.size(); ++i ) {

      if( i == 0 ) {
        opts.filename = args[i];
      } else if( i == next_to_last ) {
        pt_size_arg = std::atoi( args[i].c_str() );

        if( pt_size_arg != 0 ) {
          opts.pt_size = pt_size_arg;
        }
      }
    }

  }
  catch( TCLAP::ArgException &e )
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  e.error(), "for arg", e.argId() );

    exit(NOM_EXIT_FAILURE);
  }

  // Engine init
  if( nom::init(argc, argv) == false ) {
    nom::DialogMessageBox(APP_NAME, "Could not initialize nomlib.");
    exit(NOM_EXIT_FAILURE);
  }

  atexit(nom::quit);

  FontRenderingApp app(opts);

  if( app.on_init() == false ) {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world..!
}
