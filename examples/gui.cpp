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

#define NOM_BUILD_BUTTON_EX0
#define NOM_BUILD_BUTTON_EX1
#define NOM_BUILD_BITMAP_BUTTON_EX0

#define NOM_BUILD_LISTBOX_EX0
#define NOM_BUILD_LISTBOX_EX1
#define NOM_BUILD_LISTBOX_EX2

#define NOM_BUILD_MESSAGEBOX_EX0
#define NOM_BUILD_MESSAGEBOX_EX1
#define NOM_BUILD_QUESTION_DIALOGBOX_EX0

#define NOM_BUILD_DATAVIEWLIST_EX0
#define NOM_BUILD_DATAVIEWLIST_EX1
#define NOM_BUILD_DATAVIEWLIST_EX2

const nom::Path p;

/// \brief File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources" + p.native();

/// \brief Name of our application.
const std::string APP_NAME = "nomlib Demo | GUI widgets";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 1024;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 768;

/// \brief Maximum number of active windows we will attempt to spawn in this example
const nom::int32 MAXIMUM_WINDOWS = 1;

const std::string RESOURCE_ICON = APP_RESOURCES_DIR + "icon.png";

const std::string RESOURCE_TRUETYPE_FONT[4] = {
                                                APP_RESOURCES_DIR + "arial.ttf",
                                                APP_RESOURCES_DIR + "TimesNewRoman.ttf",
                                                APP_RESOURCES_DIR + "gui" + p.native() + "LucidaGrande.ttf",
                                                APP_RESOURCES_DIR + "gui" + p.native() + "LucidaGrande-Bold.ttf"
                                              };

const std::string RESOURCE_BITMAP_FONT = APP_RESOURCES_DIR + "VIII.png";
const std::string RESOURCE_BITMAP_SMALL_FONT = APP_RESOURCES_DIR + "VIII_small.png";

const std::string RESOURCE_SPRITE = APP_RESOURCES_DIR + "gui" + p.native() + "menu_elements.json";

const std::string RESOURCE_BUTTON_IMAGE = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "button.png";

const std::string RESOURCE_AQUA_BUTTON_IMAGE_0 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_0.png";
const std::string RESOURCE_AQUA_BUTTON_IMAGE_1 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_1.png";

/// \brief Relative filename path to saved screen shot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \brief Text string displayed on our message boxes
const std::string RESOURCE_INFO_BOX_TITLE_STRINGS[3] = { "INFO.", "INFO.", "CHOICE" };
const std::string RESOURCE_INFO_BOX_TEXT_STRINGS[4] = {
                                                        "I am a Bitmap Font!", // 0
                                                        "Are you sure?",
                                                        "Yeah Buddy!!!",
                                                        "...Light weight!" // 3
                                                      };
NOM_IGNORED_VARS();
const enum nom::Text::Alignment RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[9] = {
                                                                              nom::Text::Alignment::TopLeft,       // 0
                                                                              nom::Text::Alignment::TopCenter,
                                                                              nom::Text::Alignment::TopRight,
                                                                              nom::Text::Alignment::MiddleLeft,
                                                                              nom::Text::Alignment::MiddleCenter,  // 4
                                                                              nom::Text::Alignment::MiddleRight,
                                                                              nom::Text::Alignment::BottomLeft,
                                                                              nom::Text::Alignment::BottomCenter,
                                                                              nom::Text::Alignment::BottomRight    // 8
                                                                        };
NOM_IGNORED_ENDL();

/// \brief Usage example
class App: public nom::SDLApp
{
  public:
    App ( nom::int32 argc, char* argv[] ) :
      button_ex0{ nullptr },
      button_ex1{ nullptr },
      button_ex2{ nullptr },
      listbox_ex0{ nullptr },
      listbox_ex1{ nullptr },
      listbox_ex2{ nullptr }
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

      delete this->gui_window[0];
      delete this->gui_window[1];
      delete this->gui_window[2];
      delete this->gui_window[3];
      delete this->gui_window[4];
      delete this->gui_window[5];

      this->gui_window[0] = nullptr;
      this->gui_window[1] = nullptr;
      this->gui_window[2] = nullptr;
      this->gui_window[3] = nullptr;
      this->gui_window[4] = nullptr;
      this->gui_window[5] = nullptr;
    }

    bool on_init ( void )
    {
      // Colors used to create gradient backgrounds for nom::UIWidget objects.
      this->g_colors[0] = {
                            nom::Color4iColors{
                              nom::Color4i::Gray,
                              nom::Color4i::LightGray
                            }
                          };

      this->g_colors[1] = {
                            nom::Color4iColors{
                              nom::Color4i::LightGray,
                              nom::Color4i::Gray
                            }
                          };

      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;
      if ( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
      {
        if ( this->window[idx].create( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
        {
          return false;
        }

        // this->window[idx].set_position ( 0+(WINDOW_WIDTH/2) * idx, WINDOW_HEIGHT/2 );

        if ( this->window[idx].set_window_icon ( RESOURCE_ICON ) == false )
        {
          nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
          return false;
        }
        this->window_size[idx] = this->window[idx].size();

        // Scale window contents up by the new width & height
        this->window[idx].set_logical_size ( this->window_size[idx].x, this->window_size[idx].y );
      }

      if ( this->bitmap_font.load ( RESOURCE_BITMAP_FONT ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_FONT );
        return false;
      }

      // FIXME: this->bitmap_font.resize ( nom::Texture::ResizeAlgorithm::scale2x );

      if( this->bitmap_small_font.load ( RESOURCE_BITMAP_SMALL_FONT ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load BitmapFont: " + RESOURCE_BITMAP_SMALL_FONT );
        return false;
      }

      if( this->truetype_font.load ( RESOURCE_TRUETYPE_FONT[0] ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT[0] );
        return false;
      }

      // this->truetype_font.set_sharable( true );

      // if( this->truetype_font2.load ( RESOURCE_TRUETYPE_FONT[3] ) == false )
      // {
      //   nom::DialogMessageBox ( APP_NAME, "Could not load TrueTypeFont: " + RESOURCE_TRUETYPE_FONT[3] );
      //   return false;
      // }

      // this->truetype_font2.set_sharable( true );

      if ( this->button_bg[0].load( RESOURCE_BUTTON_IMAGE, 0 ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not load image file: " + RESOURCE_BUTTON_IMAGE );
        return false;
      }

      if ( this->button_bg[1].load( RESOURCE_AQUA_BUTTON_IMAGE_0, 0 ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not load image file: " + RESOURCE_AQUA_BUTTON_IMAGE_0 );
        return false;
      }

      if ( this->button_bg[2].load( RESOURCE_AQUA_BUTTON_IMAGE_1, 0 ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not load image file: " + RESOURCE_AQUA_BUTTON_IMAGE_1 );
        return false;
      }

      // Load a sprite sheet, using the sheet_filename as the base path to load
      // the image file from disk:
      this->menu_elements = nom::SpriteBatch( RESOURCE_SPRITE );
      if ( this->menu_elements.load( APP_RESOURCES_DIR + p.native() + this->menu_elements.sheet_filename(), 0, nom::Texture::Access::Streaming ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "Could not load sprite: " + this->menu_elements.sheet_filename() );
        return false;
      }
      // this->menu_elemen.->resize ( nom::Texture::ResizeAlgorithm::scale2x );

      NOM_ASSERT( this->do_event_tests() == 0 );

      // Top-left (relative to global "screen" coordinates)
      this->gui_window_pos[0] = nom::Point2i( 25, 25 );
      this->gui_window_size[0] = nom::Size2i( WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 25 );

      // Top-right (relative to global "screen" coordinates)
      this->gui_window_pos[1] = nom::Point2i( 562, 25 );
      this->gui_window_size[1] = nom::Size2i( WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 25 );

      // Bottom-left (relative to global "screen" coordinates)
      this->gui_window_pos[2] = nom::Point2i( 25, WINDOW_HEIGHT/2 + 11 );
      this->gui_window_size[2] = nom::Size2i( 304 , 190 );  // dataviewlist_ex0

      this->gui_window_pos[3] = nom::Point2i( 339, WINDOW_HEIGHT/2 + 11 );
      this->gui_window_size[3] = nom::Size2i( 344 , 190 );  // dataviewlist_ex1

      this->gui_window_pos[4] = nom::Point2i( 693, WINDOW_HEIGHT/2 + 11 );
      this->gui_window_size[4] = nom::Size2i( 304 , 190 );  // dataviewlist_ex2

      // gui_window[0]

      this->gui_window[0] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[0], this->gui_window_size[0] );
      this->gui_window[0]->set_name( "gui_window[0]" );
      this->gui_window[0]->set_decorator( new nom::MinimalDecorator() );

      // Window-scope mouse button click events
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->gui_window[0]->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

// this->gui_window[0]->set_font( this->bitmap_font );

      this->gui_window[0]->set_title( this->gui_window[0]->name() );

      // gui_window[1]

      this->gui_window[1] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[1], this->gui_window_size[1] );
      this->gui_window[1]->set_decorator( new nom::MinimalDecorator() );

// this->gui_window[1]->set_font( this->truetype_font );

      // Using the auto-generated name for the second window object.
      this->gui_window[1]->set_title( this->gui_window[1]->name() );

      // gui_window[2]

      this->gui_window[2] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[2], this->gui_window_size[2] );

      // FIXME: We can drop the ALL CAPS once we fix the font management issue
      // with objects. Be sure to not forget about re-enabling the set_title
      // for this window!
      this->gui_window[2]->set_name( "GUI_WINDOW[2]" );

      this->gui_window[2]->set_decorator( new nom::FinalFantasyDecorator() );

      this->gui_window[2]->set_font( this->bitmap_small_font );

      // this->gui_window[2]->set_title( this->gui_window[2]->name() );

      // gui_window[3]

      this->gui_window[3] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[3], this->gui_window_size[3] );

      // FIXME: We can drop the ALL CAPS once we fix the font management issue
      // with objects. Be sure to not forget about re-enabling the set_title
      // for this window!
      this->gui_window[3]->set_name( "GUI_WINDOW[3]" );

      this->gui_window[3]->set_decorator( new nom::FinalFantasyDecorator() );

      this->gui_window[3]->set_font( this->bitmap_small_font );

      // this->gui_window[3]->set_title( this->gui_window[3]->name() );

      // gui_window[4]

      this->gui_window[4] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[4], this->gui_window_size[4] );

      // FIXME: We can drop the ALL CAPS once we fix the font management issue
      // with objects. Be sure to not forget about re-enabling the set_title
      // for this window!
      this->gui_window[4]->set_name( "GUI_WINDOW[4]" );

      this->gui_window[4]->set_decorator( new nom::FinalFantasyDecorator() );
      this->gui_window[4]->set_font( this->bitmap_small_font );

      // this->gui_window[4]->set_title( this->gui_window[4]->name() );

      // FIXME: Temporarily using this top-level window to get things up and
      // going
      this->gui_window[5] = new nom::UIWidget( nullptr, -1, nom::Point2i( 175, 41 ), nom::Size2i( 350, 322 ) );
      this->gui_window[5]->set_name( "gui_window[5]" );
// this->gui_window[5]->set_font( this->truetype_font );
      this->gui_window[5]->set_decorator( new nom::MinimalDecorator() );

      this->gui_window[5]->insert_child( this->create_button_ex0() );
      this->gui_window[5]->insert_child( this->create_button_ex1() );
      this->gui_window[5]->insert_child( this->create_bitmap_button_ex0() );

      this->gui_window[0]->insert_child( this->create_listbox_ex0() );
      this->gui_window[0]->insert_child( this->create_listbox_ex1() );
      this->gui_window[0]->insert_child( this->create_listbox_ex2() );

      this->gui_window[1]->insert_child( this->create_messagebox_ex0() );
      this->gui_window[1]->insert_child( this->create_messagebox_ex1() );
      // this->gui_window[1]->insert_child( this->create_question_dialogbox_ex0() );

      this->gui_window[2]->insert_child( this->create_dataviewlist_ex0() );
      this->gui_window[3]->insert_child( this->create_dataviewlist_ex1() );
      this->gui_window[4]->insert_child( this->create_dataviewlist_ex2() );

      // FIXME: Widgets must not be added to the layout until *after* a) font
      // initialization (widget's size hint); b) item store initialization
      // (internal update is necessary within ListBox::set_item_store).
      this->listbox_layout = new nom::UIVBoxLayout( this->gui_window[0] );

      // this->listbox_layout->set_spacing( 0 );
      // this->listbox_layout->set_spacing( 4 );
      // this->listbox_layout->set_spacing( 20 );

      this->listbox_ex0->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );

      this->listbox_layout->append_widget( this->listbox_ex0 );
      this->listbox_layout->append_spacer( 5 );
      this->listbox_layout->append_widget( this->listbox_ex1 );
      this->listbox_layout->append_spacer( 5 );
      this->listbox_layout->append_widget( this->listbox_ex2 );

      this->listbox_layout->set_alignment( this->listbox_ex1, nom::Anchor::Center );
      this->listbox_layout->set_alignment( this->listbox_ex2, nom::Anchor::Center );

      // Relative to global (screen) coordinates of gui_window[0].
      this->listbox_layout->set_position( nom::Point2i( 25, 50 ) );

      // FIXME:
      // this->listbox_layout->set_alignment( nom::Anchor::Right );

      // Button layout
      this->button_layout = new nom::UIHBoxLayout();

      // this->button_ex0->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );
      // this->button_ex1->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );

      this->button_layout->append_widget( this->button_ex0 );
      // this->button_layout->append_spacer( 5 );
      this->button_layout->append_widget( this->button_ex1 );
      this->button_layout->append_widget( this->button_ex2 );

      // this->button_layout->set_alignment( this->button_ex0, nom::Anchor::Center );

      this->gui_window[5]->set_layout( this->button_layout );

      this->button_layout->set_position( nom::Point2i( 12, 50 ) );

      // NOM_ASSERT( this->gui_window[5]->find_child( "button_ex0 widget" ) != nullptr );
      // NOM_ASSERT( this->gui_window[5]->find_child( "button_ex1 widget" ) != nullptr );

      // Auto-generated name
      // NOM_ASSERT( this->gui_window[5]->find_child( "bitmap_button" ) != nullptr );

      NOM_ASSERT( this->gui_window[0]->find_child( "listbox_ex0" ) != nullptr );
      NOM_ASSERT( this->gui_window[0]->find_child( "listbox_ex1" ) != nullptr );
      NOM_ASSERT( this->gui_window[0]->find_child( "listbox_ex2" ) != nullptr );

      NOM_ASSERT( this->gui_window[0]->id() == 1 );
      NOM_ASSERT( this->gui_window[0]->name() == "gui_window[0]" );
      NOM_ASSERT( this->gui_window[0]->title() == "gui_window[0]" );

      NOM_ASSERT( this->gui_window[1]->id() == 2 );
      // Auto-generated name
      NOM_ASSERT( this->gui_window[1]->name() == "window" );
      NOM_ASSERT( this->gui_window[1]->title() == "window" );

      NOM_ASSERT( this->gui_window[2]->id() == 3 );
      // NOM_ASSERT( this->gui_window[2]->name() == "GUI_WINDOW[2]" );
      NOM_ASSERT( this->gui_window[2]->title() == "" );

      NOM_ASSERT( this->gui_window[3]->id() == 4 );
      // NOM_ASSERT( this->gui_window[3]->name() == "GUI_WINDOW[3]" );
      NOM_ASSERT( this->gui_window[3]->title() == "" );

      NOM_ASSERT( this->gui_window[4]->id() == 5 );
      // NOM_ASSERT( this->gui_window[4]->name() == "GUI_WINDOW[4]" );
      NOM_ASSERT( this->gui_window[4]->title() == "" );

      nom::UIWidget* widget = nullptr;

      // button_ex0
      // nom::UIWidget* widget = this->gui_window[5]->find_child( 6 );
      // NOM_ASSERT( widget != nullptr );

      // if( widget != nullptr )
      // {
      //   NOM_ASSERT( widget->is_window() == false );
      //   NOM_ASSERT( widget->name() == "button_ex0 widget" );
      // }

      // listbox_ex2
      widget = this->gui_window[0]->find_child( 99 );
      NOM_ASSERT( widget != nullptr );

      if( widget != nullptr )
      {
        NOM_ASSERT( widget->is_window() == false );
        NOM_ASSERT( widget->name() == "listbox_ex2" );
      }

      widget = nullptr;

      NOM_ASSERT( this->gui_window[0]->is_window() == true );
      NOM_ASSERT( this->gui_window[1]->is_window() == true );
      NOM_ASSERT( this->gui_window[2]->is_window() == true );
      NOM_ASSERT( this->gui_window[3]->is_window() == true );
      NOM_ASSERT( this->gui_window[4]->is_window() == true );

      return true;
    }

#if defined( NOM_BUILD_BUTTON_EX0 )
    /// \brief Test the creation of a Button widget with a BitmapFont resource.
    nom::Button* create_button_ex0( void )
    {
      // Layout managed
      // nom::Point2i pos( 190, 50 );
      nom::Point2i pos = nom::Point2i::null;

      // Minimum size
      nom::Size2i size( 50, 25 );

      this->button_ex0 = nullptr;

      this->button_ex0 = new nom::Button( this->gui_window[5], -1, pos, size );

      // Custom font for the text label
      this->button_ex0->set_font( nom::SystemFonts::cache().load_resource( "VIII" ) );

      this->button_ex0->set_name( "button_ex0 widget" );
      this->button_ex0->set_decorator( new nom::FinalFantasyDecorator() );
      // this->button_ex0->set_decorator( new nom::MinimalDecorator() );

      // Label text events
      this->button_ex0->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_click( ev ); } ) );
      this->button_ex0->set_label( "button_ex0" );

      NOM_ASSERT( this->button_ex0->name() == "button_ex0 widget" );
      // NOM_ASSERT( this->button_ex0->position() == pos );
      // NOM_ASSERT( this->button_ex0->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->button_ex0->size() == size );
      NOM_ASSERT( this->button_ex0->label_text() == "button_ex0" );

      return this->button_ex0;
    }
#endif // defined NOM_BUILD_BUTTON_EX0

#if defined( NOM_BUILD_BUTTON_EX1 )
    /// \brief Test the creation of a Button widget with a TrueTypeFont resource.
    nom::Button* create_button_ex1( void )
    {
      // Layout managed
      // nom::Point2i pos( 300, 50 );

      nom::Point2i pos = nom::Point2i::null;

      nom::Size2i size( 50, 25 );

      this->button_ex1 = nullptr;

      this->button_ex1 = new nom::Button( this->gui_window[5], -1, pos, size );
// this->button_ex1->set_font( this->truetype_font );
      this->button_ex1->set_name( "button_ex1 widget" );

      this->button_ex1->set_decorator( new nom::MinimalDecorator() );

      // Label text events
      this->button_ex1->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_click( ev ); } ) );

      this->button_ex1->set_label( "button_ex1" );

      NOM_ASSERT( this->button_ex1->name() == "button_ex1 widget" );
      // NOM_ASSERT( this->button_ex1->position() == pos );
      // NOM_ASSERT( this->button_ex1->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->button_ex1->size() == size );
      NOM_ASSERT( this->button_ex1->label_text() == "button_ex1" );

      return this->button_ex1;
    }
#endif // defined NOM_BUILD_BUTTON_EX1

#if defined( NOM_BUILD_BITMAP_BUTTON_EX0 )
    /// \brief Test the creation of a BitmapButton widget.
    ///
    /// \remarks Note that this is purely a convenience widget for us; we could
    /// have done the same thing by creating a nom::Button with a nom::Image
    /// passed to a nom::UIWidget via ::set_shape.
    nom::BitmapButton* create_bitmap_button_ex0( void )
    {
      // button_v1
      // nom::Size2i size( 77, 25 );

      // Layout managed
      // nom::Point2i pos( 405, 64 );
      nom::Point2i pos = nom::Point2i::null;

      // button_v2
      nom::Size2i size( 102, 25 );

      this->button_ex2 = nullptr;

      // TODO: Create separate examples (including a nullptr test):
      // button = new nom::BitmapButton( window, pos, size, nullptr );
      // button = new nom::BitmapButton( this->gui_window[0], -1, pos, size, &this->button_bg[0] );
      // button = new nom::BitmapButton( this->gui_window[0], -1, pos, size, &this->button_bg[0] );

      this->button_ex2 = new nom::BitmapButton( this->gui_window[5], -1, pos, size, &this->button_bg[2] );
// this->button_ex2->set_font( this->truetype_font2 );

      nom::UIStyle::shared_ptr custom_style = nullptr;
      custom_style.reset( new nom::UIStyle() );

      custom_style->set_font_color( nom::Color4i::Black );
      custom_style->set_font_size( 13 );
      custom_style->set_font_style( nom::Text::Style::Bold );

      this->button_ex2->set_style( custom_style );

      // Use the auto-generated name for this widget.

      // Button action for on mouse hover event (enter)
      //
      // FIXME:
      // button->register_event_listener( 2, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_hover_enter( ev ); } ) );

      // Button action for on mouse hover event (leave)
      //
      // FIXME:
      // button->register_event_listener( 3, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_hover_leave( ev ); } ) );

      // Label text events
      this->button_ex2->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_click( ev ); } ) );

      this->button_ex2->set_label( "Hello!" );

      NOM_ASSERT( this->button_ex2->name() == "bitmap_button" );
      // NOM_ASSERT( button->position() == pos );
      // NOM_ASSERT( this->button_ex2->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->button_ex2->size() == size );
      NOM_ASSERT( this->button_ex2->label_text() == "Hello!" );

      return this->button_ex2;
    }
#endif // defined NOM_BUILD_BITMAP_BUTTON_EX0

#if defined( NOM_BUILD_LISTBOX_EX0 )
    /// \brief Construct a minimal ListBox widget, with a custom selection text
    /// color, but no default selection item, giving the end-user the
    /// responsibility of choosing an item before seeing a result.
    ///
    /// \remarks No item has been set as the defaulted selection, which gives
    /// you the responsibility of choosing an item from the list in order to
    /// see a result.
    nom::ListBox* create_listbox_ex0( void )
    {
      // Absolute dimensions
      // nom::Point2i pos( 25, 50 );

      // Layout managed
      nom::Point2i pos = nom::Point2i::null;

      nom::Size2i size( 50, 24 );
      this->listbox_ex0 = nullptr;

      nom::ItemStrings choice_selection = { std::string( "boobies!" ), std::string( "more boobies!" ) };

      nom::UIItemContainer::raw_ptr storage = new nom::UIItemContainer();
      storage->append( choice_selection );

      this->listbox_ex0 = new nom::ListBox( this->gui_window[0], -1, pos, size, storage );
      this->listbox_ex0->set_name( "listbox_ex0" );
// this->listbox_ex0->set_font( this->truetype_font );
      this->listbox_ex0->set_decorator( new nom::MinimalDecorator() );
      this->listbox_ex0->set_selected_text_color( nom::Color4i::Gray );

      // Trigger a callback when an item has a mouse button click
      // FIXME: Temporarily disabled (
      // this->listbox_ex0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->item_on_selection( ev ); } ) );

      NOM_ASSERT( this->listbox_ex0->name() == "listbox_ex0" );
      // NOM_ASSERT( this->listbox_ex0->position() == pos );
      NOM_ASSERT( this->listbox_ex0->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->listbox_ex0->size() == size );

      nom::UIItemContainer::raw_ptr items = this->listbox_ex0->store();
      nom::ItemStrings choices = items->item_labels();
      NOM_ASSERT( choices[0] == "boobies!" );
      NOM_ASSERT( choices[1] == "more boobies!" );

      return this->listbox_ex0;
    }
#endif // defined NOM_BUILD_LISTBOX_EX0

#if defined( NOM_BUILD_LISTBOX_EX1 )

    /// \brief Construct a ListBox widget using a TrueType font.
    nom::ListBox* create_listbox_ex1( void )
    {
      // Absolute dimensions
      // nom::Point2i pos( 25, 150 );

      // Layout managed
      nom::Point2i pos = nom::Point2i::null;

      nom::Size2i size( 50, 24 );
      this->listbox_ex1 = nullptr;

      nom::ItemStrings choice_selection = { std::string( "item_0" ), std::string( "item_1" ), std::string( "item_2" ) };

      nom::UIItemContainer::raw_ptr storage = new nom::UIItemContainer();

      storage->append( choice_selection );
      storage->set_selection( 1 );

      this->listbox_ex1 = new nom::ListBox( this->gui_window[0], -1, pos, size, storage );
      this->listbox_ex1->set_name( "listbox_ex1" );
      this->listbox_ex1->set_decorator( new nom::MinimalDecorator() );

      // Trigger a callback when an item has a mouse button click
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->listbox_ex1->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->item_on_selection( ev ); } ) );

      NOM_ASSERT( this->listbox_ex1->name() == "listbox_ex1" );
      // NOM_ASSERT( this->listbox_ex1->position() == pos );
      NOM_ASSERT( this->listbox_ex1->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->listbox_ex1->size() == size );

      nom::UIItemContainer::raw_ptr items = this->listbox_ex1->store();

      nom::ItemStrings choices = items->item_labels();
      NOM_ASSERT( choices[0] == "item_0" );
      NOM_ASSERT( choices[1] == "item_1" );

      NOM_ASSERT( items->selection() == 1 );

      return this->listbox_ex1;
    }
#endif // defined NOM_BUILD_LISTBOX_EX1

#if defined( NOM_BUILD_LISTBOX_EX2 )
    /// \brief Construct a ListBox widget using a TrueType font.
    ///
    /// \remarks Test keyboard, mouse button and wheel events.
    nom::ListBox* create_listbox_ex2( void )
    {
      // Absolute dimensions
      // nom::Point2i pos( 25, 250 );

      // Layout managed
      nom::Point2i pos = nom::Point2i::null;

      nom::Size2i size( 50, 24 );
      this->listbox_ex2 = nullptr;

      nom::ItemStrings choice_selection = { std::string( "item_0" ), std::string( "item_1" ), std::string( "item_2" ), std::string( "item_3" ) };

      nom::UIItemContainer::raw_ptr storage = new nom::UIItemContainer();

      storage->append( choice_selection );
      storage->set_selection( 1 );

      this->listbox_ex2 = new nom::ListBox( this->gui_window[0], 99, pos, size, storage );
      this->listbox_ex2->set_font( this->truetype_font );
      this->listbox_ex2->set_name( "listbox_ex2" );
      this->listbox_ex2->set_decorator( new nom::MinimalDecorator() );

      // Trigger a callback when a focused widget receives key strokes, mouse
      // button clicks (on a text label) or receives a mouse wheel event.
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->listbox_ex2->register_event_listener( nom::UIEvent::KEY_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );
      // this->listbox_ex2->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // FIXME: Temporarily disabled (to cease debugging output):
      // this->listbox_ex2->register_event_listener( nom::UIEvent::MOUSE_WHEEL, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      NOM_ASSERT( this->listbox_ex2->id() == 99 );
      NOM_ASSERT( this->listbox_ex2->name() == "listbox_ex2" );
      // NOM_ASSERT( this->listbox_ex2->position() == pos );
      NOM_ASSERT( this->listbox_ex2->parent()->position() == this->gui_window[0]->position() );
      NOM_ASSERT( this->listbox_ex2->size() == size );

      nom::UIItemContainer::raw_ptr items = this->listbox_ex2->store();

      nom::ItemStrings choices = items->item_labels();
      NOM_ASSERT( choices[0] == "item_0" );
      NOM_ASSERT( choices[1] == "item_1" );
      NOM_ASSERT( choices[2] == "item_2" );
      NOM_ASSERT( choices[3] == "item_3" );

      NOM_ASSERT( items->selection() == 1 );

      return this->listbox_ex2;
    }
#endif // defined NOM_BUILD_LISTBOX_EX2

#if defined( NOM_BUILD_MESSAGEBOX_EX0 )
    nom::MessageBox* create_messagebox_ex0( void )
    {
      nom::Point2i pos( 38, 25 );
      nom::Size2i size( 200, 48 );

      nom::MessageBox* info_box = nullptr;

      info_box = new nom::MessageBox  (
                                        this->gui_window[1],
                                        -1,
                                        pos,
                                        size
                                      );

      info_box->set_decorator( new nom::FinalFantasyDecorator() );

      info_box->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );
      info_box->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->item_on_selection( ev ); } ) );

      info_box->set_title_label( nom::Text ( RESOURCE_INFO_BOX_TITLE_STRINGS[0], this->bitmap_small_font, 8, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0] ) );
      info_box->set_message_label( nom::Text ( RESOURCE_INFO_BOX_TEXT_STRINGS[0], this->bitmap_font, 12, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4] ) );

      // NOM_ASSERT( info_box->position() == pos );
      NOM_ASSERT( info_box->parent()->position() == this->gui_window[1]->position() );
      NOM_ASSERT( info_box->size() == size );
      NOM_ASSERT( info_box->title_text() == RESOURCE_INFO_BOX_TITLE_STRINGS[0] );
      NOM_ASSERT( info_box->message_text() == RESOURCE_INFO_BOX_TEXT_STRINGS[0] );

      return info_box;
    }
#endif // defined NOM_BUILD_MESSAGEBOX_EX0

#if defined( NOM_BUILD_MESSAGEBOX_EX1 )
    /// \brief Construct a minimal message dialog box widget
    nom::MessageBox* create_messagebox_ex1( void )
    {
      nom::Point2i pos( 38, 83 );
      nom::Size2i size( 300, 48 );

      nom::MessageBox* info_box = nullptr;

      info_box = new nom::MessageBox  (
                                        this->gui_window[1],
                                        -1,
                                        pos,
                                        size
                                      );

      info_box->set_decorator( new nom::FinalFantasyDecorator() );
      info_box->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      info_box->set_title_label( nom::Text ( RESOURCE_INFO_BOX_TITLE_STRINGS[1], this->bitmap_small_font, 8, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0] ) );
      info_box->set_message_label( nom::Text ( RESOURCE_INFO_BOX_TEXT_STRINGS[2], this->bitmap_font, 14, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4] ) );

      // NOM_ASSERT( info_box->position() == pos );
      NOM_ASSERT( info_box->parent()->position() == this->gui_window[1]->position() );
      NOM_ASSERT( info_box->size() == size );
      NOM_ASSERT( info_box->title_text() == RESOURCE_INFO_BOX_TITLE_STRINGS[1] );
      NOM_ASSERT( info_box->message_text() == RESOURCE_INFO_BOX_TEXT_STRINGS[2] );

      return info_box;
    }
#endif // defined NOM_BUILD_MESSAGEBOX_EX1

#if defined( NOM_BUILD_QUESTION_DIALOGBOX_EX0 )
    /// \brief Construct a simple question dialog box that registers event
    /// actions for window-scope mouse clicks -- MOUSE_BUTTON, checked from the
    /// base class MessageBox::process_event, text labels (MOUSE_SELECTION), checked
    /// again from the base class (MessageBox), and finally, a third event
    /// observer responded to from within QuestionDialogBox::process_event.
    nom::QuestionDialogBox* create_question_dialogbox_ex0( void )
    {
      nom::QuestionDialogBox::ItemStrings labels;

      nom::Point2i pos( 38, 141 );
      nom::Size2i size( 124, 72 );  // TTcards dimensions: 232, 72

      this->qbox_ex0 = nullptr;

      this->qbox_ex0 = new nom::QuestionDialogBox  (
                                                        this->gui_window[1],
                                                        -1,
                                                        pos,
                                                        size
                                                      );

      this->qbox_ex0->set_decorator( new nom::FinalFantasyDecorator() );

      // NOTE: Uses nom::MessageBox::process_event method call.
      //
      // Delegate 'Type 1' is emitted when either the caption (title) or message
      // label bounds are double clicked upon by the user's mouse.
      this->qbox_ex0->register_event_listener( nom::UIEvent::MOUSE_DCLICK, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // NOTE: Uses nom::QuestionDialogBox::process_event method call.
      //
      // Delegate 'Type 1' is emitted when the user's mouse click is within the
      // label bounds of a choice text.
      //
      // Note that we *must* always execute virtual method ::register_delegate
      // from our widget pointer, and not from the window pointer. Doing so will
      // result in an error due to an empty callback table. This is because
      // QuestionDialogBox's derived class is *not* Window, as is commonly seen,
      // but instead inherits from MessageBox, whom then inherits from UIWidget.
      this->qbox_ex0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button_on_click( ev ); } ) );

      // FIXME: Temporarily disabled (to cease debugging output):
      // this->qbox_ex0->register_event_listener( nom::UIEvent::MOUSE_WHEEL, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->qbox_ex0_wheel_selection( ev ); } ) );
      // this->qbox_ex0->register_event_listener( nom::UIEvent::KEY_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->qbox_ex0_key_selection( ev ); } ) );

      this->qbox_ex0->set_title_label( nom::Text( RESOURCE_INFO_BOX_TITLE_STRINGS[2], this->bitmap_small_font, -1, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[0] ) );
      this->qbox_ex0->set_message_label( nom::Text( RESOURCE_INFO_BOX_TEXT_STRINGS[1], this->bitmap_font, -1, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[1] ) );

      nom::Text yes_label( nom::Text( "Yes", this->bitmap_font, -1, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4] ) );

      // FIXME (?):
      //
      // Appending a single space character onto "No" text label so that it
      // lines up with our "Yes" text label.
      nom::Text no_label( nom::Text( "No ", this->bitmap_font, -1, RESOURCE_INFO_BOX_TEXT_ALIGNMENTS[4] ) );

      this->qbox_ex0->append_choice( yes_label );
      this->qbox_ex0->append_choice( no_label );
      this->qbox_ex0->set_selection( 1 );

      // FIXME:
      //
      // labels = this->qbox_ex0->choices();
      // NOM_ASSERT( labels[0] == "Yes" );
      // NOM_ASSERT( labels[1] == "No " );

      // NOM_ASSERT( this->qbox_ex0->position() == pos );
      NOM_ASSERT( this->qbox_ex0->parent()->position() == this->gui_window[1]->position() );
      NOM_ASSERT( this->qbox_ex0->size() == size );
      NOM_ASSERT( this->qbox_ex0->title_text() == RESOURCE_INFO_BOX_TITLE_STRINGS[2] );
      NOM_ASSERT( this->qbox_ex0->message_text() == RESOURCE_INFO_BOX_TEXT_STRINGS[1] );
      NOM_ASSERT( this->qbox_ex0->selection() == 1 );

      return this->qbox_ex0;
    }
#endif // defined NOM_BUILD_QUESTION_DIALOGBOX_EX0

#if defined( NOM_BUILD_DATAVIEWLIST_EX0 )
    nom::DataViewList* create_dataviewlist_ex0( void )
    {
      nom::Point2i pos( 0, 0 );
      nom::Size2i size( 204, 160 );

      nom::DataViewList* dview = nullptr;

      dview = new nom::DataViewList( this->gui_window[2], -1, pos, size );

      // TODO:
      // dview->register_event_listener( 1, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->dview_on_selection( ev ); } ) );

      nom::DataViewColumn col_0( 0, "CARDS  PG. 1", 75, nom::IDataViewColumn::Alignment::Left );
      nom::DataViewColumn col_1( 1, "NUM.", 75, nom::IDataViewColumn::Alignment::Right );

      dview->append_column( col_0 );
      dview->append_column( col_1 );

      // Column 0 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_0;
      values_0.push_back( new nom::Text( "TEST_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_1", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_2", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_x", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_xx", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "VALUE_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "VALUE_1", this->bitmap_font ) );

      // Column 1 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_1;
      values_1.push_back( new nom::Text( "Test_3", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_4", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_5", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "TEST_6", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "TEST_XXX", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "VALUE_2", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "value_3", this->bitmap_font ) );

      // FIXME:
      // dview->append_item( values_0 );
      // dview->append_item( values_1 );

      dview->insert_item( 0, values_0 );
      dview->insert_item( 1, values_1 );

      // These should work as long as we ensure that drawable_items_ is cleared
      // within the update_items method.
      //
      // FIXME:
      // dview->insert_item( 0, 0, new nom::Text("VALUE_HAXXEDDDDDDD" ) );
      // dview->insert_item( 1, 5, new nom::Text("VALUE_HAXXED" ) );

      // NOM_ASSERT( dview->position() == pos );
      NOM_ASSERT( dview->parent()->position() == this->gui_window[2]->position() );
      NOM_ASSERT( dview->size() == size );

      nom::DataViewListStore::ColumnNames cols = dview->column_names();

      // Total number of inserted columns
      NOM_ASSERT( dview->columns_size() == 2 );
      NOM_ASSERT( cols[0] == "CARDS  PG. 1" );
      NOM_ASSERT( cols[1] == "NUM." );

      // Column item total
      NOM_ASSERT( dview->items_size( 0 ) == 7 );
      NOM_ASSERT( dview->items_size( 1 ) == 7 );

      // Alternative API sanity test #1
      NOM_ASSERT( col_0.title() == "CARDS  PG. 1" );
      NOM_ASSERT( col_1.title() == "NUM." );

      // Alternative API sanity test #2
      nom::DataViewColumn c;

      c = dview->column( 0 );
      NOM_ASSERT( c.title() == "CARDS  PG. 1" );

      c = dview->column( 1 );
      NOM_ASSERT( c.title() == "NUM." );

      return dview;
    }
#endif // defined NOM_BUILD_DATAVIEWLIST_EX0

#if defined( NOM_BUILD_DATAVIEWLIST_EX1 )
    /// \remarks Un-even numbered columns tests, including a mixture of both
    /// Bitmap & TrueType font usage.
    ///
    /// \NOTE We intentionally make the height of the widget shorter than the
    /// minimum size required to hold the second set of column values.
    nom::DataViewList* create_dataviewlist_ex1( void )
    {
      nom::Point2i pos( 0, 0 );
      nom::Size2i size( 344, 190 );

      nom::DataViewList* dview = nullptr;

      // Column 0 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_0;
      values_0.push_back( new nom::Text( "TEST_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_1", this->truetype_font ) );
      values_0.push_back( new nom::Text( "TEST_2", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_X", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "TEST_XX", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "VALUE_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "VALUE_1", this->truetype_font ) );

      // Column 1 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_1;
      values_1.push_back( new nom::Text( "TEST_3", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "TEST_4", this->truetype_font ) );
      values_1.push_back( new nom::Text( "TEST_5", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "TEST_6", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "TEST_XXX", this->truetype_font ) );
      values_1.push_back( new nom::Text( "VALUE_1", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "VALUE_2", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "VALUE_3", this->truetype_font ) );
      values_1.push_back( new nom::Text( "VALUE_4", this->bitmap_font ) );

      // Column 2 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_2;
      values_2.push_back( new nom::Text( "1", this->truetype_font ) );
      values_2.push_back( new nom::Text( "1", this->bitmap_font ) );
      values_2.push_back( new nom::Text( "6", this->bitmap_font ) );
      values_2.push_back( new nom::Text( "3", this->bitmap_font ) );
      values_2.push_back( new nom::Text( "1", this->bitmap_font ) );
      values_2.push_back( new nom::Text( "4", this->bitmap_font ) );
      values_2.push_back( new nom::Text( "6", this->truetype_font ) );

      dview = new nom::DataViewList( this->gui_window[3], -1, pos, size );

      // TODO:
      // dview->register_event_listener( 1, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->dview_on_selection( ev ); } ) );

      nom::DataViewColumn col_0( 0, "CARDS  PG. 1", 55, nom::IDataViewColumn::Alignment::Left );
      nom::DataViewColumn col_1( 1, "TCOL.", 75, nom::IDataViewColumn::Alignment::Center );
      nom::DataViewColumn col_2( 2, "NUM.", 50, nom::IDataViewColumn::Alignment::Right );

      dview->append_column( col_0 );
      dview->append_column( col_1 );
      dview->append_column( col_2 );

      dview->insert_item( 0, values_0 );
      dview->insert_item( 1, values_1 );
      dview->insert_item( 2, values_2 );

      // These should work as long as we ensure that drawable_items_ is cleared
      // within the update_items method.
      //
      // FIXME:
      // dview->insert_item( 0, 0, nom::Text("VALUE_HAXXEDDDDDDD") );
      // dview->insert_item( 1, 5, nom::Text("VALUE_HAXXED") );

      // NOM_ASSERT( dview->position() == pos );
      // NOM_ASSERT( dview->parent()->position() == this->gui_window[3]->position() );
      NOM_ASSERT( dview->size() == size );

      nom::DataViewListStore::ColumnNames cols = dview->column_names();

      // Total number of inserted columns
      NOM_ASSERT( dview->columns_size() == 3 );

      NOM_ASSERT( cols[0] == "CARDS  PG. 1" );
      NOM_ASSERT( cols[1] == "TCOL." );
      NOM_ASSERT( cols[2] == "NUM." );

      // Column item total
      NOM_ASSERT( dview->items_size( 0 ) == 7 );
      NOM_ASSERT( dview->items_size( 1 ) == 9 );
      NOM_ASSERT( dview->items_size( 2 ) == 7 );

      // Misc. API sanity tests
      NOM_ASSERT( col_0.title() == "CARDS  PG. 1" );
      NOM_ASSERT( col_1.title() == "TCOL." );
      NOM_ASSERT( col_2.title() == "NUM." );

      nom::DataViewColumn c;

      c = dview->column( 0 );
      NOM_ASSERT( c.title() == "CARDS  PG. 1" );

      c = dview->column( 1 );
      NOM_ASSERT( c.title() == "TCOL." );

      c = dview->column( 2 );
      NOM_ASSERT( c.title() == "NUM." );

      nom::DataViewItem<nom::IDrawable::raw_ptr> item = dview->item( 0, 0 );
      NOM_ASSERT( NOM_ISA( nom::Text*, item.data() ) );

      return dview;
    }
#endif // defined NOM_BUILD_DATAVIEWLIST_EX1

#if defined( NOM_BUILD_DATAVIEWLIST_EX2 )
    nom::DataViewList* create_dataviewlist_ex2( void )
    {
      nom::Point2i pos( 0, 0 );
      nom::Size2i size( 304, 190 );

      nom::DataViewList* dview = nullptr;

      // 'Enabled' menu element frame.
      this->menu_elements.set_frame ( 0 );

      // Column 0 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_0;
      values_0.push_back( new nom::Text( "Test_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Test_1", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Test_2", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Test_X", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Test_XX", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Value_0", this->bitmap_font ) );
      values_0.push_back( new nom::Text( "Value_1", this->bitmap_font ) );

      // Column 1 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_1;
      values_1.push_back( new nom::Text( "Test_10", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_11", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_12", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_13", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Test_14", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "Value_15", this->bitmap_font ) );
      values_1.push_back( new nom::Text( "VALUE_16", this->bitmap_font ) );

      // Column 2 values
      std::vector<nom::DataViewItem<nom::IDrawable::raw_ptr>> values_2;

      // FIXME: Optimize the sprite cloning process below -- very similar in
      // concept to IFont copying; see notes in Text.hpp regarding ::set_font.

      // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

      // 'Disabled' menu element frame:
      this->menu_elements.set_frame( 1 );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

      // 'Enabled' menu element frame:
      this->menu_elements.set_frame( 0 );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

      // 'Disabled' menu element frame:
      this->menu_elements.set_frame( 1 );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

      // Right page cursor frame:
      this->menu_elements.set_frame( 3 );

       // Note that the sprite is internally cloned here as well
      values_2.push_back( this->menu_elements.clone() );

      dview = new nom::DataViewList( this->gui_window[4], -1, pos, size );
      dview->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->dview_on_selection( ev ); } ) );

      nom::DataViewColumn col_0( 0, "CARDS  PG. 1", 55, nom::IDataViewColumn::Alignment::Left );
      nom::DataViewColumn col_1( 1, "TCOL.", 75, nom::IDataViewColumn::Alignment::Center );
      nom::DataViewColumn col_2( 2, "NUM.", 75, nom::IDataViewColumn::Alignment::Right );

      dview->append_column( col_0 );
      dview->append_column( col_1 );
      dview->append_column( col_2 );

      dview->insert_item( 0, values_0 );
      dview->insert_item( 1, values_1 );
      dview->insert_item( 2, values_2 );

      // These should work as long as we ensure that drawable_items_ is cleared
      // within the update_items method.
      //
      // FIXME:
      // dview->insert_item( 0, 0, new nom::Text( "VALUE_HAXXEDDDDDDD", this->bitmap_font ) );
      // dview->insert_item( 1, 5, new nom::Text( "VALUE_HAXXED", this->bitmap_font ) );

      // NOM_ASSERT( dview->position() == pos );
      NOM_ASSERT( dview->parent()->position() == this->gui_window[4]->position() );
      NOM_ASSERT( dview->size() == size );

      nom::DataViewListStore::ColumnNames cols = dview->column_names();

      // Total number of inserted columns
      NOM_ASSERT( dview->columns_size() == 3 );

      NOM_ASSERT( cols[0] == "CARDS  PG. 1" );
      NOM_ASSERT( cols[1] == "TCOL." );
      NOM_ASSERT( cols[2] == "NUM." );

      // Column item total
      NOM_ASSERT( dview->items_size( 0 ) == 7 );
      NOM_ASSERT( dview->items_size( 1 ) == 7 );
      NOM_ASSERT( dview->items_size( 1 ) == 7 );

      // Misc API sanity tests
      NOM_ASSERT( col_0.title() == "CARDS  PG. 1" );
      NOM_ASSERT( col_1.title() == "TCOL." );
      NOM_ASSERT( col_2.title() == "NUM." );

      nom::DataViewColumn c;

      c = dview->column( 0 );
      NOM_ASSERT( c.title() == "CARDS  PG. 1" );

      c = dview->column( 1 );
      NOM_ASSERT( c.title() == "TCOL." );

      c = dview->column( 2 );
      NOM_ASSERT( c.title() == "NUM." );

      nom::DataViewItem<nom::IDrawable::raw_ptr> item = dview->item( 2, 0 );
      NOM_ASSERT( NOM_ISA( nom::SpriteBatch*, item.data() ) );

      return dview;
    }
#endif // defined NOM_BUILD_DATAVIEWLIST_EX2

    void window_on_click( const nom::UIWidgetEvent& ev ) const
    {
      nom::Event event = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      if( event.type == SDL_MOUSEBUTTONDOWN )
      {
        // event.dump();

        // Dump lots of extra debugging data
        // NOM_DUMP( event.type );
        // NOM_DUMP( event.mouse.x );
        // NOM_DUMP( event.mouse.y );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.button ) );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.clicks ) );
      }
    }

    void item_on_selection( const nom::UIWidgetEvent& ev ) const
    {
      nom::Event event = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      // event.dump();

      if( event.type == SDL_MOUSEBUTTONDOWN )
      {
        // NOM_DUMP( event.mouse.x );
        // NOM_DUMP( event.mouse.y );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.button ) );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.clicks ) );
      }
    }

    /// \brief Specialized event listener for qbox_ex0 widget so that we are
    /// able to deal with key press actions.
    void qbox_ex0_key_selection( const nom::UIWidgetEvent& ev ) const
    {
      nom::Event event = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      // event.dump();

      if( event.type == SDL_KEYDOWN )
      {
        // NOM_DUMP( event.key.sym );

        int selected = this->qbox_ex0->selection();

        if( event.key.sym == SDLK_DOWN && selected < this->qbox_ex0->items_size()-1 )
        {
          selected++;
          this->qbox_ex0->set_selection( selected );
        }
        else if( event.key.sym == SDLK_UP && selected > 0 )
        {
          selected--;
          this->qbox_ex0->set_selection( selected );
        }
      }
    }

    /// \brief Specialized event listener for qbox_ex0 widget so that we are
    /// able to deal with mouse wheel actions.
    void qbox_ex0_wheel_selection( const nom::UIWidgetEvent& ev )
    {
      nom::Event event = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      // event.dump();

      if( event.type == SDL_MOUSEBUTTONDOWN )
      {
        // NOM_DUMP( event.mouse.x );
        // NOM_DUMP( event.mouse.y );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.button ) );
        // NOM_DUMP( NOM_SCAST( int, event.mouse.clicks ) );
      }
      else if( event.type == SDL_MOUSEWHEEL )
      {
        // NOM_DUMP( event.wheel.x );
        // NOM_DUMP( event.wheel.y );

        int selected = this->qbox_ex0->selection();

        // Up
        if( event.wheel.y > 0 && selected > 0 )
        {
          --selected;
          this->qbox_ex0->set_selection( selected );
        }
        // Down
        else if( event.wheel.y < 0 && ( selected < this->qbox_ex0->items_size() - 1 ) )
        {
          ++selected;
          this->qbox_ex0->set_selection( selected );
        }
      }
    }

    void button_on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

    /// \todo Implement logic -- called in bitmap_button_ex0.
    void button_on_hover_enter( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      // nom::Text label;

      // TODO:
      // label = nom::Text( "bitmap_button", this->bitmap_font, 14, nom::Text::Alignment::MiddleCenter );
      // label.set_color( nom::Color4i::Red );
      // button->set_label( label );
    }

    /// \todo Implement logic -- called in bitmap_button_ex0.
    void button_on_hover_leave( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      // nom::Text label;

      // NOM_DUMP( ev.index() );
      // NOM_DUMP( ev.text() );

      // TODO:
      // label = nom::Text( "bitmap_button", this->bitmap_font, 14, nom::Text::Alignment::MiddleCenter );
      // label.set_color( nom::Color4i::White );
      // button->set_label( label );
    }

    void dview_on_selection( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

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
        while( this->poll_event( this->event ) )
        {
          this->on_event( this->event );

          for( auto idx = 0; idx != GUI_WINDOWS; ++idx )
          {
            this->gui_window[idx]->process_event( this->event );
          }
        }

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

        for( auto idx = 0; idx != GUI_WINDOWS; ++idx )
        {
          this->gui_window[idx]->update();
        }

        this->window[0].fill( nom::Color4i::SkyBlue );

        for( auto idx = 0; idx != GUI_WINDOWS; ++idx )
        {
          this->gui_window[idx]->draw( this->window[0] );
        }

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
        case SDLK_q: this->on_app_quit( ev ); break;

        case SDLK_F1:
        {
          if( this->window[ev.key.window_id - 1].window_id() == ev.key.window_id )
          {
            if( this->window[ev.key.window_id - 1].save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
            {
              nom::DialogMessageBox( APP_NAME, "Could not save screenshot");
              break;
            } // end save_screenshot err check
          } // end window_id check
          break;
        }

        case SDLK_f:
        {
          if ( this->window[ev.key.window_id - 1].window_id() == ev.key.window_id )
          {
            this->window[ev.key.window_id - 1].toggle_fullscreen();
          } // end window_id match
          break;
        } // end SDLK_f
      } // end switch key
    } // onKeyDown

  int do_event_tests( void )
  {
/*
    // nom::IEvent e1;
    // NOM_ASSERT( e1.type() == 0 );
    // NOM_ASSERT( e1.timestamp() == 0 );

    nom::UIKeyEvent e2( SDL_KEYDOWN, SDLK_UP );
    NOM_ASSERT( e2.type() == SDL_KEYDOWN );
    NOM_ASSERT( e2.timestamp() == 0 );
    NOM_ASSERT( e2.symbol() == SDLK_UP );
    NOM_ASSERT( e2.modifier() == KMOD_NONE );

    // nom::UIKeyEvent* e2p = e2.clone();
    // NOM_ASSERT( e2p->type() == SDL_KEYDOWN );
    // NOM_ASSERT( e2p->timestamp() == 0 );
    // NOM_ASSERT( e2p->symbol() == SDLK_UP );
    // NOM_ASSERT( e2p->modifier() == KMOD_NONE );

    nom::IEvent* i2p = dynamic_cast<nom::IEvent*>( e2.clone() );
    NOM_ASSERT( i2p->type() == SDL_KEYDOWN );
    NOM_ASSERT( i2p->timestamp() == 0 );
    // NOM_ASSERT( i2p->symbol() == SDLK_UP );
    // NOM_ASSERT( i2p->modifier() == KMOD_NONE );

    // Success!
*/
    return 0;
  }

  private:
    static const nom::sint GUI_WINDOWS = 6;

    nom::Event event;

    /// Window handles
    nom::RenderWindow window[MAXIMUM_WINDOWS];

    nom::Point2i window_size[MAXIMUM_WINDOWS];

    /// Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    /// \brief Image resource for our button widget.
    nom::Texture button_bg[3];

    // Our resources for our UI widgets
    nom::Font bitmap_font;
    nom::Font bitmap_small_font;
    nom::Font truetype_font;
    nom::Font truetype_font2;
    nom::SpriteBatch menu_elements;

    nom::Point2i gui_window_pos[GUI_WINDOWS];
    nom::Size2i gui_window_size[GUI_WINDOWS];
    nom::UIWidget::raw_ptr gui_window[GUI_WINDOWS];

    nom::Button* button_ex0;
    nom::Button* button_ex1;
    nom::BitmapButton* button_ex2;

    nom::ListBox* listbox_ex0;
    nom::ListBox* listbox_ex1;

    /// \brief The widget for ::create_listbox_ex2 helper method is necessarily
    /// declared as a class-scope variable so that we can update selections
    /// upon the appropriate key press -- see also, App::listbox_ex2_key_selection.
    nom::ListBox* listbox_ex2;

    /// \brief The widget for ::create_qbox_ex0 helper method is necessarily
    /// declared as a class-scope variable so that we can update selections
    /// upon the appropriate event action (i.e.: mouse wheel).
    nom::QuestionDialogBox* qbox_ex0;

    /// \brief Our colors used in gradient fills; used as a background for
    /// widget objects.
    nom::Color4iColors g_colors[2];

    nom::UIBoxLayout* listbox_layout;
    nom::UIBoxLayout* button_layout;
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
