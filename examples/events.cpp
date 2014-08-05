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

// Pubic nomlib interface headers
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// \brief  Relative file path name of our resource example
const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";

/// \brief Name of our application.
const std::string APP_NAME = "Input Mapping";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 768;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 448;

/// \brief Maximum number of active windows we will attempt to spawn in this
/// example.
const nom::int32 MAXIMUM_WINDOWS = 2;

const nom::int32 USER_EVENT_DEBUG = 0;
/// \brief Usage example
/// \remarks For unit testing: ensure that library is compiled with
/// the appropriate defines enabled within EventHandler.hpp.
class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] )
    {
      NOM_LOG_TRACE( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if( nom::init( argc, argv ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize nomlib." );
        exit( NOM_EXIT_FAILURE );
      }

      atexit( nom::quit );
    } // end App

    ~App( void )
    {
      NOM_LOG_TRACE( NOM );
    } // end ~App

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        if ( this->window[idx].create( APP_NAME, ( WINDOW_WIDTH / 2 ), WINDOW_HEIGHT, window_flags ) == false )
        {
          return false;
        }

        this->window[idx].set_position( 0 + ( WINDOW_WIDTH / 2 ) * idx, ( WINDOW_HEIGHT / 2 ) );

        if( this->window[idx].set_window_icon( RESOURCE_ICON ) == false )
        {
          nom::DialogMessageBox( APP_NAME, "ERROR: Could not load window icon: " + RESOURCE_ICON );
          return false;
        }

        // Scale window contents up by the new width & height
        // this->window[idx].set_logical_size( this->window[idx].size(), this->window[idx].size() );
      }

      // Start out execution with both windows minimized.
      nom::EventDispatcher sender;
      nom::Event user_event;

      nom::EventCallback delegate1( [&] ( const nom::Event& evt ) { this->minimize( evt ); } );
      nom::EventCallback delegate2( [&] ( const nom::Event& evt ) { this->restore( evt ); } );

      user_event.type = SDL_USEREVENT;
      user_event.timestamp = ticks();
      user_event.user.code = USER_EVENT_DEBUG;
      user_event.user.data1 = nullptr;
      user_event.user.data2 = NOM_SCAST( nom::EventCallback*, &delegate1 );
      user_event.user.window_id = 0;
      // sender.dispatch( user_event );

      user_event.type = SDL_USEREVENT;
      user_event.timestamp = ticks();
      user_event.user.code = USER_EVENT_DEBUG;
      user_event.user.data1 = nullptr;
      user_event.user.data2 = NOM_SCAST( nom::EventCallback*, &delegate2 );
      user_event.user.window_id = 0;
      // sender.dispatch( user_event );

      // State 0 is mouse button input mapping
      // State 1 is keyboard input mapping
      // State 2 is joystick button input mapping
      // State 3 is mouse wheel input mapping
      // State 4 is joystick axis input mapping; note that this implementation
      // is broken (not fully implemented).
      // State 5 is keyboard input mapping with repeating key active
      nom::InputActionMapper state0, state1, state2, state3, state4, state5;

      nom::EventCallback quit_app( [&] ( const nom::Event& evt )
        {
            this->on_app_quit( evt );
        }
      );

      // Mouse button mappings

      state0.insert( "minimize_window_0", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT ), nom::EventCallback( [&] ( const nom::Event& evt ) { this->minimize( evt ); } ) );
      state0.insert( "restore_window_0", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_RIGHT ), nom::EventCallback( [&] ( const nom::Event& evt ) { this->restore( evt ); } ) );
      state0.insert (
                      "quit_app",
                      nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, 3 ),
                      quit_app
                    );

      NOM_CONNECT_INPUT_MAPPING( state1, "minimize_window_0", nom::KeyboardAction( SDL_KEYDOWN, SDLK_1 ), minimize, evt );

      // NOTE: The following keyboard action will have its modifier key reset to
      // zero (0) as a side-effect of minimizing the window, ergo you will not be
      // able to continue holding down LCTRL after the first time in order to
      // re-execute the restoration of said window. This is NOT a bug within the
      // the input mapper.
      NOM_CONNECT_INPUT_MAPPING( state1, "restore_window_0", nom::KeyboardAction( SDL_KEYDOWN, SDLK_2, KMOD_LCTRL ), restore, evt );

      state1.insert (
                      "quit_app",
                      nom::KeyboardAction( SDL_KEYDOWN, SDLK_1, KMOD_LCTRL ),
                      quit_app
                    );

      // Joystick button mappings

      state2.insert (
                      "minimize_window_0",
                      nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::L1 ),
                      nom::EventCallback( [&] ( const nom::Event& evt ) { this->minimize( evt ); } )
                     );

      state2.insert (
                      "restore_window_0",
                      nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::R1 ),
                      nom::EventCallback( [&] ( const nom::Event& evt ) { this->restore( evt ); } )
                    );

      // Mouse wheel mappings

      // Wheel is going upward
      nom::MouseWheelAction mouse_wheel (
                                          SDL_MOUSEWHEEL,
                                          nom::MouseWheelAction::AXIS_Y,
                                          nom::MouseWheelAction::UP
                                        );

      state3.insert( "color_fill_1", mouse_wheel, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 0 ); } ) );

      // Wheel is going downward
      mouse_wheel = nom::MouseWheelAction (
                                            SDL_MOUSEWHEEL,
                                            nom::MouseWheelAction::AXIS_Y,
                                            nom::MouseWheelAction::DOWN
                                          );

      state3.insert( "color_fill_1", mouse_wheel, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 1 ); } ) );

      // Wheel is going leftward
      mouse_wheel = nom::MouseWheelAction (
                                            SDL_MOUSEWHEEL,
                                            nom::MouseWheelAction::AXIS_X,
                                            nom::MouseWheelAction::LEFT
                                          );

      state3.insert( "color_fill_1", mouse_wheel, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 2 ); } ) );

      // Wheel is going rightward
      mouse_wheel = nom::MouseWheelAction (
                                            SDL_MOUSEWHEEL,
                                            nom::MouseWheelAction::AXIS_X,
                                            nom::MouseWheelAction::RIGHT
                                          );

      state3.insert( "color_fill_1", mouse_wheel, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 3 ); } ) );

      nom::InputAction jaxis;

      // Joystick axis is going upward
      jaxis = nom::JoystickAxisAction ( 0, SDL_JOYAXISMOTION, 0, -1 );

      state4.insert( "color_fill_1", jaxis, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 0 ); } ) );

      // Joystick axis is going downward
      jaxis = nom::JoystickAxisAction( 0, SDL_JOYAXISMOTION, 0, 1 );

      state4.insert( "color_fill_1", jaxis, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 1 ); } ) );

      // Joystick axis is going leftward
      jaxis = nom::JoystickAxisAction( 0, SDL_JOYAXISMOTION, 1, -1 );

      state4.insert( "color_fill_1", jaxis, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 2 ); } ) );

      // Joystick axis is going rightward
      jaxis = nom::JoystickAxisAction( 0, SDL_JOYAXISMOTION, 1, 1 );

      state4.insert( "color_fill_1", jaxis, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 3 ); } ) );

      nom::InputAction kb_repeat;

      // Keyboard action should only trigger when the key symbols 4 and the
      // Command (OS X) or Windows modifier key is repeating (pressed down for
      // at least ~0.5s).
      kb_repeat = nom::KeyboardAction( SDL_KEYDOWN, SDLK_3, KMOD_LGUI, 1 );
      state5.insert( "color_fill_1", kb_repeat, nom::EventCallback( [&] ( const nom::Event& evt ) { this->color_fill( evt, 3 ); } ) );

      // Mouse button input mapping
      this->input_mapper.insert( "state0", state0, true );

      // Keyboard input mapping
      this->input_mapper.insert( "state1", state1, true );

      // Joystick Button input mapping
      this->input_mapper.insert( "state2", state2, true );

      // Mouse wheel input mapping
      this->input_mapper.insert( "state3", state3, true );

      // Joystick Axis input mapping
      this->input_mapper.insert( "state4", state4, true );

      // Keyboard repeating press mappings
      this->input_mapper.insert( "state5", state5, true );

      // this->input_mapper.clear();

      // this->input_mapper.disable( "state0" );
      // this->input_mapper.disable( "state1" );
      // this->input_mapper.disable( "state2" );
      // this->input_mapper.disable( "state3" );
      // this->input_mapper.disable( "state4" );
      // this->input_mapper.disable( "state5" );

      // this->input_mapper.activate_only( "state3" );

      if( this->input_mapper.active( "state1" ) )
      {
        // nom::DialogMessageBox( APP_NAME, "ERROR: Input context state1 is active." );
        // return false;
      }

      // this->input_mapper.activate_only( "state1" );
      // this->input_mapper.dump();

      return true;
    } // end on_init

    void minimize( const nom::Event& ev )
    {
      ev.dump();

      if( ev.type == SDL_KEYDOWN )
      {
        ev.key.dump();
      }
      else if( ev.type == SDL_MOUSEBUTTONDOWN )
      {
        ev.mouse.dump();
      }
      else if( ev.type == SDL_JOYBUTTONDOWN )
      {
        ev.jbutton.dump();
      }

      NOM_DUMP("MINIMIZE WINDOW 0");
      this->window[0].minimize_window();
    }

    void restore( const nom::Event& ev )
    {
      ev.dump();

      if( ev.type == SDL_KEYDOWN )
      {
        ev.key.dump();
      }
      else if( ev.type == SDL_MOUSEBUTTONDOWN )
      {
        ev.mouse.dump();
      }
      else if( ev.type == SDL_JOYBUTTONDOWN )
      {
        ev.jbutton.dump();
      }

      NOM_DUMP("RESTORE WINDOW 0");
      this->window[0].restore_window();
    }

    void color_fill( const nom::Event& ev, nom::uint8 dir )
    {
      ev.dump();

      if( ev.type == SDL_MOUSEWHEEL )
      {
        ev.wheel.dump();
      }
      else if( ev.type == SDL_JOYAXISMOTION )
      {
        ev.jaxis.dump();
      }

      // NOM_LOG_TRACE( NOM );

      switch( dir )
      {
        default:
        {
          NOM_DUMP( "INVALID" );
          break;
        }

        case 0:
        {
          NOM_DUMP( "WHEEL UP" );
          this->window[1].fill( nom::Color4i::Magenta );
          this->window[1].update();
          break;
        }

        case 1:
        {
          NOM_DUMP( "WHEEL DOWN" );
          this->window[1].fill( nom::Color4i::Gray );
          this->window[1].update();
          break;
        }

        case 2:
        {
          NOM_DUMP( "WHEEL LEFT" );
          this->window[1].fill( nom::Color4i::Orange );
          this->window[1].update();
          break;
        }

        case 3:
        {
          NOM_DUMP( "WHEEL RIGHT" );
          this->window[1].fill( nom::Color4i::Yellow );
          this->window[1].update();
          break;
        }
      } // end switch dir
    } // end color_fill

    nom::sint Run( void )
    {
      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        this->update[idx].start();
        this->fps[idx].start();
      }

      // Paint the window(s) only once ... let event callbacks do the rest!
      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        this->window[idx].fill( nom::Color4i::SkyBlue );
        this->window[idx].update();
      }

      // Test the simulation of an input event
      nom::Event key_ev;
      key_ev.type = SDL_KEYDOWN;
      key_ev.timestamp = nom::ticks();
      key_ev.key.sym = SDLK_1;
      key_ev.key.mod = KMOD_NONE;
      key_ev.key.repeat = 0;
      this->push_event( key_ev );

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while( this->poll_event( this->event ) )
        {
          this->on_event( this->event );

          this->input_mapper.on_event( this->event );
        }

        for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
        {
          // this->window[idx].update();
          this->fps[idx].update();

          // Refresh the frames per second at 1 second intervals
          if ( this->update[idx].ticks() > 1000 )
          {
            if ( this->show_fps() == true )
            {
              this->window[idx].set_window_title( APP_NAME + " - " + this->fps[idx].asString() + ' ' + "fps" );
            }
            else
            {
              this->window[idx].set_window_title( APP_NAME + " [" + std::to_string(this->window[idx].window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window[idx].window_display_id() ) );
            }

            this->update[idx].restart();
          } // end refresh cycle
        } // end for MAXIMUM_WINDOWS update loop
      } // end while SDLApp::running() is true

      return NOM_EXIT_SUCCESS;
    } // end Run()

  private:
    /// \brief Event handler for user-defined events.
    ///
    /// \remarks Implements nom::EventHandler::on_user_event
    void on_user_event( const nom::Event& ev )
    {
      // A call is made here to the virtual method being re-implemented here in
      // order to catch debugging output with debug builds compiled in; see
      // EventHandler.hpp.
      // Input::on_user_event( ev );

      if( ev.user.code == USER_EVENT_DEBUG )
      {
        ev.dump();
        ev.user.dump();

        nom::EventCallback* delegate = ev.user.get_callback();

        if( delegate != nullptr )
        {
          // FIXME: I get a segfault here when we try to execute the callback.
          #if ! defined( NOM_PLATFORM_WINDOWS )
            // FIXME: We get a segfault under OS X (and presumed Windows, too)
            // when we add more than one kb_repeat input action. No idea why!
            // delegate->operator()( ev );
          #endif
        }
      }
    }

  private:
    nom::Event event;

    /// \brief Window handles
    ///
    /// \todo Use std::vector?
    nom::RenderWindow window[MAXIMUM_WINDOWS];

    /// \brief Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// \brief Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    nom::InputStateMapper input_mapper;
}; // end class App

nom::sint main( nom::int32 argc, char* argv[] )
{
  App app ( argc, argv );

  if ( app.on_init() == false )
  {
    nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world!
}
