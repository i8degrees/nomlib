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
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>

using namespace nom;

/// \brief The file used for resource path lookups for this example.
const std::string RES_FILE = "gamecontroller_events.json";

class App: public nom::SDLApp
{
  public:
    App()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

      // ...Initialize the input mapper callbacks...

      this->quit_app = nom::event_callback( [=](const nom::Event& evt) {
        this->on_app_quit(evt);
      });

      this->init_db_bindings = nom::event_callback( [=](const nom::Event& evt) {
        this->initialize_game_controller_db(this->db_filename);
      });

      this->x_axis_action = nom::event_callback( [=](const nom::Event& evt) {

        int32 axis = evt.caxis.axis;
        int32 axis_value = evt.caxis.value;
        Point2i rect_pos;

        if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ||
            axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE )
        {
          // ...proportionate transform of the joystick axis && screen-space
          // coordinate systems

          rect_pos.x = axis_value + 32768;

          // Account for circular dead zone
          if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
            rect_pos.x -= nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
          } else if( axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
            rect_pos.x += nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
          }

          rect_pos.x *= WINDOW_RESOLUTION.w;
          rect_pos.x /= 65535;

          if( rect_pos.x < 0 ) {
            rect_pos.x = 0;
          } else if( rect_pos.x > WINDOW_RESOLUTION.w - AXIS_RECT_DIMS.w ) {
            rect_pos.x = WINDOW_RESOLUTION.w - AXIS_RECT_DIMS.w;
          }

          if( axis < 2 ) {
            this->axis_rectangle_pos[0].x = rect_pos.x;
          } else {
            this->axis_rectangle_pos[1].x = rect_pos.x;
          }
        } else {
          // Resting position (we're in the dead zone)
          if( axis < 2 ) {
            this->axis_rectangle_pos[0].x =
              (WINDOW_RESOLUTION.w - AXIS_RECT_DIMS.w) / 2;
          } else {
            this->axis_rectangle_pos[1].x =
              (WINDOW_RESOLUTION.w - AXIS_RECT_DIMS.w) / 2;
          }
        }
      });

      this->y_axis_action = nom::event_callback( [=](const nom::Event& evt) {

        int32 axis = evt.caxis.axis;
        int32 axis_value = evt.caxis.value;
        Point2i rect_pos;

        if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ||
            axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE )
        {
          // ...proportionate transform of the joystick axis && screen-space
          // coordinate systems

          rect_pos.y = axis_value + 32768;

          // Account for circular dead zone
          if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
            rect_pos.y -= nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
          } else if( axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
            rect_pos.y += nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
          }

          rect_pos.y *= WINDOW_RESOLUTION.h;
          rect_pos.y /= 65535;

          if( rect_pos.y < 0 ) {
            rect_pos.y = 0;
          } else if( rect_pos.y > WINDOW_RESOLUTION.h - AXIS_RECT_DIMS.h ) {
            rect_pos.y = WINDOW_RESOLUTION.h - AXIS_RECT_DIMS.h;
          }

          if( axis < 2 ) {
            this->axis_rectangle_pos[0].y = rect_pos.y;
          } else {
            this->axis_rectangle_pos[1].y = rect_pos.y;
          }
        } else {
          // Resting position (we're in the dead zone)
          if( axis < 2 ) {
            this->axis_rectangle_pos[0].y =
              (WINDOW_RESOLUTION.h - AXIS_RECT_DIMS.h) / 2;
          } else {
            this->axis_rectangle_pos[1].y =
              (WINDOW_RESOLUTION.h - AXIS_RECT_DIMS.h) / 2;
          }
        }
      });

      this->button_action = nom::event_callback( [=](const nom::Event& evt) {

        int32 button = evt.cbutton.button;
        int32 button_state = evt.cbutton.state;
        Point2i rect_pos;

        if( button_state == InputState::PRESSED ) {
          auto spacing = BUTTON_RECT_DIMS.w * 2;
          auto origin = BUTTON_RECT_DIMS.w;
          rect_pos.x =
            origin + (spacing * button);
          rect_pos.y = WINDOW_RESOLUTION.h - BUTTON_RECT_DIMS.h * 2;
        } else {
          // Render off-screen by default
          rect_pos.x = -WINDOW_RESOLUTION.w;
          rect_pos.y = -WINDOW_RESOLUTION.h;
        }

        if( button < MAX_BUTTONS ) {
          this->button_rectangle_pos[button] = rect_pos;
        }
      });
    }

    ~App()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
    }

    bool on_init()
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      if( this->res.load_file(RES_FILE, "resources") == false ) {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Could not resolve the resource path from the file:",
                      RES_FILE );
        return false;
      }

      this->db_filename = res.path() + "gamecontrollerdb.txt";

      if( this->window.create( APP_NAME, WINDOW_RESOLUTION,
          window_flags ) == false )
      {
        return false;
      }

      NOM_ASSERT( this->evt_handler.joystick_event_type() ==
                  EventHandler::NO_EVENT_HANDLER );

      if( this->evt_handler.enable_game_controller_polling() == false ) {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Could not initialize game controller subsystem: ",
                      nom::error() );
        return false;
      }

      NOM_ASSERT( this->evt_handler.joystick_event_type() ==
                  EventHandler::GAME_CONTROLLER_EVENT_HANDLER );

      this->initialize_game_controller_db(this->db_filename);

      // NOTE: Ensure that closing the joystick device more than once does not
      // crash with a double-free memory violation from SDL_GameControllerClose
      {
        GameController jdev;
        for(  auto joystick_index = 0;
              joystick_index != Joystick::num_joysticks();
              ++joystick_index )
        {
          if( jdev.open(joystick_index) == true ) {
              jdev.close();
              jdev.close();
              jdev.close();
          }
        }
      }

      // ...Test remapping a couple game controller buttons...
#if 0
      {
        GameController jdev;
        if( jdev.open(0) == true ) {
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                       "The game controller's current mapping string is",
                       jdev.mapping_string() );

          // Flip the left and right shoulder buttons
          const std::string GAME_CONTROLLER_MAPPING_STR =
            "6d0400000000000016c2000000000000,Logitech F310 Gamepad (DInput),a:b1,b:b2,back:b8,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftshoulder:b5,leftstick:b10,lefttrigger:b6,leftx:a0,lefty:a1,rightshoulder:b4,rightstick:b11,righttrigger:b7,rightx:a2,righty:a3,start:b9,x:b0,y:b3,platform:Mac OS X,";
          int result = jdev.load_mapping_string(GAME_CONTROLLER_MAPPING_STR);
          // We always expect an existing mapping to be updated
          NOM_ASSERT(result == 0);

          jdev.close();
        }
      }
#endif

#if 0
      if( Joystick::num_joysticks() > 0 ) {
        std::string mapping_str;
        JoystickGUID dev_guid0 = Joystick::device_guid(0);
        mapping_str = GameController::mapping_string(dev_guid0);
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                      "The game controller's mapping string is",
                      mapping_str );
      }
#endif

      Joystick jdev;
      for(  auto joystick_index = 0;
            joystick_index != Joystick::num_joysticks();
            ++joystick_index )
      {
        if( jdev.open(joystick_index) == true ) {

          std::string guid_str = "0x" + jdev.device_guid_string();

          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Joystick",
                        joystick_index, ":", Joystick::name(joystick_index) );

          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "\taxes:", jdev.num_axes() );
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "\ttrack balls:", jdev.num_track_balls() );
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "\tbuttons:", jdev.num_buttons() );
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "\tPOV hats:", jdev.num_hats() );

          NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "\tguid:", guid_str);
        }
      }
      for(  auto joystick_index = 0;
            joystick_index != Joystick::num_joysticks();
            ++joystick_index )
      {
        if( GameController::compatible_joystick(joystick_index) == false ) {
          NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION, "Joystick",
                        joystick_index, ":", Joystick::name(joystick_index),
                        "is not supported by SDL's game controller interface!" );
        }
      }

      // ...Initialize of the visual joystick state positions...

      for( auto axis_idx = 0; axis_idx != MAX_AXES; ++axis_idx ) {
        this->axis_rectangle_pos[axis_idx] =
          nom::alignment_rect(  AXIS_RECT_DIMS, Point2i::zero,
                                WINDOW_RESOLUTION, Anchor::MiddleCenter );
      }

      for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {
        // Defaults to rendering off-screen
        this->button_rectangle_pos[button_idx].x = -WINDOW_RESOLUTION.w;
        this->button_rectangle_pos[button_idx].y = -WINDOW_RESOLUTION.h;
      }

      SDLApp::set_event_handler(this->evt_handler);

      this->input_mapper.set_event_handler(this->evt_handler);

      return true;
    }

    int Run()
    {
      this->window.fill(nom::Color4i::SkyBlue);

      // 1. Events
      // 2. Logic
      // 3. Render
      while( this->running() == true ) {

        nom::Event evt;
        while( evt_handler.poll_event(evt) == true ) {

          switch(evt.type)
          {
            default: break;

            case Event::JOYSTICK_ADDED:
            case Event::JOYSTICK_REMOVED:
            {
              NOM_ASSERT_INVALID_PATH();
            } break;

            case Event::GAME_CONTROLLER_ADDED:
            {
              // NOTE: Use the most recently added joystick for mapping input
              // bindings to
              auto device_index = evt.cdevice.id;
              this->on_game_controller_add(device_index);

              // IMPORTANT: We must have the actual hardware initialized before
              // simulating this event type!
              this->test_simulated_events();
            } break;

            case Event::GAME_CONTROLLER_REMOVED:
            {
              // NOTE: Upon removal, if we can, remap joystick input bindings
              // to whatever device that is still available, so we can still
              // control this application example with a joystick
              if( Joystick::num_joysticks() > 0 ) {
                this->on_game_controller_add(0);
              } else {
                NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                              "No joystick devices are available; exiting!" );
                this->on_app_quit(evt);
              }
            } break;
          }
        } // end inner while

        this->window.update();

        this->window.fill(nom::Color4i::SkyBlue);

        for( auto axis_idx = 0; axis_idx != MAX_AXES; ++axis_idx ) {
          this->render_rectangle( this->axis_rectangle_pos[axis_idx],
                                  AXIS_RECT_DIMS, Color4i::Green );
        }

        for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {
          this->render_rectangle( this->button_rectangle_pos[button_idx],
                                  BUTTON_RECT_DIMS, Color4i::Green );
        }

      } // end outer while

      return NOM_EXIT_SUCCESS;
    }

  private:
    nom::SearchPath res;
    std::string db_filename;

    const std::string APP_NAME = "Game controller events";
    const Size2i WINDOW_RESOLUTION = Size2i(640, 480);
    nom::RenderWindow window;

    // Event handling

    EventHandler evt_handler;
    nom::InputStateMapper input_mapper;

    nom::event_callback quit_app;
    nom::event_callback init_db_bindings;
    nom::event_callback x_axis_action;
    nom::event_callback y_axis_action;
    nom::event_callback button_action;

    // The maximum number of buttons to render
    static const nom::size_type MAX_BUTTONS =
      GameController::BUTTON_MAX;

    // The maximum number of axes to render
    static const nom::size_type MAX_AXES = 2;

    Point2i axis_rectangle_pos[MAX_AXES];
    Point2i button_rectangle_pos[MAX_BUTTONS];
    const Size2i AXIS_RECT_DIMS = Size2i(16, 16);
    const Size2i BUTTON_RECT_DIMS = Size2i(16, 16);

    /// \brief Test the simulation of input events.
    void test_simulated_events()
    {
      JoystickID dev_id = 0;
      auto button = nom::GameController::BUTTON_LEFT_SHOULDER;
      nom::Event cbutton_ev;

      cbutton_ev = nom::create_game_controller_button_press(dev_id, button);
      this->evt_handler.push_event(cbutton_ev);

      cbutton_ev = nom::create_game_controller_button_release(dev_id, button);
      this->evt_handler.push_event(cbutton_ev);
    }

    void
    render_rectangle(const Point2i& pos, const Size2i& dims, const Color4i& color)
    {
      IntRect rect_bounds;
      rect_bounds.set_position(pos);
      rect_bounds.set_size(dims);

      auto rect = Rectangle(rect_bounds, color);
      rect.draw(this->window);
    }

    /// \brief Load the game controller database for automatic joystick
    /// layout mapping.
    ///
    /// \see https://github.com/gabomdq/SDL_GameControllerDB
    void initialize_game_controller_db(const std::string& filename)
    {
      int mappings_count = GameController::load_mapping_file(filename);
      if( mappings_count == -1 ) {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Could not load the game controller database file:",
                      filename );
      } else {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                      "Added", mappings_count,
                      "game controller mappings from file:", filename );
      }
    }

    /// \brief Initialize joystick input bindings
    void on_game_controller_add(JoystickIndex dev_index)
    {
      // NOTE: This device index is platform-dependent and cannot be relied on,
      // as the order of joysticks change.
      auto dev_name = Joystick::name(dev_index);

      // NOTE: The device's id is unique and is what the input mapper relies on
      // for identifying joystick events by!
      auto dev_id = GameController::device_id(dev_index);

      if( this->initialize_game_controller_bindings(dev_id) == false ) {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Could not re-initialize game controller bindings",
                      "for instance ID", dev_id );
      } else {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                      "Initialized game controller bindings for",
                      dev_name, "(ID:", dev_id, ")" );
      }
    }

    /// \brief Install the input mappings for the new game controller device.
    bool initialize_game_controller_bindings(JoystickID dev_id)
    {
      bool result0 = false;
      bool result1 = false;
      bool result2 = false;
      InputActionMapper key, cbutton, caxis;
      nom::InputAction action;

      NOM_ASSERT(dev_id >= 0);
      if( dev_id < 0 ) {
        // Err -- invalid instance ID
        return false;
      }

      action =
        nom::KeyboardAction(Event::KEY_PRESS, SDLK_ESCAPE);
      key.insert("quit_app", action, this->quit_app);

      action =
        nom::KeyboardAction(Event::KEY_PRESS, SDLK_r);
      key.insert("reload_controller_mappings", action, this->init_db_bindings);

      // ...buttons...

      for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {

        std::stringstream desc;
        desc << "button_" << button_idx;

        auto button = NOM_SCAST(nom::GameController::Button, button_idx);

        action =
          nom::GameControllerButtonAction(  dev_id, button,
                                            nom::InputState::PRESSED );
        cbutton.insert(desc.str(), action, this->button_action);

        action =
          nom::GameControllerButtonAction(  dev_id, button,
                                            nom::InputState::RELEASED );
        cbutton.insert(desc.str(), action, this->button_action);
      }

      // ...left thumb axis...

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_LEFT_X);
      caxis.insert("left_thumb_left", action, this->x_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_LEFT_X);
      caxis.insert("left_thumb_right", action, this->x_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_LEFT_Y);
      caxis.insert("left_thumb_up", action, this->y_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_LEFT_Y);
      caxis.insert("left_thumb_down", action, this->y_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, GameController::AXIS_TRIGGER_LEFT);
      caxis.insert("left_trigger_axis", action, this->x_axis_action);

      // ...right thumb axis...

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_RIGHT_X);
      caxis.insert("right_thumb_left", action, this->x_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_RIGHT_X);
      caxis.insert("right_thumb_right", action, this->x_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_RIGHT_Y);
      caxis.insert("right_thumb_up", action, this->y_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_RIGHT_Y);
      caxis.insert("right_thumb_down", action, this->y_axis_action);

      action =
        nom::GameControllerAxisAction(dev_id, nom::GameController::AXIS_TRIGGER_RIGHT);
      caxis.insert("right_trigger_axis", action, this->x_axis_action);

      this->input_mapper.erase("game_controller_buttons");
      result0 =
        this->input_mapper.insert("game_controller_buttons", cbutton, true);

      this->input_mapper.erase("game_controller_axes");
      result1 =
        this->input_mapper.insert("game_controller_axes", caxis, true);

      this->input_mapper.erase("keyboard");
      result2 =
        this->input_mapper.insert("keyboard", key, true);

      return( result0 == true && result1 == true && result2 == true);
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
