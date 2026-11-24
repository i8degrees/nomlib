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
#include <cstdint>
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
  STATE_PLAYING = 2,
  STATE_PAUSE = 3
};

constexpr nom::size_type MAX_BUTTONS =
  nom::GameController::BUTTON_MAX;
constexpr nom::size_type MAX_AXES = 2;

/// \brief Map a joystick instance identifier to a player color.
nom::Color4i player_color( nom::JoystickID id )
{
  static const nom::Color4i palette[] = {
    nom::Color4i::Blue,
    nom::Color4i::Red,
    nom::Color4i::Green,
    nom::Color4i::Orange,
    nom::Color4i::Magenta,
    nom::Color4i::Cyan,
    nom::Color4i::Yellow,
    nom::Color4i::White
  };

  auto idx = static_cast<size_t>( id ) %
             ( sizeof(palette) / sizeof(palette[0]) );
  return palette[idx];
}

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
/// \brief Encode a non-negative JoystickID as a void pointer.
///
/// JoystickID is a small integer; this lets it pass through StateMachine's
/// user-data pointer for per-instance construction arguments.
nom::void_ptr encode_joystick_id( nom::JoystickID id )
{
  return reinterpret_cast<nom::void_ptr>( static_cast<intptr_t>( id ) );
}

/// \brief Decode a JoystickID from a void pointer encoded by encode_joystick_id.
nom::JoystickID decode_joystick_id( nom::void_ptr data )
{
  return static_cast<nom::JoystickID>( reinterpret_cast<intptr_t>( data ) );
}

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
/// Installs a single "playing" input context containing keyboard, game
/// controller button and game controller axis bindings. The StateMachine
/// activates and disables that context as the state is entered and paused,
/// so the start button cannot re-trigger the pause overlay while it is
/// already active.
class PlayingState: public AppState
{
  public:
    PlayingState( App* app, nom::JoystickID dev_id );

    std::string input_context() const override;

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

/// \brief Pause overlay pushed on top of PlayingState.
///
/// Pressing the start button pops back to PlayingState. The example uses
/// push/pop so the PlayingState remains on the stack and
/// resumes automatically.
class PauseState: public AppState
{
  public:
    PauseState( App* app );

    std::string input_context() const override;
    void on_init( nom::void_ptr data ) override;
    void on_exit( nom::void_ptr data ) override;
    void on_draw( nom::RenderWindow& target ) override;

  private:
    void insert_start_button_binding( nom::InputActionMapper& map );
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
    nom::event_callback pause_action;

    // State machine integration
    nom::StateMachine& state_machine() { return this->state_machine_; }
    nom::InputStateMapper& input_mapper() { return this->input_mapper_; }
    nom::EventHandler& event_handler() { return this->evt_handler; }
    nom::RenderWindow& window() { return this->window_; }

    /// \brief Store the executable path for runtime resource discovery.
    void set_program_path( const char* argv0 );

    /// \brief Device ID of the currently active controller.
    nom::JoystickID current_controller_id() const { return this->current_controller_id_; }

    /// \brief Set the window title for the active application mode.
    void set_window_title(const std::string& title) { this->window_.set_window_title(title); this->title_ = title; }

    /// \brief Title shown when no pause overlay is active.
    const std::string& active_title() const { return this->title_; }

    // Layout constants used by state rendering
    const nom::Size2i AXIS_X_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i AXIS_Y_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i BUTTON_RECT_DIMS = nom::Size2i(16, 16);
    const nom::Size2i HAT_RECT_DIMS = nom::Size2i(4, 4);

    // Rendering state, mutated by input callbacks
    nom::Point2i axis_rectangle_pos[MAX_AXES];
    nom::Point2i button_rectangle_pos[MAX_BUTTONS];
    nom::Point2i hat_rectangle_pos = nom::Point2i::zero;

    // Color used to draw each rectangle; encodes which controller last
    // updated it.
    nom::Color4i axis_rectangle_color[MAX_AXES];
    nom::Color4i button_rectangle_color[MAX_BUTTONS];

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
    nom::JoystickID current_controller_id_ = -1;

    /// Index to reconnect after a deferred pop.
    nom::JoystickIndex pending_reconnect_index_ = -1;

    std::string title_;

    /// If true, log every game controller button/axis event for harness tests.
    bool verbose_input_ = false;
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
  return "playing";
}

void PlayingState::on_init( nom::void_ptr data )
{
  // Insert the context here, after the previous state's on_exit has erased
  // its keys. Inserting in the constructor would fail because the old state's
  // context is still present until the next update().
  nom::InputActionMapper map;
  this->insert_keyboard_bindings(map);

  // Listener mappings cover all currently connected game controllers, so
  // switching controllers or adding a second one does not break the overlay.
  const auto num_joysticks = nom::Joystick::num_joysticks();
  for( auto idx = 0; idx != num_joysticks; ++idx ) {
    const auto dev_id = nom::GameController::device_id(idx);
    this->insert_button_bindings(dev_id, map);
    this->insert_axis_bindings(dev_id, map);
  }

  this->app_->input_mapper().insert(
    this->input_context(), map, false );

  this->app_->set_window_title("Gamecontroller is connected");
}

void PlayingState::on_exit( nom::void_ptr data )
{
  // Diagnostics-only: the StateMachine erases the "playing" context after
  // this returns.
}

void PlayingState::on_draw( nom::RenderWindow& target )
{
  (void)target;
  App* app = this->app_;

  for( auto axis_idx = 0; axis_idx != MAX_AXES; ++axis_idx ) {
    if( axis_idx == 0 ) {
      app->render_rectangle(  app->axis_rectangle_pos[axis_idx],
                              app->AXIS_X_RECT_DIMS,
                              app->axis_rectangle_color[axis_idx] );
    }
    if( axis_idx != 0 ) {
      app->render_rectangle(  app->axis_rectangle_pos[axis_idx],
                              app->AXIS_Y_RECT_DIMS,
                              app->axis_rectangle_color[axis_idx] );
    }
  }

  app->render_rectangle(  app->hat_rectangle_pos,
                          app->HAT_RECT_DIMS,
                          nom::Color4i::Yellow );

  for( auto button_idx = 0; button_idx != MAX_BUTTONS; ++button_idx ) {
    app->render_rectangle(  app->button_rectangle_pos[button_idx],
                            app->BUTTON_RECT_DIMS,
                            app->button_rectangle_color[button_idx] );
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

  // Start button pauses the game by pushing the pause overlay.
  action = nom::GameControllerButtonAction(
              dev_id, nom::GameController::BUTTON_START,
              nom::InputState::PRESSED );
  map.insert("pause", action, this->app_->pause_action);

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

// PauseState implementation

PauseState::PauseState( App* app ) :
  AppState(STATE_PAUSE, app)
{
}

std::string PauseState::input_context() const
{
  return "paused";
}

void PauseState::on_init( nom::void_ptr data )
{
  nom::InputActionMapper map;
  this->insert_start_button_binding(map);

  this->app_->input_mapper().insert(
    this->input_context(), map, false );

  this->app_->set_window_title("Paused...");

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Paused" );
}

void PauseState::on_exit( nom::void_ptr data )
{
  // Restore the previous active title when the pause overlay is popped.
  this->app_->window().set_window_title(this->app_->active_title());

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Resumed from pause" );
}

void PauseState::on_draw( nom::RenderWindow& target )
{
  // PauseState intentionally leaves the visual feedback from PlayingState
  // untouched; the PlayingState renders behind the pause context.
  (void)target;
}

void PauseState::insert_start_button_binding( nom::InputActionMapper& map )
{
  // Resume by popping back to the PlayingState parent. Bind for every
  // connected controller so any controller can dismiss the overlay.
  const auto num_joysticks = nom::Joystick::num_joysticks();
  for( auto idx = 0; idx != num_joysticks; ++idx ) {
    nom::InputAction action = nom::GameControllerButtonAction(
                nom::GameController::device_id(idx),
                nom::GameController::BUTTON_START,
                nom::InputState::PRESSED );

    nom::event_callback resume = nom::event_callback( [this](const nom::Event&) {
      this->app_->state_machine().pop( STATE_PLAYING );
    });

    map.insert("resume", action, resume);
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

  for( auto i = 0u; i != MAX_AXES; ++i ) {
    this->axis_rectangle_color[i] = ( i == 0 ? nom::Color4i::Green
                                            : nom::Color4i::Orange );
  }

  for( auto i = 0u; i != MAX_BUTTONS; ++i ) {
    this->button_rectangle_color[i] = nom::Color4i::Blue;
  }

  const char* verbose_env = std::getenv("NOM_GC_VERBOSE");
  if( verbose_env != nullptr && std::string(verbose_env) == "1" ) {
    this->verbose_input_ = true;
  }

  this->state_machine_.set_input_mapper(&this->input_mapper_);

  // Register the two application states with the StateMachine. The Playing
  // state is constructed with the device ID supplied via the user data pointer
  // at transition time.
  this->state_machine_.register_state(
    {
      STATE_WAITING,
      NO_STATE,
      [this]( nom::void_ptr ) {
        return std::make_unique<WaitingState>( this );
      }
    }
  );

  this->state_machine_.register_state(
    {
      STATE_PLAYING,
      STATE_WAITING,
      [this]( nom::void_ptr data ) {
        auto dev_id = static_cast<nom::JoystickID>(
          reinterpret_cast<intptr_t>( data ) );
        return std::make_unique<PlayingState>( this, dev_id );
      }
    }
  );

  this->state_machine_.register_state(
    {
      STATE_PAUSE,
      STATE_PLAYING,
      [this]( nom::void_ptr ) {
        return std::make_unique<PauseState>( this );
      }
    }
  );

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
        this->axis_rectangle_color[0] = player_color( evt.caxis.id );
      } else {
        this->axis_rectangle_pos[1].x = rect_pos.x;
        this->axis_rectangle_color[1] = player_color( evt.caxis.id );
      }
    } else {
      if( axis < 2 ) {
        this->axis_rectangle_pos[0].x =
          (WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w) / 2;
        this->axis_rectangle_color[0] = player_color( evt.caxis.id );
      } else {
        this->axis_rectangle_pos[1].x =
          (WINDOW_RESOLUTION.w - AXIS_X_RECT_DIMS.w) / 2;
        this->axis_rectangle_color[1] = player_color( evt.caxis.id );
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
        this->axis_rectangle_color[0] = player_color( evt.caxis.id );
      } else {
        this->axis_rectangle_pos[1].y = rect_pos.y;
        this->axis_rectangle_color[1] = player_color( evt.caxis.id );
      }
    } else {
      if( axis < 2 ) {
        this->axis_rectangle_pos[0].y =
          (WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h) / 2;
        this->axis_rectangle_color[0] = player_color( evt.caxis.id );
      } else {
        this->axis_rectangle_pos[1].y =
          (WINDOW_RESOLUTION.h - AXIS_Y_RECT_DIMS.h) / 2;
        this->axis_rectangle_color[1] = player_color( evt.caxis.id );
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

      if( button_state == nom::InputState::PRESSED ) {
        this->button_rectangle_color[button] = player_color( evt.cbutton.id );
      }
    }
  });

  // Pause / resume via the controller start button.
  this->pause_action = nom::event_callback( [=](const nom::Event& evt) {
    this->state_machine_.push( STATE_PAUSE );
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

  if( this->verbose_input_ ) {
    this->evt_handler.append_event_watch(
      nom::event_filter([](const nom::Event& ev, void*) {
        if( ev.type == nom::Event::GAME_CONTROLLER_BUTTON_PRESS ) {
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "GC button event",
                        static_cast<int>(ev.cbutton.button) );
        } else if( ev.type == nom::Event::GAME_CONTROLLER_AXIS_MOTION ) {
          NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                        "GC axis event",
                        static_cast<int>(ev.caxis.axis),
                        ev.caxis.value );
        }
        return false;
      }), nullptr );
  }

  // Begin in the waiting state. If a controller is already connected, the
  // first GAME_CONTROLLER_ADDED event will transition us to PlayingState.
  this->state_machine_.push_state( STATE_WAITING );
  this->state_machine_.update(0.f);

  if( nom::Joystick::num_joysticks() < 1 ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "No joystick devices are available!" );
    this->set_window_title("Waiting on a gamecontroller to be connected...");
  } else {
    this->on_game_controller_add(0);
    this->state_machine_.update(0.f);
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
        } break;

        case nom::Event::GAME_CONTROLLER_REMOVED:
        {
          auto device_index = evt.cdevice.id;
          this->on_game_controller_remove(device_index);

          if( nom::Joystick::num_joysticks() > 0 ) {
            this->pending_reconnect_index_ = 0;
          } else {
            NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                          "No joystick devices are available!" );

            this->state_machine_.pop( STATE_WAITING );
            this->set_window_title("Waiting on a gamecontroller to be connected...");
          }
        } break;
      }
    } // end inner while

    // Let any deferred state transitions take effect before the active state
    // updates and renders the frame.
    this->state_machine_.update(0.f);

    // A controller was removed while another remains: rejoin PlayingState
    // after the pop(STATE_WAITING) completed.
    if( this->state_machine_.current_state_symbol() == STATE_WAITING &&
        nom::Joystick::num_joysticks() > 0 ) {
      this->on_game_controller_add(0);
    }

    // Reconnect to a remaining controller after a deferred pop completed.
    if( this->pending_reconnect_index_ != -1 ) {
      if( this->state_machine_.current_state_symbol() == STATE_PLAYING ||
          this->state_machine_.current_state_symbol() == STATE_PAUSE ) {
        // A new controller was added while already active; the on_add handler
        // already replaced the active state. Nothing to do.
      } else if( this->state_machine_.current_state_symbol() == STATE_WAITING ) {
        this->on_game_controller_add(this->pending_reconnect_index_);
      }
      this->pending_reconnect_index_ = -1;
    }

    this->window_.update();

    this->window_.fill(nom::Color4i::SkyBlue);

    // Render the state stack. PlayingState draws the visual feedback; if
    // PauseState is active, PlayingState renders behind the pause overlay.
    if( this->state_machine_.current_state_symbol() == STATE_PLAYING ||
        this->state_machine_.current_state_symbol() == STATE_PAUSE ) {
      this->state_machine_.draw(this->window_);
    }
  } // end outer while

  return NOM_EXIT_SUCCESS;
}

void App::on_game_controller_add( nom::JoystickIndex dev_index )
{
  auto dev_name = nom::Joystick::name(dev_index);
  auto dev_id = nom::GameController::device_id(dev_index);

  this->current_controller_id_ = dev_id;

  const auto active = this->state_machine_.current_state_symbol();
  if( active == STATE_PLAYING || active == STATE_PAUSE ) {
    // A different controller has been plugged in while already playing. Pop
    // back to Waiting so the main loop will push a fresh Playing state for
    // the new controller.
    this->state_machine_.pop( STATE_WAITING );
  } else {
    this->state_machine_.push( STATE_PLAYING, encode_joystick_id(dev_id) );
  }

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Initialized game controller bindings for",
                dev_name, "(ID:", dev_id, ")" );
}

void App::on_game_controller_remove( nom::JoystickIndex dev_index )
{
  auto dev_name = nom::Joystick::name(dev_index);
  auto dev_id = nom::GameController::device_id(dev_index);

  this->current_controller_id_ = -1;

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
