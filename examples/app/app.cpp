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

/// Position & size declarations for our info_box object
const nom::sint INFO_BOX_WIDTH[2] = { 200, 300 };
const nom::sint INFO_BOX_HEIGHT[2] = { 48, 48 };

const nom::sint INFO_BOX_ORIGIN_X[2] =  {
                                          ( WINDOW_WIDTH / 2 - INFO_BOX_WIDTH[0] ) / 2,
                                          ( (WINDOW_WIDTH/2) - INFO_BOX_WIDTH[1] ) / 2
                                        };

const nom::sint INFO_BOX_ORIGIN_Y[2] =  {
                                          ( WINDOW_HEIGHT - INFO_BOX_HEIGHT[0] ) / 2,
                                          ( WINDOW_HEIGHT - 100 - INFO_BOX_HEIGHT[1] )
                                        };

/// Relative file path name of our resource example
const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";

const std::string RESOURCE_TRUETYPE_FONT[2] = {
                                                APP_RESOURCES_DIR + p.native() + "arial.ttf",
                                                APP_RESOURCES_DIR + p.native() + "TimesNewRoman.ttf"
                                              };

const std::string RESOURCE_BITMAP_FONT = APP_RESOURCES_DIR + p.native() + "VIII.png";
const std::string RESOURCE_BITMAP_SMALL_FONT = APP_RESOURCES_DIR + p.native() + "VIII_small.png";

const std::string RESOURCE_SPRITE = APP_RESOURCES_DIR + p.native() + "cursors.json";

/// Copyright (c) 2013 Fielding Johnston. All rights reserved.
const std::string RESOURCE_STATIC_IMAGE = APP_RESOURCES_DIR + p.native() + "boardoutline.png";

/// Relative filename path to saved screen shot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// Text string displayed on our message boxes
const std::string RESOURCE_INFO_BOX_TITLE_STRINGS[2] = { "INFO.", "INFO." };
const std::string RESOURCE_INFO_BOX_TEXT_STRINGS[4] = {
                                                        "I am a Bitmap Font!", // 0
                                                        "Use the arrow keys to update me...",
                                                        "Yeah Buddy!!!",
                                                        "...Light weight!" // 3
                                                      };

const enum nom::Label::Alignment RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[9] = {
                                                                              nom::Label::Alignment::TopLeft,       // 0
                                                                              nom::Label::Alignment::TopCenter,
                                                                              nom::Label::Alignment::TopRight,
                                                                              nom::Label::Alignment::MiddleLeft,
                                                                              nom::Label::Alignment::MiddleCenter,  // 4
                                                                              nom::Label::Alignment::MiddleRight,
                                                                              nom::Label::Alignment::BottomLeft,
                                                                              nom::Label::Alignment::BottomCenter,
                                                                              nom::Label::Alignment::BottomRight    // 8
                                                                            };

const int MIN_FONT_POINT_SIZE = 9;
const int MAX_FONT_POINT_SIZE = 41;

/// \brief Usage example
class App: public nom::SDL_App
{
  public:
    App ( nom::int32 argc, char* argv[] ) :
      sprite_angle ( -90.0f ),
      selected_font ( 0 ),        // nom::TrueType font
      selected_alignment ( 4 ),   // nom::Label::Alignment::TopLeft
      selected_font_size ( 14 ),  // Font's size (in pixels)
      selected_text_string ( 2 )  // "Yeah Buddy!!!"
    {
      NOM_LOG_TRACE ( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if ( nom::init ( argc, argv ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
        exit ( NOM_EXIT_FAILURE );
      }
      atexit(nom::quit);
    } // App

    ~App ( void )
    {
      NOM_LOG_TRACE ( NOM );
    } // ~App

    bool onInit ( void )
    {
      // nom::Gradient utilizes nom::Rectangle
      nom::Gradient gradient;

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
        this->window_size[idx] = this->window[idx].size();

        // Scale window contents up by the new width & height
        this->window[idx].set_logical_size ( this->window_size[idx].x, this->window_size[idx].y );
      }

      this->window[0].make_current();

      if ( this->bitmap_font.load ( RESOURCE_BITMAP_FONT ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT );
        return false;
      }
      // FIXME: this->bitmap_font.resize ( nom::ResizeAlgorithm::scale2x );

      if ( this->bitmap_small_font.load ( RESOURCE_BITMAP_SMALL_FONT ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_SMALL_FONT );
        return false;
      }

      if ( this->truetype_font.load ( RESOURCE_TRUETYPE_FONT[0] ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT[0] );
        return false;
      }

      // Load a sprite sheet, using the sheet_filename as the base path to load
      // the image file from disk
      this->sprite = nom::SpriteBatch ( RESOURCE_SPRITE );
      if ( this->sprite.load ( APP_RESOURCES_DIR + p.native() + this->sprite.sheet_filename() ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load sprite: " + this->sprite.sheet_filename() );
        return false;
      }
      // FIXME: this->sprite.resize ( nom::ResizeAlgorithm::scale2x );
      this->sprite.set_frame ( 1 ); // Left-pointing cursor hand

      // Load the same sprite sheet -- but this time -- used for animation
      // effects!
      this->ani_sprite = nom::AnimatedSprite ( RESOURCE_SPRITE );
      if ( this->ani_sprite.load ( APP_RESOURCES_DIR + p.native() + this->ani_sprite.sheet_filename() ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load sprite: " + this->ani_sprite.sheet_filename() );
        return false;
      }

      this->window[1].make_current();
      if ( this->background.load ( RESOURCE_STATIC_IMAGE, 0 ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load image file: " + RESOURCE_STATIC_IMAGE );
        return false;
      }

      this->window[0].make_current();

      // Initialize the background to use in our info_box object as a gradient
      // filled background
      gradient.set_start_color ( nom::Color4i::Gray );
      gradient.set_end_color ( nom::Color4i::LightGray );
      gradient.set_fill_direction ( nom::Gradient::FillDirection::Left );

      // Initialize our info_box[0] object
      this->info_box[0] = nom::MessageBox (
                                                INFO_BOX_ORIGIN_X[0],
                                                INFO_BOX_ORIGIN_Y[0],
                                                INFO_BOX_WIDTH[0],
                                                INFO_BOX_HEIGHT[0],
                                                // Use the built-in "gray" frame
                                                // style
                                                nom::MessageBox::Style::Gray,
                                                // Use a custom background style
                                                // object. A copy is made of
                                                // the object, so forgetting
                                                // about the object afterwards
                                                // is OK!
                                                gradient
                                              );

      this->info_box[0].set_title ( nom::Label ( RESOURCE_INFO_BOX_TITLE_STRINGS[0], this->bitmap_small_font, 8, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0] ) );
      this->info_box[0].set_text ( nom::Label ( RESOURCE_INFO_BOX_TEXT_STRINGS[0], this->bitmap_font, 12, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4] ) );

      // Initialize our info_box[1] object
      this->info_box[1] = nom::MessageBox (
                                                INFO_BOX_ORIGIN_X[1],
                                                INFO_BOX_ORIGIN_Y[1],
                                                INFO_BOX_WIDTH[1],
                                                INFO_BOX_HEIGHT[1],
                                                // Use the built-in "gray" frame
                                                // style
                                                nom::MessageBox::Style::Gray,
                                                // Use a custom background style
                                                // object
                                                gradient
                                              );

      this->info_box[1].set_title ( nom::Label ( RESOURCE_INFO_BOX_TITLE_STRINGS[1], this->bitmap_small_font, 8, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0] ) );
      this->info_box[1].set_text ( nom::Label ( RESOURCE_INFO_BOX_TEXT_STRINGS[1], this->select_font(), this->select_font_size(), this->select_alignment() ) );

// FIXME: should be 26 (sprite sheet width), but is 130 (total texture size)
NOM_DUMP_VAR(this->sprite.size().x);
      this->sprite.set_position ( this->info_box[0].position().x - 26, this->info_box[0].position().y );
      this->ani_sprite.set_position ( this->info_box[0].position().x + this->info_box[0].size().x + 26, this->info_box[0].position().y );
 // 16 is correct
NOM_DUMP_VAR(this->sprite.size().y);

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

        // Compute rotation angle to pass to renderer
        this->sprite_angle += 360.0f / this->fps[0].fps_float();
        if ( this->sprite_angle > 360.0f ) this->sprite_angle -= 360.0f;

        this->window[0].fill ( nom::Color4i::SkyBlue );
        this->info_box[0].draw ( this->window[0] );
        this->info_box[1].draw ( this->window[0] );
        this->sprite.draw ( this->window[0], this->sprite_angle );
        this->ani_sprite.draw ( this->window[0] );

        this->window[1].fill ( nom::Color4i::Black );
        this->background.draw ( this->window[1] );

        this->window[2].fill ( nom::Color4i::Magenta );

        // Choose a random color for filling the window with as a backdrop when
        // MAXIMUM_WINDOWS is greater than 3
        for ( auto idx = 3; idx < MAXIMUM_WINDOWS; idx++ )
        {
          nom::int32 random_color = nom::rand ( 1, 11 );

          switch ( random_color )
          {
            default: this->window[idx].fill ( nom::Color4i::Black ); break;
            case 1: this->window[idx].fill ( nom::Color4i::White ); break;
            case 2: this->window[idx].fill ( nom::Color4i::Red ); break;
            case 3: this->window[idx].fill ( nom::Color4i::Green ); break;
            case 4: this->window[idx].fill ( nom::Color4i::Blue ); break;
            case 5: this->window[idx].fill ( nom::Color4i::Yellow ); break;
            case 6: this->window[idx].fill ( nom::Color4i::Magenta ); break;
            case 7: this->window[idx].fill ( nom::Color4i::Cyan ); break;
            case 8: this->window[idx].fill ( nom::Color4i::Orange ); break;
            case 9: this->window[idx].fill ( nom::Color4i::LightGray ); break;
            case 10: this->window[idx].fill ( nom::Color4i::SkyBlue ); break;
            case 11: this->window[idx].fill ( nom::Color4i::Magenta ); break;
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

        case SDLK_EQUALS:
        {
          // Cap our maximal font point size (defaults is 41) -- this is done
          // because this is the maximum size that can fit inside our info box.
          if ( this->selected_font_size < MAX_FONT_POINT_SIZE ) this->selected_font_size += 1;

          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_MINUS:
        {
          // Cap our minimal font point size (defaults is 9)
          if ( this->selected_font_size > MIN_FONT_POINT_SIZE ) this->selected_font_size -= 1;

          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_0:
        {
          if ( mod == KMOD_LSHIFT )
          {
            this->selected_font_size = 14;
            this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
            break;
          }
          else if ( mod == KMOD_LGUI )
          {
            this->selected_text_string = 0;
            this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
            break;
          }

          this->selected_alignment = 0;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_1:
        {
          if ( mod == KMOD_LGUI )
          {
            this->selected_text_string = 1;
            this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
            break;
          }

          this->selected_alignment = 1;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_2:
        {
          if ( mod == KMOD_LGUI )
          {
            this->selected_text_string = 2;
            this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
            break;
          }

          this->selected_alignment = 2;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_3:
        {
          if ( mod == KMOD_LGUI )
          {
            this->selected_text_string = 3;
            this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
            break;
          }

          this->selected_alignment = 3;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_4:
        {
          this->selected_alignment = 4;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_5:
        {
          this->selected_alignment = 5;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_6:
        {
          this->selected_alignment = 6;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_7:
        {
          this->selected_alignment = 7;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_8:
        {
          this->selected_alignment = 8;
          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_UP:
        {
          // Do something cool
          break;
        }

        case SDLK_DOWN:
        {
          // Do something cool
          break;
        }

        case SDLK_LEFT:
        {
          if ( mod == KMOD_LSHIFT )
          {
            this->selected_font = 0; // nom::TrueTypeFont
            this->selected_font_size = 24;
          }

          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_RIGHT:
        {
          if ( mod == KMOD_LSHIFT )
          {
            this->selected_font = 1; // nom::BitmapFont
          }

          this->info_box[1].set_text ( nom::Label ( this->select_text_string(), this->select_font(), this->select_font_size(), this->select_alignment() ) );
          break;
        }

        case SDLK_F1:
        {
          if ( this->window[window_id - 1].window_id() == window_id )
          {
            if ( this->window[window_id - 1].save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
            {
              nom::DialogMessageBox( APP_NAME, "Could not save screenshot");
              break;
            } // end save_screenshot err check
          } // end window_id check
          break;
        }

        case SDLK_f:
        {
          if ( this->window[window_id - 1].window_id() == window_id )
          {
            this->window[window_id - 1].toggle_fullscreen();
          } // end window_id match
          break;
        } // end SDLK_f
      } // end switch key
    } // onKeyDown

  private:
    /// Window handles
    ///
    /// \todo Use std::vector
    nom::Window window[MAXIMUM_WINDOWS];

    nom::Point2i window_size[MAXIMUM_WINDOWS];

    /// Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    /// Input events
    Input::Event event;

    /// Utilize one of nomlib's advanced class object types -- the dialog
    /// message box; this is a part of an interface kit with game interfacing in
    /// mind.
    nom::MessageBox info_box[2];

    /// Texture used as a static background image
    nom::Texture background;

    /// Our spiffy sprites
    nom::SpriteBatch sprite;
    double sprite_angle;
    nom::AnimatedSprite ani_sprite;

    /// Our font resources for nom::Label, our text rendering API
    nom::BitmapFont bitmap_font;
    nom::BitmapFont bitmap_small_font;
    nom::TrueTypeFont truetype_font;

    int selected_font;
    int selected_alignment;
    int selected_font_size;
    nom::sint selected_text_string;

    nom::IFont::SharedPtr select_font ( void )
    {
      if ( this->selected_font == 0 && this->truetype_font.valid() == true )
      {
        return std::shared_ptr<nom::IFont> ( this->truetype_font.clone() );
      }
      else if ( this->selected_font == 1 && this->bitmap_font.valid() == true )
      {
        return std::shared_ptr<nom::IFont> ( this->bitmap_font.clone() );
      }
      else
      {
        return nullptr;
      }
    }

    enum nom::Label::Alignment select_alignment ( void )
    {
      switch ( this->selected_alignment )
      {
        default:
        case 0: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0]; break;
        case 1: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[1]; break;
        case 2: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[2]; break;
        case 3: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[3]; break;
        case 4: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4]; break;
        case 5: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[5]; break;
        case 6: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[6]; break;
        case 7: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[7]; break;
        case 8: return RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[8]; break;
      }
    }

    nom::sint select_font_size ( void )
    {
      return this->selected_font_size;
    }

    const std::string& select_text_string ( void )
    {
      switch ( this->selected_text_string )
      {
        default:
        case 0: return RESOURCE_INFO_BOX_TEXT_STRINGS[0]; break;
        case 1: return RESOURCE_INFO_BOX_TEXT_STRINGS[1]; break;
        case 2: return RESOURCE_INFO_BOX_TEXT_STRINGS[2]; break;
        case 3: return RESOURCE_INFO_BOX_TEXT_STRINGS[3]; break;
      }
    }
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
