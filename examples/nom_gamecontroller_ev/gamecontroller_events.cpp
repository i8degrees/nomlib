/******************************************************************************

  nomlib - C++11 cross-platform game engine

  gamecontroller_events.cpp

  Demonstrates the new StateMachine + InputStateMapper integration using
  per-state input contexts that are automatically activated and disabled by the
  state machine during controller connection / disconnection transitions.

  Copyright (c) 2013, 2014, 2015 Jeffrey Carpenter <i8degrees@gmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include <climits>
#include <cstdlib>
#include <limits.h>

#include <nomlib/core.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/system/GameControllerEventHandler.hpp>

using namespace nom;

/// \brief Application state identifiers.
enum StateID {
  STATE_WAITING = 1,
  STATE_PLAYING = 2
};

constexpr nom::size_type MAX_BUTTONS =
  nom::GameController::BUTTON_MAX;
constexpr nom::size_type MAX_AXES = 2;

/// Resolve the SharedSupport resource directory relative to the executable.
///
/// Candidate layouts include:
///   - <exe>/Resources/SharedSupport
///   - <exe>/../Resources/SharedSupport             (macOS bundle)
///   - <exe>/../../Resources/SharedSupport            (development build/<cfg>)
///   - <exe>/../../../Resources/SharedSupport          (CMake build/<cfg>/bin)
///   - <exe>/../share/nomlib/Resources/SharedSupport   (installed prefix layout)
filesystem::path find_resource_dir( const char* argv0 )
{
  filesystem::path exe_dir;

  // Use realpath to resolve symlinks and collapse any ".."/"." segments
  // that may appear when argv[0] is unusual (e.g., rewritten by a launcher).
  char resolved[PATH_MAX];
  if( ::realpath( argv0, resolved ) != nullptr ) {
    exe_dir = filesystem::path( resolved ).parent_path();
  } else {
    filesystem::path argv_path( argv0 );
    if( argv_path.is_absolute() ) {
      exe_dir = argv_path.parent_path();
    } else {
      exe_dir = ( filesystem::current_path() / argv_path ).parent_path();
    }
  }

  const std::vector<filesystem::path> candidates = {
    exe_dir / "Resources" / "SharedSupport",
    exe_dir / ".." / "Resources" / "SharedSupport",
    exe_dir / ".." / ".." / "Resources" / "SharedSupport",
    exe_dir / ".." / ".." / ".." / "Resources" / "SharedSupport",
    exe_dir / ".." / "share" / "nomlib" / "Resources" / "SharedSupport"
  };

  // Fallback to the first candidate that actually exists, even if it does not
  // yet contain all assets. This avoids returning a synthesized directory path.
  for( const auto& c : candidates ) {
    if( filesystem::is_directory( c ) ) {
      return c;
    }
  }

  // Last resort: a source-tree relative path where input device mappings live.
  return filesystem::current_path() / "Resources" / "SharedSupport";
}

class App;

/// \brief Base state shared by the example's two states.
///
/// Holds onto a non-owning pointer back to App so that input callbacks can
/// mutate the rendering state (joystick axis & button positions).
class AppState: public nom::IState
{
  public:
    AppState( nom::uint32 id, App* app ) :
      nom::IState(id),
      app_{app}
    {
    }

  protected:
    App* app_;
};

/// \brief State shown when no game controller is connected.
///
/// Only binds the keyboard quit / reload mappings.
class WaitingState: public AppState
{
  public:
    WaitingState( App* app );

    std::string input_context() const override;
    void on_init( nom::void_ptr data ) override;
    void on_draw( nom::RenderWindow& target ) override;

  private:
    void insert_keyboard_bindings( nom::InputActionMapper& map );
};

/// \brief State shown when at least one game controller is connected.
///
/// Installs keyboard, game controller button and game controller axis input
/// contexts. The StateMachine disables the contexts when the controller is
/// disconnected and erases them when the state exits.
class PlayingState: public AppState
{
  public:
    PlayingState( App* app, nom::JoystickID dev_id );

    std::string input_context() const override;

    /// \remarks Each PlayingState owns three distinct input context keys.
    /// The state machine only activates the primary key returned here; the
    /// on_init callback activates the companion keys explicitly.
    void on_init( nom::void_ptr data ) override;
    void on_exit( nom::void_ptr data ) override;
    void on_draw( nom::RenderWindow& target ) override;

  private:
    void insert_keyboard_bindings( nom::InputActionMapper& map );
    void insert_button_bindings(  nom::JoystickID dev_id,
                                nom::InputActionMapper& map );
    void insert_axis_bindings(  nom::JoystickID dev_id,
                                nom::InputActionMapper& map );

    nom::JoystickID dev_id_;
};

class App: public nom::SDLApp
{
  public:
    App();
    ~App();

    bool on_init() override;
    int Run() override;

    void on_game_controller_add( nom::JoystickIndex dev_index );
    void on_game_controller_remove( nom::JoystickIndex dev_index );

    bool initialize_game_controller_db( const std::string& filename );
    void render_rectangle(  const nom::Point2i& pos,
                            const nom::Size2i& dims,
                            const nom::Color4i& color );
    void test_simulated_events();

    // Event callbacks installed by states
    nom::event_callback quit_app;
    nom::event_callback init_db_bindings;
    nom::event_callback x_axis_action;
    nom::event_callback y_axis_action;
    nom::event_callback button_action;

    // State machine integration
    nom::StateMachine& state_machine() { return this->state_machine_; }
    nom::InputStateMapper& input_mapper() { return this->input_mapper_; }
    nom::EventHandler& event_handler() { return this->evt_handler; }
    nom::RenderWindow& window() { return this->window_; }

    /// \brief Store the executable path for runtime resource discovery.
    void set_program_path( const char* argv0 );

    // Layout constants used by state rendering
    const nom::Size2i AXIS_X_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i AXIS_Y_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i BUTTON_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i HAT_RECT_DIMS = nom::Size2i(4, 4);

    // Rendering state, mutated by input callbacks
    nom::Point2i axis_rectangle_pos[MAX_AXES];
    nom::Point2i button_rectangle_pos[MAX_BUTTONS];
    nom::Point2i hat_rectangle_pos = nom::Point2i::zero;

  private:
    std::string program_path_;
    std::string db_filename;

    const std::string APP_NAME = "Game controller events";
    const nom::Size2i WINDOW_RESOLUTION = nom::Size2i(640, 480);
    nom::RenderWindow window_;

    nom::EventHandler evt_handler;
    nom::InputStateMapper input_mapper_;
    nom::StateMachine state_machine_;

    nom::GameControllerEventHandler joystick_evt;
};

// AppState implementation is header-only above.

WaitingState::WaitingState( App* app ) :
  AppState(STATE_WAITING, app)
{
}

std::string WaitingState::input_context() const
{
  return "waiting";
}

void WaitingState::on_init( nom::void_ptr data )
{
  nom::InputActionMapper map;
  this->insert_keyboard_bindings(map);

  this->app_->input_mapper().insert(
    this->input_context(), map, false );
}

void WaitingState::on_draw( nom::RenderWindow& target )
{
  // No controller-specific rendering when waiting.
  // The App::Run loop already fills the background.
}

void WaitingState::insert_keyboard_bindings( nom::InputActionMapper& map )
{
  nom::InputAction action;

  action = nom::KeyboardAction(SDLK_q);
  map.insert("quit_app", action, this->app_->quit_app);

  action = nom::KeyboardAction(SDLK_ESCAPE);
  map.insert("quit_app", action, this->app_->quit_app);

  action = nom::KeyboardAction(SDLK_r);
  map.insert("reload_controller_mappings", action, this->app_->init_db_bindings);
}

PlayingState::PlayingState( App* app, nom::JoystickID dev_id ) :
  AppState(STATE_PLAYING, app),
  dev_id_{dev_id}
{
}

std::string PlayingState::input_context() const
{
  return "keyboard";
}

void PlayingState::on_init( nom::void_ptr data )
{
  // Insert contexts here, after the previous state's on_exit has erased its
  // keys. Inserting in the constructor would fail because the old state's
  // contexts are still present until the next update().
  nom::InputActionMapper key;
  this->insert_keyboard_bindings(key);
  this->app_->input_mapper().insert("keyboard", key, false);

  nom::InputActionMapper buttons;
  this->insert_button_bindings(this->dev_id_, buttons);
  this->app_->input_mapper().insert(
    "game_controller_buttons", buttons, false );

  nom::InputActionMapper axes;
  this->insert_axis_bindings(this->dev_id_, axes);
  this->app_->input_mapper().insert(
    "game_controller_axes", axes, false );

  // Activate the companion contexts alongside the primary "keyboard" context
  // that the state machine already activated.
  this->app_->input_mapper().activate("game_controller_buttons");
  this->app_->input_mapper().activate("game_controller_axes");
}

void PlayingState::on_exit( nom::void_ptr data )
{
  // Diagnostics-only: the StateMachine already erases the primary context.
  // The companion contexts are erased automatically by the machine because
  // they share this state's input_context() key? No -- they have distinct keys
  // and were inserted in the constructor. They will be erased by on_exit
  // because we erase all three keys here explicitly to keep the example tidy.
  this->app_->input_mapper().erase("game_controller_buttons");
  this->app_->input_mapper().erase("game_controller_axes");
}

void PlayingState::on_draw( nom::RenderWindow& target )
{
  App* app = this->app_;

  for( auto axis_idx = 0; axis_idx != MAX_AXES; ++axis_idx ) {
    if( axis_idx == 0 ) {
      app->render_rectangle(  app->axis_rectangle_pos[axis_idx],
                              app->AXIS_X_RECT_DIMS,
                              nom::Color4i::Green );
    }
    if( axis_idx != 0 ) {
      app->render_rectangle(  app->axis_rectangle_pos[axis_idx],
                              app->AXIS_Y_RECT_DIMS,
                              nom::Color4i::Orange );
    }
  }

  app->render_rectangle(  app->hat_rectangle_pos,
                          app->HAT_RECT_DIMS,
                          nom::Color4i::Yellow );

  for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {
    app->render_rectangle(  app->button_rectangle_pos[button_idx],
                            app->BUTTON_RECT_DIMS,
                            nom::Color4i::Blue );
  }
}

void PlayingState::insert_keyboard_bindings( nom::InputActionMapper& map )
{
  nom::InputAction action;

  action = nom::KeyboardAction(SDLK_q);
  map.insert("quit_app", action, this->app_->quit_app);

  action = nom::KeyboardAction(SDLK_ESCAPE);
  map.insert("quit_app", action, this->app_->quit_app);

  action = nom::KeyboardAction(SDLK_r);
  map.insert("reload_controller_mappings", action, this->app_->init_db_bindings);
}

void PlayingState::insert_button_bindings(  nom::JoystickID dev_id,
                                              nom::InputActionMapper& map )
{
  nom::InputAction action;

  for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {

    std::stringstream desc;
    desc << "button_" << button_idx;

    auto button = NOM_SCAST(nom::GameController::Button, button_idx);

    action = nom::GameControllerButtonAction(
                dev_id, button, nom::InputState::PRESSED );
    map.insert(desc.str(), action, this->app_->button_action);

    action = nom::GameControllerButtonAction(
                dev_id, button, nom::InputState::RELEASED );
    map.insert(desc.str(), action, this->app_->button_action);
  }
}

void PlayingState::insert_axis_bindings(  nom::JoystickID dev_id,
                                          nom::InputActionMapper& map )
{
  nom::InputAction action;

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_LEFT_X );
  map.insert("left_thumb_left", action, this->app_->x_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_LEFT_X );
  map.insert("left_thumb_right", action, this->app_->x_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_LEFT_Y );
  map.insert("left_thumb_up", action, this->app_->y_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_LEFT_Y );
  map.insert("left_thumb_down", action, this->app_->y_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_TRIGGER_LEFT );
  map.insert("left_trigger_axis", action, this->app_->x_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_RIGHT_X );
  map.insert("right_thumb_left", action, this->app_->x_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_RIGHT_X );
  map.insert("right_thumb_right", action, this->app_->x_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_RIGHT_Y );
  map.insert("right_thumb_up", action, this->app_->y_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_RIGHT_Y );
  map.insert("right_thumb_down", action, this->app_->y_axis_action);

  action = nom::GameControllerAxisAction(
              dev_id, nom::GameController::AXIS_TRIGGER_RIGHT );
  map.insert("right_trigger_axis", action, this->app_->x_axis_action);
}

App::App()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->state_machine_.set_input_mapper(&this->input_mapper_);

  // quit_app
  this->quit_app = nom::event_callback( [=](const nom::Event& evt) {
    int32 numJoysticks = this->joystick_evt.num_joysticks();
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, numJoysticks,
                  "remaining in pool");
    this->on_app_quit(evt);
  });

  // reload_controller_mappings
  this->init_db_bindings = nom::event_callback( [=](const nom::Event& evt) {
    if(this->initialize_game_controller_db(this->db_filename) == false) {
      this->initialize_game_controller_db("gamecontrollerdb.txt");
    }
  });

  // left_thumb_left / right_thumb_left / left_trigger_axis / right_trigger_axis
  this->x_axis_action = nom::event_callback( [=](const nom::Event& evt) {

    int32 axis = evt.caxis.axis;
    int32 axis_value = evt.caxis.value;
    nom::Point2i rect_pos;

    if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ||
        axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE )
    {
      rect_pos.x = axis_value + 32768;

      if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
        rect_pos.x -= nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
      } else if( axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
        rect_pos.x += nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
      }

      rect_pos.x *= WINDOW_RESOLUTION.w;
      rect_pos.x /= 65535;

      if( rect_pos.x < 0 ) {
        rect_pos.x = 0;
      } else if( rect_pos.x > WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w ) {
        rect_pos.x = WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w;
      }

      if( axis < 2 ) {
        this->axis_rectangle_pos[0].x = rect_pos.x;
      } else {
        this->axis_rectangle_pos[1].x = rect_pos.x;
      }
    } else {
      if( axis < 2 ) {
        this->axis_rectangle_pos[0].x =
          (WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w) / 2;
      } else {
        this->axis_rectangle_pos[1].x =
          (WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w) / 2;
      }
    }
  });

  // left_thumb_up / right_thumb_up / left_trigger_axis / right_trigger_axis
  this->y_axis_action = nom::event_callback( [=](const nom::Event& evt) {

    int32 axis = evt.caxis.axis;
    int32 axis_value = evt.caxis.value;
    nom::Point2i rect_pos;

    if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ||
        axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE )
    {
      rect_pos.y = axis_value + 32768;

      if( axis_value > nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
        rect_pos.y -= nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
      } else if( axis_value < -nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE ) {
        rect_pos.y += nom::JOYSTICK_LEFT_THUMB_DEAD_ZONE;
      }

      rect_pos.y *= WINDOW_RESOLUTION.h;
      rect_pos.y /= 65535;

      if( rect_pos.y < 0 ) {
        rect_pos.y = 0;
      } else if( rect_pos.y > WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h ) {
        rect_pos.y = WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h;
      }

      if( axis < 2 ) {
        this->axis_rectangle_pos[0].y = rect_pos.y;
      } else {
        this->axis_rectangle_pos[1].y = rect_pos.y;
      }
    } else {
      if( axis < 2 ) {
        this->axis_rectangle_pos[0].y =
          (WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h) / 2;
      } else {
        this->axis_rectangle_pos[1].y =
          (WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h) / 2;
      }
    }
  });

  // gamecontroller_buttons
  this->button_action = nom::event_callback( [=](const nom::Event& evt) {

    int32 button = evt.cbutton.button;
    int32 button_state = evt.cbutton.state;
    nom::Point2i rect_pos;

    if( button_state == nom::InputState::PRESSED ) {
      auto spacing = BUTTON_RECT_DIMS.w * 2;
      auto origin = BUTTON_RECT_DIMS.w;
      rect_pos.x = origin + (spacing * button);
      rect_pos.y = WINDOW_RESOLUTION.h - BUTTON_RECT_DIMS.h * 2;
    } else {
      rect_pos.x = -WINDOW_RESOLUTION.w;
      rect_pos.y = -WINDOW_RESOLUTION.h;
    }

    if( button < MAX_BUTTONS ) {
      this->button_rectangle_pos[button] = rect_pos;
    }
  });
}

App::~App()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

void App::set_program_path( const char* argv0 )
{
  this->program_path_ = argv0 ? argv0 : "";
}

bool App::on_init()
{
  nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

  filesystem::path resource_dir = find_resource_dir(this->program_path_.c_str());

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Resource directory:", resource_dir.native() );

  this->db_filename =
    ( resource_dir / "gamecontrollerdb.txt" ).native();

  if( this->window_.create( APP_NAME, WINDOW_RESOLUTION,
      window_flags ) == false )
  {
    return false;
  }

  NOM_ASSERT( this->evt_handler.joystick_event_type() ==
              nom::EventHandler::NO_EVENT_HANDLER );

  if( this->evt_handler.enable_game_controller_polling() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize game controller subsystem: ",
                  nom::error() );
    return false;
  }

  NOM_ASSERT( this->evt_handler.joystick_event_type() ==
              nom::EventHandler::GAME_CONTROLLER_EVENT_HANDLER );

  // Load the community game controller mapping database when it is available.
  // The file is installed/copied only when NOM_INSTALL_GCDB is enabled.
  if( filesystem::is_regular_file( filesystem::path(this->db_filename) ) ) {
    this->initialize_game_controller_db(this->db_filename);
  } else {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "No game controller database found at:",
                  this->db_filename,
                  "Using built-in SDL mappings instead." );
  }

  // NOTE: Ensure that closing the joystick device more than once does not
  // crash with a double-free memory violation from SDL_GameControllerClose
  {
    nom::GameController jdev;
    for(  auto joystick_index = 0;
          joystick_index != nom::Joystick::num_joysticks();
          ++joystick_index )
    {
      if( jdev.open(joystick_index) == true ) {
        jdev.close();
        jdev.close();
        jdev.close();
      }
    }
  }

  nom::Joystick jdev;
  for(  auto joystick_index = 0;
        joystick_index != nom::Joystick::num_joysticks();
        ++joystick_index )
  {
    if( jdev.open(joystick_index) == true ) {

      std::string guid_str = "0x" + jdev.device_guid_string();

      NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Joystick",
                    joystick_index, ":", nom::Joystick::name(joystick_index) );

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
        joystick_index != nom::Joystick::num_joysticks();
        ++joystick_index )
  {
    if( nom::GameController::compatible_joystick(joystick_index) == false ) {
      NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION, "Joystick",
                    joystick_index, ":", nom::Joystick::name(joystick_index),
                    "is not supported by SDL's game controller interface!" );
    }
  }

  // Initialize the visual joystick state positions
  for( auto axis_idx = 0; axis_idx != MAX_AXES; ++axis_idx ) {
    this->axis_rectangle_pos[axis_idx] =
      nom::alignment_rect(  AXIS_Y_RECT_DIMS, nom::Point2i::zero,
                            WINDOW_RESOLUTION, nom::Anchor::MiddleCenter );
  }

  for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {
    this->button_rectangle_pos[button_idx].x = -WINDOW_RESOLUTION.w;
    this->button_rectangle_pos[button_idx].y = -WINDOW_RESOLUTION.h;
  }

  this->hat_rectangle_pos =
    nom::alignment_rect(  HAT_RECT_DIMS, nom::Point2i::zero,
                          WINDOW_RESOLUTION, nom::Anchor::MiddleCenter );

  // Wire the event handler into SDLApp so hardware events reach us, and into
  // the input mapper so action callbacks are triggered.
  nom::SDLApp::set_event_handler(this->evt_handler);
  this->input_mapper_.set_event_handler(this->evt_handler);

  // Begin in the waiting state. If a controller is already connected, the
  // first GAME_CONTROLLER_ADDED event will transition us to PlayingState.
  this->state_machine_.set_state(
    std::make_unique<WaitingState>(this), nullptr );
  this->state_machine_.update(0.f);

  if( nom::Joystick::num_joysticks() < 1 ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "No joystick devices are available!" );
    this->window_.set_window_title("Waiting on a gamecontroller to be connected...");
  }

  return true;
}

int App::Run()
{
  this->window_.fill(nom::Color4i::SkyBlue);

  while( this->running() == true ) {

    nom::Event evt;
    while( evt_handler.poll_event(evt) == true ) {

      switch(evt.type)
      {
        default: break;

        case nom::Event::JOYSTICK_ADDED:
        case nom::Event::JOYSTICK_REMOVED:
        {
          NOM_ASSERT_INVALID_PATH();
        } break;

        case nom::Event::GAME_CONTROLLER_ADDED:
        {
          auto device_index = evt.cdevice.id;
          this->on_game_controller_add(device_index);

          this->test_simulated_events();

          this->window_.set_window_title("Gamecontroller is connected");
        } break;

        case nom::Event::GAME_CONTROLLER_REMOVED:
        {
          auto device_index = evt.cdevice.id;
          this->on_game_controller_remove(device_index);

          if( nom::Joystick::num_joysticks() > 0 ) {
            this->on_game_controller_add(0);
          } else {
            NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                          "No joystick devices are available!" );

            this->window_.set_window_title("Waiting on a gamecontroller to be connected...");
          }
        } break;
      }
    } // end inner while

    // Let any deferred state transitions take effect before the active state
    // updates and renders the frame.
    this->state_machine_.update(0.f);

    this->window_.update();

    this->window_.fill(nom::Color4i::SkyBlue);

    // Only render controller feedback when a controller is connected.
    if( this->state_machine_.current_state_id() == STATE_PLAYING ) {
      this->state_machine_.draw(this->window_);
    }
  } // end outer while

  return NOM_EXIT_SUCCESS;
}

void App::on_game_controller_add( nom::JoystickIndex dev_index )
{
  auto dev_name = nom::Joystick::name(dev_index);
  auto dev_id = nom::GameController::device_id(dev_index);

  if( this->state_machine_.current_state_id() == STATE_PLAYING ) {
    // Already in playing state; just update the active context's device id.
    // For simplicity, we insert a fresh PlayingState so mappings are rebound.
  }

  this->state_machine_.set_state(
    std::make_unique<PlayingState>(this, dev_id), nullptr );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Initialized game controller bindings for",
                dev_name, "(ID:", dev_id, ")" );
}

void App::on_game_controller_remove( nom::JoystickIndex dev_index )
{
  auto dev_name = nom::Joystick::name(dev_index);
  auto dev_id = nom::GameController::device_id(dev_index);

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Removed game controller", dev_name,
                "(ID:", dev_id, ")" );
}

bool App::initialize_game_controller_db( const std::string& filename )
{
  int mappings_count = nom::GameController::load_mapping_file(filename);
  if( mappings_count == -1 ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load the game controller database file:",
                  filename );
    return false;
  } else {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "Added", mappings_count,
                  "game controller mappings from file:", filename );
  }

  return true;
}

void App::render_rectangle( const nom::Point2i& pos,
                            const nom::Size2i& dims,
                            const nom::Color4i& color )
{
  nom::IntRect rect_bounds;
  rect_bounds.set_position(pos);
  rect_bounds.set_size(dims);

  auto rect = nom::Rectangle(rect_bounds, color);
  rect.draw(this->window_);
}

void App::test_simulated_events()
{
  nom::JoystickID dev_id = 0;
  auto button = nom::GameController::BUTTON_LEFT_SHOULDER;
  nom::Event cbutton_ev;

  cbutton_ev = nom::create_game_controller_button_press(dev_id, button);
  this->evt_handler.push_event(cbutton_ev);

  cbutton_ev = nom::create_game_controller_button_release(dev_id, button);
  this->evt_handler.push_event(cbutton_ev);
}

int main(nom::int32 argc, char* argv[])
{
  if( nom::init(argc, argv) == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize nomlib." );
    return NOM_EXIT_FAILURE;
  }

  atexit(nom::quit);

  nom::SDL2Logger::set_logging_priority(  NOM_LOG_CATEGORY_EVENT,
                                          NOM_LOG_PRIORITY_INFO );

  nom::set_hint(NOM_EVENT_QUEUE_STATISTICS, "1");

  App app;
  app.set_program_path(argv[0]);

  if( app.on_init() == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();
}
