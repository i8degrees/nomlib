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
#ifndef NOMLIB_SYSTEM_GAME_CONTROLLER_HPP
#define NOMLIB_SYSTEM_GAME_CONTROLLER_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/Joystick.hpp"

// Forward declarations (third-party)
typedef struct _SDL_Joystick SDL_Joystick;
typedef struct _SDL_GameController SDL_GameController;

namespace nom {

/// \brief Initialize the joystick and game controller subsystems.
///
/// \returns Boolean TRUE if the joystick and game controller subsystems was
/// successfully initialized, and boolean FALSE if the joystick and game
/// controller subsystems was **not** successfully initialized.
bool init_game_controller_subsystem();

/// \brief Shutdown the joystick and game controller subsystems.
void shutdown_game_controller_subsystem();

/// \brief Custom deleter for SDL_GameController pointers.
void GameControllerDeleter(SDL_GameController* dev);

/// \brief SDL2 game controller interface
class GameController
{
  public:
    /// \brief Game controller axis definitions.
    enum Axis: int8
    {
      AXIS_INVALID = -1,
      AXIS_LEFT_X,
      AXIS_LEFT_Y,
      AXIS_RIGHT_X,
      AXIS_RIGHT_Y,
      AXIS_TRIGGER_LEFT,
      AXIS_TRIGGER_RIGHT,
      AXIS_MAX,
    };

    /// \brief Game controller button definitions.
    enum Button: int8
    {
      BUTTON_INVALID = -1,
      BUTTON_A,
      BUTTON_B,
      BUTTON_X,
      BUTTON_Y,
      BUTTON_BACK,
      BUTTON_GUIDE,
      BUTTON_START,
      BUTTON_LEFT_STICK,
      BUTTON_RIGHT_STICK,
      BUTTON_LEFT_SHOULDER,
      BUTTON_RIGHT_SHOULDER,
      BUTTON_DPAD_UP,
      BUTTON_DPAD_DOWN,
      BUTTON_DPAD_LEFT,
      BUTTON_DPAD_RIGHT,
      BUTTON_MAX,
    };

    // NOTE: This is a workaround to prevent a double-free bug that
    // occasionally happens when the joystick device is closed more than once.
    // This issue can be reliably reproduced by adding a second
    // SDL_GameControllerClose call to test/testgamecontroller.c from the SDL
    // source repository.
    static bool device_closed_;

    typedef GameController self_type;

    GameController();

    ~GameController();

    /// \brief Get the underlying implementation pointer.
    ///
    /// \returns A non-owned pointer to the underlying implementation.
    SDL_Joystick* device() const;

    /// \brief Get the status of the joystick.
    ///
    /// \returns Boolean TRUE if the joystick has been opened, and boolean
    /// FALSE if it has not been opened.
    bool attached() const;

    /// \brief Get the instance ID of an opened game controller.
    ///
    /// \returns The device instance ID for the game controller device on
    /// success, and a negative error code on failure.
    JoystickID device_id() const;

    /// \brief Get the implementation-dependent name of the joystick.
    ///
    /// \returns The name of the joystick on success, and a null-terminated
    /// string on failure, such as an invalid device state.
    std::string name() const;

    /// \brief Open a joystick for use.
    ///
    /// \param device_index The device index of the joystick as recognized by
    /// SDL.
    ///
    /// \remarks The device index is **not** the same as the instance ID of the
    /// joystick, which is used to identify the joystick in future events.
    ///
    /// \see Joystick::num_joysticks
    bool open(JoystickIndex device_index);

    /// \brief Close an opened joystick.
    ///
    /// \remarks This effectively frees the resources of the joystick.
    void close();

    /// \brief Get the implementation-dependent name of the joystick.
    ///
    /// \param device_index The index of the joystick device, as recognized by
    /// SDL.
    ///
    /// \returns The name of the indexed joystick on success, and a zero-length
    /// string on failure, such as when no name for the device index can be found.
    ///
    /// \remarks This function can be called before any joysticks are accessed.
    ///
    /// \see Joystick::num_joysticks
    static std::string name(JoystickIndex device_index);

    /// \brief Toggle the game controller's event polling state.
    ///
    /// \param state One of the following: SDL_QUERY, SDL_IGNORE or SDL_ENABLE.
    ///
    /// \returns One (1) if enabled, zero (0) if disabled or a negative error
    /// code on failure. If the state was SDL_QUERY, then the current state is
    /// returned, otherwise the new processing state is returned.
    ///
    /// \remarks If game controller events are disabled, you must call
    /// GameController::update and manually check the state of the joystick
    /// when event data is needed.
    ///
    /// \note This does not disable the underlying joystick events.
    ///
    /// \see Joystick::set_event_state
    static int set_event_state(int state);

    /// \brief Update the current state of the opened game controllers.
    ///
    /// \remarks This method is automatically called by the internal event loop
    /// of SDL if any joystick events are enabled.
    static void update();

    std::string mapping_string();

    /// \remarks This function can be called before any joysticks are accessed.
    static std::string mapping_string(JoystickGUID guid);

    /// \todo Implement this function
    static int load_mapping_memory(const char* buffer, int buffer_size);

    /// \fixme This function call leaks memory from within SDL!
    static int load_mapping_file(const std::string& filename);

    static int load_mapping_string(const std::string& mapping);

    /// \brief Get the instance ID of a joystick device.
    ///
    /// \param device_index The index of the joystick device, as recognized by
    /// SDL.
    ///
    /// \returns The device instance ID for the joystick device on success, and
    /// a negative error code on failure.
    ///
    /// \remarks This function can be called before any joysticks are accessed.
    ///
    /// \see Joystick::num_joysticks
    static JoystickID device_id(JoystickIndex device_index);

    /// \brief Check to see if the joystick is supported by SDL's game
    /// controller interface.
    static bool compatible_joystick(JoystickIndex device_index);

  private:
    typedef std::unique_ptr<SDL_GameController, void (*)(SDL_GameController*)>
    joystick_dev;

    joystick_dev device_;
};

std::unique_ptr<GameController> make_unique_game_controller();
std::shared_ptr<GameController> make_shared_game_controller();

} // namespace nom

#endif // include guard defined

/// \class nom::GameController
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
