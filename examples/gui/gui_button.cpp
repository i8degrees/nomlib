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
#include <nomlib/gui.hpp>

using namespace nom;

const nom::Path p;

/// \brief File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources" + p.native();

/// \brief Name of our application.
const std::string APP_NAME = "nomlib's GUI | Button Widgets";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 640;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 480;

const std::string RESOURCE_ICON = APP_RESOURCES_DIR + "icon.png";

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

const std::string RESOURCE_BUTTON_IMAGE = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "button.png";
const std::string RESOURCE_AQUA_BUTTON_IMAGE_0 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_0.png";
const std::string RESOURCE_AQUA_BUTTON_IMAGE_1 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_1.png";

class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] ) :
      button0{ nullptr },
      button1{ nullptr },
      button2{ nullptr }
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
    }

    ~App( void )
    {
      NOM_LOG_TRACE ( NOM );

      delete this->gui_window[0];
      delete this->gui_window[1];

      this->gui_window[0] = nullptr;
      this->gui_window[1] = nullptr;
    }

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

      if ( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if ( this->window.create( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        return false;
      }

      if ( this->window.set_window_icon ( RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
        return false;
      }

      // Scale window contents up by the new width & height
      this->window.set_logical_size ( WINDOW_WIDTH, WINDOW_HEIGHT );

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

      // Top-level window (relative to global "screen" coordinates):
      this->gui_window[0] = new nom::UIWidget( nullptr, -1, nom::Point2i( 2, 2 ), nom::Size2i( WINDOW_WIDTH - 4, WINDOW_HEIGHT - 4 ) );
      this->gui_window[0]->set_name( "Root" );
      this->gui_window[0]->set_title( this->gui_window[0]->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // top-level window
      this->gui_window[0]->set_decorator( new nom::MinimalDecorator() );

      // Window-scope mouse button click events
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->gui_window[0]->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // Layout container initialization:

      this->gui_window[1] = new nom::UIWidget( nullptr, -1, Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 ) );
      this->gui_window[1]->set_name( "Layout" );
      this->gui_window[1]->set_title( this->gui_window[1]->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->gui_window[1]->set_decorator( new nom::MinimalDecorator() );

      // Our widgets to be used in the layout:

      // Note that each button's size (other than the bitmap button) will be
      // too small to fit the text used (which was intentional), but with layout
      // management, the size dimensions will be calculated for us automatically
      // (meaning that the size dimensions used will be from the widget's
      // size_hint method, when the size policy is set to Preferred).
      this->button0 = this->create_button( this->gui_window[1], Point2i::null, Size2i(50,25), "button0", "Click me!" );
      this->button0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->button0->set_decorator( new nom::FinalFantasyDecorator() );
      NOM_CONNECT_UIWIDGET_EVENT( this->button0, nom::UIEvent::MOUSE_DOWN, this->button0_on_click );
      NOM_CONNECT_UIWIDGET_EVENT( this->button0, nom::UIEvent::MOUSE_UP, this->button0_on_click );

      this->button1 = this->create_button( this->gui_window[1], Point2i::null, Size2i(50,25), "button1", "button1" );
      this->button1->set_decorator( new nom::MinimalDecorator() );
      NOM_CONNECT_UIWIDGET_EVENT( this->button1, nom::UIEvent::MOUSE_DOWN, this->button1_on_click );
      NOM_CONNECT_UIWIDGET_EVENT( this->button1, nom::UIEvent::MOUSE_UP, this->button1_on_click );

      nom::UIStyle::shared_ptr custom_style = nullptr;
      custom_style.reset( new nom::UIStyle() );

      custom_style->set_font_color( nom::Color4i::Black );
      custom_style->set_font_size( 13 );
      custom_style->set_font_style( nom::Text::Style::Bold );

      this->button2 = this->create_bitmap_button( this->gui_window[1], Point2i::null, Size2i(102,25), "bitmap_button", "Hello", this->button_bg[1], custom_style );
      this->button2->set_pressed_bitmap( this->button_bg[2] );
      NOM_CONNECT_UIWIDGET_EVENT( this->button2, nom::UIEvent::MOUSE_DOWN, this->on_click );
      NOM_CONNECT_UIWIDGET_EVENT( this->button2, nom::UIEvent::MOUSE_UP, this->on_click );

      // Explicitly positioned:
// this->gui_window[0]->insert_child( this->create_bitmap_button( this->gui_window[0], Point2i(102,400), Size2i(102,25), "expliclt_bitmap_button", "HelloB", this->button_bg[1], custom_style ) );

      this->gui_window[1]->insert_child( this->button0 );
      this->gui_window[1]->insert_child( this->button1 );
      this->gui_window[1]->insert_child( this->button2 );

      // Button layout
      this->layout = new nom::UIHBoxLayout();

      if( this->button0 != nullptr )
      {
        // this->button0->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );
      }

      this->layout->append_spacer( 8 );
      this->layout->append_widget( this->button0 );
      this->layout->append_spacer( 8 );
      this->layout->append_widget( this->button1 );
      this->layout->append_spacer( 40 );
      this->layout->append_widget( this->button2 );

      this->gui_window[1]->set_layout( this->layout );

      // Relative to parent widget
      this->layout->set_bounds( nom::IntRect( nom::Point2i( 12, 25 ), this->gui_window[1]->size() ) );

      // Auto-generated name
      // NOM_ASSERT( this->gui_window[1]->find_child( "bitmap_button" ) != nullptr );

      NOM_ASSERT( this->gui_window[0]->id() == 1 );
      NOM_ASSERT( this->gui_window[0]->name() == "Root" );
      NOM_ASSERT( this->gui_window[0]->title() == "Root" );

      NOM_ASSERT( this->gui_window[1]->id() == 2 );

      NOM_ASSERT( this->gui_window[1]->name() == "Layout" );
      NOM_ASSERT( this->gui_window[1]->title() == "Layout" );

      nom::UIWidget* widget = nullptr;

      // button0
      widget = this->gui_window[1]->find_child( 6 );

      if( widget != nullptr )
      {
        NOM_ASSERT( widget->is_window() == false );
        // NOM_ASSERT( widget->name() == "button0" );
      }

      NOM_ASSERT( this->gui_window[0]->is_window() == true );
      NOM_ASSERT( this->gui_window[1]->is_window() == true );

      return true;
    }

    /// \brief Test the creation of a button widget.
    nom::Button::raw_ptr create_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label )
    {
      nom::Button::raw_ptr button = new nom::Button( window, -1, pos, size );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );
      NOM_ASSERT( button->size() == size );

      return button;
    }

    /// \brief Test the creation of a bitmap button widget.
    nom::BitmapButton::raw_ptr create_bitmap_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label, const Texture& tex, const UIStyle::shared_ptr style )
    {
      // TODO: Create separate examples (including a nullptr test):
      // button = new nom::BitmapButton( window, pos, size, nullptr );
      // button = new nom::BitmapButton( this->gui_window[0], -1, pos, size, &this->button_bg[0] );
      // button = new nom::BitmapButton( this->gui_window[0], -1, pos, size, &this->button_bg[0] );

      nom::BitmapButton::raw_ptr button = new nom::BitmapButton( window, -1, pos, size, tex );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      // FIXME: This has to be set before the text label; this is the same
      // problem as we had with nom::Text before relocating everything to
      // ::update.
      button->set_style( style );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );
      NOM_ASSERT( button->size() == size );

      return button;
    }

    void on_click( const UIWidgetEvent& ev ) const
    {
      // Event evt = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

    void button0_on_click( const UIWidgetEvent& ev ) const
    {
      // Event evt = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      NOM_ASSERT( this->button0 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button0->set_label( "Click me!" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button0->set_label( "Release me!" );
      }
    }

    void button1_on_click( const UIWidgetEvent& ev ) const
    {
      // Event evt = ev.event();

      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      NOM_ASSERT( this->button1 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button1->set_label( "button1" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button1->set_label( "Yeah buddy!" );
      }
    }

    nom::int32 Run( void )
    {
      this->update.start();
      this->fps.start();

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while( this->poll_event( this->event ) )
        {
          this->on_event( this->event );

          this->gui_window[0]->process_event( this->event );
          this->gui_window[1]->process_event( this->event );
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

        this->gui_window[0]->update();
        this->gui_window[1]->update();

        this->window.fill( nom::Color4i::SkyBlue );

        this->gui_window[0]->draw( this->window );
        this->gui_window[1]->draw( this->window );

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

        case SDLK_F1:
        {
          if( this->window.save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
          {
            nom::DialogMessageBox( APP_NAME, "Could not save screenshot");
          } // end save_screenshot err check

          break;
        }

        case SDLK_f:
        {
          this->window.toggle_fullscreen();
          break;
        } // end SDLK_f

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

    /// \brief Image resource for our button widget.
    nom::Texture button_bg[3];

    nom::UIWidget::raw_ptr gui_window[2];

    nom::Button* button0;
    nom::Button* button1;
    nom::BitmapButton* button2;

    nom::UIBoxLayout* layout;
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
