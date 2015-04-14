/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include <nomlib/core.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

using namespace nom;

// Enable base API sanity checks for nom::EventHandler
#define NOM_TEST_EVENT_HANDLER_API

class App: public nom::SDLApp
{
  public:
    enum InputDirection: uint8
    {
      LEFT_DIRECTION,
      RIGHT_DIRECTION,
      UP_DIRECTION,
      DOWN_DIRECTION,
    };

    App()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

      // ...Initialize the input mapper callbacks...

      this->quit_app = nom::event_callback( [=](const nom::Event& evt) {
        this->on_app_quit(evt);
      });

      this->minimize_window = nom::event_callback ( [=](const nom::Event& evt) {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Minimizing window 0." );
        this->window[0].minimize_window();
      });

      this->restore_window = nom::event_callback( [=](const nom::Event& evt) {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Restoring window 0." );
        this->window[0].restore_window();
      });

      this->up_action = nom::event_callback( [=](const nom::Event& evt) {
        this->color_fill(evt, UP_DIRECTION);
      });

      this->down_action = nom::event_callback( [=](const nom::Event& evt) {
        this->color_fill(evt, DOWN_DIRECTION);
      });

      this->left_action = nom::event_callback( [=](const nom::Event& evt) {
        this->color_fill(evt, LEFT_DIRECTION);
      });

      this->right_action = nom::event_callback( [=](const nom::Event& evt) {
        this->color_fill(evt, RIGHT_DIRECTION);
      });
    }

    ~App()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
    }

    bool on_init()
    {
      uint32 window_flags = 0;

      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx ) {

        if( this->window[idx].create( APP_NAME,
            WINDOW_RESOLUTION, window_flags) == false )
        {
          return false;
        }

        auto window_pos = Point2i::zero;
        window_pos.x = WINDOW_RESOLUTION.w * idx;
        window_pos.y = 0;
        this->window[idx].set_position(window_pos);
      }

      nom::InputActionMapper mouse_buttons, keyboard, mouse_wheel, kb_repeat;
      nom::InputAction action;

      // ...Mouse button mappings...

      mouse_buttons.insert( "minimize_window_0",
                            nom::MouseButtonAction(nom::LEFT_MOUSE_BUTTON),
                            this->minimize_window );

      mouse_buttons.insert( "restore_window_0",
                            nom::MouseButtonAction(nom::RIGHT_MOUSE_BUTTON),
                            this->restore_window );

      mouse_buttons.insert( "quit_app",
                            nom::MouseButtonAction(nom::LEFT_MOUSE_BUTTON, 3),
                            this->quit_app );

      // ...Keyboard mappings...

      NOM_CONNECT_INPUT_MAPPING(  keyboard, "minimize_window_0",
                                  nom::KeyboardAction(SDLK_1),
                                  this->minimize_window, evt );

      // NOTE: The following keyboard action will have its modifier key reset to
      // zero (0) as a side-effect of minimizing the window, ergo you will not be
      // able to continue holding down LCTRL after the first time in order to
      // re-execute the restoration of said window. This is NOT a bug within the
      // the input mapper.
      NOM_CONNECT_INPUT_MAPPING(  keyboard, "restore_window_0",
                                  nom::KeyboardAction(SDLK_2, KMOD_LCTRL),
                                  this->restore_window, evt );

      keyboard.insert(  "quit_app",
                        nom::KeyboardAction(SDLK_1, KMOD_LCTRL),
                        this->quit_app );

      // ...Mouse wheel mappings...

      action = nom::MouseWheelAction(nom::MOUSE_WHEEL_UP);
      mouse_wheel.insert("color_fill_up", action, this->up_action);

      action = nom::MouseWheelAction(nom::MOUSE_WHEEL_DOWN);
      mouse_wheel.insert("color_fill_down", action, this->down_action);

      action = nom::MouseWheelAction(nom::MOUSE_WHEEL_LEFT);
      mouse_wheel.insert("color_fill_left", action, this->left_action);

      action = nom::MouseWheelAction(nom::MOUSE_WHEEL_RIGHT);
      mouse_wheel.insert("color_fill_right", action, this->right_action);

      // ...Keyboard repeat action...

      // Keyboard action should only trigger when the key symbols 4 and the
      // Command (OS X) or Windows modifier key is repeating (pressed down for
      // at least ~0.5s).
      action = nom::KeyboardAction(SDLK_3, KMOD_LGUI, 1);
      kb_repeat.insert("color_fill_1", action, this->right_action);

      // ...Installation of input mappings...

      this->input_mapper.insert("mouse_buttons", mouse_buttons, true);
      this->input_mapper.insert("keyboard", keyboard, true);
      this->input_mapper.insert("mouse_wheel", mouse_wheel, true);
      this->input_mapper.insert("kb_repeat", kb_repeat, true);

      // ...Isolated tests...
#if 0
      this->input_mapper.disable("mouse_buttons");
      this->input_mapper.disable("keyboard");
      this->input_mapper.disable("mouse_wheel");
      this->input_mapper.disable("kb_repeat");
#endif

#if 0
      this->input_mapper.activate_only("keyboard");
#endif

      if( this->input_mapper.active("keyboard") == false ) {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Input context 'keyboard' is not active." );
        return false;
      }

#if 0
      this->input_mapper.clear();
#endif

#if 0
      // Debugging diagnostics
      this->input_mapper.dump();
#endif

#if defined(NOM_TEST_EVENT_HANDLER_API)
      this->test_flush_events();
      this->test_event_watch();
#endif

      SDLApp::set_event_handler(this->evt_handler);

      this->input_mapper.set_event_handler(this->evt_handler);

#if defined(NOM_TEST_EVENT_HANDLER_API)
      this->test_simulated_events();
#endif

      return true;
    }

    int Run()
    {
      // Clear the render buffer
      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx ) {
        this->window[idx].fill(nom::Color4i::SkyBlue);
        this->window[idx].update();
      }

      // 1. Events
      // 2. Logic
      // 3. Render
      while( this->running() == true ) {

        nom::Event evt;
        while( evt_handler.poll_event(evt) == true ) {
          // NOTE: Pending events will be handled by the event listeners that
          // were given an EventHandler object via ::set_event_handler.
          //
          // Additional event processing done in here is still OK, too.
        } // end inner while
      } // end outer while

      // ...Clean up ::on_user_event test data...
      delete NOM_SCAST(nom::event_callback*, this->user_data1);
      delete NOM_SCAST(nom::event_callback*, this->user_data2);
      this->user_data1 = nullptr;
      this->user_data2 = nullptr;

#if defined(NOM_TEST_EVENT_HANDLER_API)
      this->cleanup_event_watch_test();
#endif

      return NOM_EXIT_SUCCESS;
    }

  private:
    const std::string APP_NAME = "InputMapper and events handling";
    const Size2i WINDOW_RESOLUTION = Size2i(640/2, 480);
    static const nom::int32 MAXIMUM_WINDOWS = 2;
    nom::RenderWindow window[MAXIMUM_WINDOWS];

    // Event handling
    EventHandler evt_handler;
    nom::InputStateMapper input_mapper;

    nom::event_callback quit_app;
    nom::event_callback minimize_window;
    nom::event_callback restore_window;
    nom::event_callback up_action;
    nom::event_callback down_action;
    nom::event_callback left_action;
    nom::event_callback right_action;

    nom::event_filter test_watch_callback;
    nom::event_callback* user_data1 = nullptr;
    nom::event_callback* user_data2 = nullptr;

    void color_fill(const nom::Event& ev, InputDirection dir)
    {
      switch(dir)
      {
        default:
        {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "INVALID INPUT");
          NOM_ASSERT_INVALID_PATH();
          break;
        }

        case UP_DIRECTION:
        {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "UP INPUT");
          this->window[1].fill(nom::Color4i::Magenta);
          this->window[1].update();
          break;
        }

        case DOWN_DIRECTION:
        {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "DOWN INPUT");
          this->window[1].fill(nom::Color4i::Gray);
          this->window[1].update();
          break;
        }

        case LEFT_DIRECTION:
        {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "LEFT INPUT");
          this->window[1].fill(nom::Color4i::Orange);
          this->window[1].update();
          break;
        }

        case RIGHT_DIRECTION:
        {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "RIGHT INPUT");
          this->window[1].fill(nom::Color4i::Yellow);
          this->window[1].update();
          break;
        }
      } // end switch dir
    }

    /// \brief The default event handler for user-defined events.
    ///
    /// \remarks The default implementation in SDLApp::on_user_event does
    /// nothing with these events.
    void on_user_event(const nom::Event& ev) override
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_EVENT, NOM_LOG_PRIORITY_INFO);

      nom::event_callback* data1 =
        NOM_SCAST(nom::event_callback*, ev.user.data1);
      nom::event_callback* data2 =
        NOM_SCAST(nom::event_callback*, ev.user.data2);
      if( ev.type == Event::USER_EVENT && ev.user.code == 666 ) {

        if( data1 != nullptr ) {
          data1->operator()(ev);
        }

        if( data2 != nullptr ) {
          data2->operator()(ev);
        }
      }
    }

    /// \brief Test events flush API of EventHandler
    void test_flush_events()
    {
      NOM_ASSERT( this->evt_handler.joystick_event_type() ==
                  EventHandler::NO_EVENT_HANDLER );

      nom::Event test_flush;
      test_flush.type = Event::FIRST_EVENT;
      test_flush.timestamp = nom::ticks();
      this->evt_handler.push_event(test_flush);
      this->evt_handler.push_event(test_flush);
      NOM_ASSERT(this->evt_handler.num_events() == 2);

      this->evt_handler.flush_event(Event::FIRST_EVENT);
      NOM_ASSERT(this->evt_handler.num_events() == 1);

      this->evt_handler.flush_events();
      NOM_ASSERT(this->evt_handler.num_events() == 0);
    }

    /// \brief Test the addition and removal API in EventHandler for event
    /// watchers.
    void test_event_watch()
    {
      this->test_watch_callback =
        nom::event_filter([=](const Event& evt, void* data) {
          if( evt.type == Event::KEY_RELEASE && evt.key.sym == SDLK_2 &&
              evt.key.state == InputState::RELEASED )
          {
            if( data != nullptr ) {
              NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "data:",
                            NOM_SCAST(char*, data) );
            }
          }
      });

      this->evt_handler.append_event_watch( this->test_watch_callback,
                                            (char*)"hello, world!" );
      NOM_ASSERT(this->evt_handler.num_event_watchers() == 1);

      this->evt_handler.append_event_watch(nullptr, nullptr);
      NOM_ASSERT(this->evt_handler.num_event_watchers() == 1);

      this->evt_handler.remove_event_watch(nullptr);
      NOM_ASSERT(this->evt_handler.num_event_watchers() == 1);

      // NOTE: This should trigger the callback assigned to the event watcher;
      // see ::on_watch_callback
      nom::Event key_release_ev;
      key_release_ev = nom::create_key_release(SDLK_2, KMOD_NONE, 0);
      this->evt_handler.push_event(key_release_ev);
    }

    void cleanup_event_watch_test()
    {
      this->evt_handler.remove_event_watch(this->test_watch_callback);

      // NOTE: Only the event watchers from SDLApp && input mapper should exist
      // at this point!
      NOM_ASSERT(this->evt_handler.num_event_watchers() == 2);
    }

    /// \brief Test the simulation of input events.
    void test_simulated_events()
    {
      // ...key press...

      nom::Event key_ev;
      key_ev = nom::create_key_press(SDLK_1, KMOD_NONE, 0);
      evt_handler.push_event(key_ev);

      // ...mouse button click...

      nom::Event mbutton_ev;
      mbutton_ev =
        nom::create_mouse_button_click(nom::RIGHT_MOUSE_BUTTON, 1, 1);
      evt_handler.push_event(mbutton_ev);

      // ...user event...

      this->user_data1 = new nom::event_callback( [=](const nom::Event& evt) {
        if( evt.type == Event::USER_EVENT ) {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Event::USER_EVENT");
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "event code:", evt.user.code );
        }
      });

      this->user_data2 = new nom::event_callback( [=](const nom::Event& evt) {
        if( evt.type == Event::USER_EVENT ) {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Event::USER_EVENT");
        } else if( evt.type == Event::QUIT_EVENT ) {
          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Event::QUIT_EVENT");
        }
      });

      nom::Event user_ev =
        nom::create_user_event(666, this->user_data1, this->user_data2, 0);
      evt_handler.push_event(user_ev);

      // ...quit event...
#if 0
      nom::Event quit_ev =
        nom::create_quit_event(this->user_data1, this->user_data2);
      evt_handler.push_event(quit_ev);
#endif
    }
}; // end class App

int main(nom::int32 argc, char* argv[])
{
  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if( nom::init(argc, argv) == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize nomlib." );
    return NOM_EXIT_FAILURE;
  }

  atexit(nom::quit);

  // Show add and removal events
  nom::SDL2Logger::set_logging_priority(  NOM_LOG_CATEGORY_EVENT,
                                          NOM_LOG_PRIORITY_INFO );

  // Enable event handler queue debugging statistics
  nom::set_hint(NOM_EVENT_QUEUE_STATISTICS, "1");

  App app;

  if( app.on_init() == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world!
}
