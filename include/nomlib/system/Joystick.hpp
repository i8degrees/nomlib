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
#ifndef NOMLIB_SYSTEM_JOYSTICK_HPP
#define NOMLIB_SYSTEM_JOYSTICK_HPP

#include <string>
#include <memory>

#include "nomlib/config.hpp"

// Forward declarations (third-party)
typedef struct _SDL_Joystick SDL_Joystick;

namespace nom {

typedef int32 JoystickIndex;

/// \note This is a forward declaration of SDL_JoystickID.
typedef int32 JoystickID;

/// \brief the maximal data size of the JoystickGUID structure.
///
/// \note This should always match the size of the data array member of
/// SDL_JoystickGUID.
const nom::size_type GUID_MAX_LENGTH = 16;

/// \brief Internal representation of a stable, fixed identifier -- 128 bits
/// wide -- for a joystick device. This identifier is platform-dependent.
///
/// \note This is a forward declaration of SDL_JoystickGUID.
struct JoystickGUID
{
  uint8 data[GUID_MAX_LENGTH];
};

/// \brief The default dead zone values for joysticks.
///
/// \note NOTE: These values are sourced from XInput.h, documented at [MSDN: Getting Started With XInput](https://msdn.microsoft.com/en-us/library/windows/desktop/ee417001%28v=vs.85%29.aspx#dead_zone).
const int16 JOYSTICK_LEFT_THUMB_DEAD_ZONE = 7849;
const int16 JOYSTICK_RIGHT_THUMB_DEAD_ZONE = 8689;
const int16 JOYSTICK_TRIGGER_THRESHOLD = 30;

/// \brief Initialize the joystick subsystem.
///
/// \returns Boolean TRUE if the joystick subsystem was successfully
/// initialized, and boolean FALSE if the joystick subsystem was **not**
/// successfully initialized.
bool init_joystick_subsystem();

/// \brief Shutdown the joystick subsystem.
void shutdown_joystick_subsystem();

/// \brief Custom deleter for SDL_Joystick pointers.
void JoystickDeleter(SDL_Joystick* dev);

/// \brief SDL2 joystick interface
class Joystick
{
  public:
    /// \brief Joystick axis definitions.
    enum Axis: uint8
    {
      AXIS_ZERO = 0,
      AXIS_ONE,
      AXIS_TWO,
      AXIS_THREE,
      AXIS_FOUR,
      AXIS_FIVE,
      AXIS_SIX,
      AXIS_SEVEN,
      AXIS_EIGHT,
      AXIS_MAX,
    };

    /// \brief Joystick button definitions.
    enum Button: uint8
    {
      BUTTON_ZERO = 0,
      BUTTON_ONE,
      BUTTON_TWO,
      BUTTON_THREE,
      BUTTON_FOUR,
      BUTTON_FIVE,
      BUTTON_SIX,
      BUTTON_SEVEN,
      BUTTON_EIGHT,
      BUTTON_NINE,
      BUTTON_TEN,
      BUTTON_ELEVEN,
      BUTTON_TWELVE,
      BUTTON_FOURTEEN,
      BUTTON_FIFTEEN,
      BUTTON_SIXTEEN,
      BUTTON_MAX,
    };

    /// \brief Joystick hat definitions.
    enum Hat: uint8
    {
      HAT_ZERO = 0,
      HAT_ONE,
      HAT_TWO,
      HAT_THREE,
      HAT_FOUR,
      HAT_MAX,
    };

    /// \brief Joystick hat position definitions.
    enum HatPosition: uint8
    {
      HAT_CENTERED = 0x00,
      HAT_UP = 0x01,
      HAT_RIGHT = 0x02,
      HAT_DOWN = 0x04,
      HAT_LEFT = 0x08,
      HAT_RIGHTUP = (HAT_RIGHT | HAT_UP),
      HAT_RIGHTDOWN = (HAT_RIGHT | HAT_DOWN),
      HAT_LEFTUP = (HAT_LEFT | HAT_UP),
      HAT_LEFTDOWN = (HAT_LEFT | HAT_DOWN),
    };

    typedef Joystick self_type;

    Joystick();

    ~Joystick();

    /// \brief Get the underlying implementation pointer.
    ///
    /// \returns A non-owned pointer to the underlying implementation.
    SDL_Joystick* device() const;

    /// \brief Get the status of the joystick.
    ///
    /// \returns Boolean TRUE if the joystick has been opened, and boolean
    /// FALSE if it has not been opened.
    bool attached() const;

    /// \brief Get the instance ID of an opened joystick.
    ///
    /// \returns The device instance ID for the joystick device on success, and
    /// a negative error code on failure.
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

    /// \brief Get the number of axes for this joystick.
    ///
    /// \returns The number of axes on success, and a negative error code on
    /// failure.
    ///
    /// \remarks This function requires an opened joystick.
    int num_axes();

    /// \brief Get the number of track balls for this joystick.
    ///
    /// \returns The number of track balls on success, and a negative error
    /// code on failure.
    ///
    /// \remarks This function requires an opened joystick.
    int num_track_balls();

    /// \brief Get the number of buttons for this joystick.
    ///
    /// \returns The number of buttons on success, and a negative error code on
    /// failure.
    ///
    /// \remarks This function requires an opened joystick.
    int num_buttons();

    /// \brief Get the number of POV hats for this joystick.
    ///
    /// \returns The number of POV hats on success, and a negative error code
    /// on failure.
    ///
    /// \remarks This function requires an opened joystick.
    int num_hats();

    /// \brief Toggle the joystick event polling state.
    ///
    /// \param state One of the following: SDL_QUERY, SDL_IGNORE or SDL_ENABLE.
    ///
    /// \returns One (1) if enabled, zero (0) if disabled and a negative error
    /// code on failure. If the state was SDL_QUERY, then the current state is
    /// returned, otherwise the new processing state is returned.
    ///
    /// \remarks If joystick events are disabled, you must call
    /// Joystick::update and manually check the state of the joystick when
    /// event data is needed.
    ///
    /// \note Calling this method may delete all events currently in SDL's
    /// event queue.
    static int set_event_state(int state);

    /// \brief Update the current state of the opened joysticks.
    ///
    /// \remarks This method is automatically called by the internal event loop
    /// of SDL if any joystick events are enabled.
    static void update();

    /// \brief Get the number of joysticks attached to the system.
    ///
    /// \returns The number of attached joysticks on success, and a negative
    /// error code on failure.
    static int num_joysticks();

    /// \brief Get the implementation-dependent name of the joystick.
    ///
    /// \param device_index The index of the joystick device, as recognized by
    /// SDL.
    ///
    /// \returns The name of the indexed joystick on success, and a zero-length
    /// string on failure, such as when no name for the device index can be
    /// found.
    ///
    /// \remarks This function can be called before any joysticks are accessed.
    ///
    /// \see Joystick::num_joysticks
    static std::string name(JoystickIndex device_index);

    /// \brief Get the implementation-dependent GUID for this joystick
    /// instance.
    ///
    /// \returns The GUID of the joystick on success, and a zero-length GUID on
    /// failure, such as when the device state is invalid.
    ///
    /// \remarks This function requires an opened joystick.
    ///
    /// \see Joystick::attached
    JoystickGUID device_guid() const;

    /// \brief Get the implementation-dependent GUID for a joystick.
    ///
    /// \param device_index The index of the joystick device, as recognized by
    /// SDL.
    ///
    /// \returns The GUID of the joystick on success, and a zero-length GUID on
    /// failure, such as when an invalid index is passed.
    ///
    /// \remarks This function can be called before any joysticks are accessed.
    ///
    /// \see Joystick::num_joysticks
    static JoystickGUID device_guid(JoystickIndex device_index);

    /// \brief Get the ASCII string representation of a joystick GUID.
    ///
    /// \returns The GUID string of the joystick on success, and a zero-length
    /// GUID on failure, such as when an invalid GUID has been passed.
    ///
    /// \remarks This function can be called before any joysticks are accessed.
    static std::string device_guid_string(JoystickGUID guid);

    /// \brief Get the ASCII string representation of this joystick instance
    /// GUID.
    ///
    /// \returns The GUID string of the joystick on success, and a zero-length
    /// GUID on failure, such as when the device state is invalid.
    ///
    /// \remarks This function requires an opened joystick.
    ///
    /// \see Joystick::attached
    std::string device_guid_string() const;

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

  private:
    typedef std::unique_ptr<SDL_Joystick, void (*)(SDL_Joystick*)> joystick_dev;

    joystick_dev device_;
};

std::unique_ptr<Joystick> make_unique_joystick();
std::shared_ptr<Joystick> make_shared_joystick();

JoystickGUID convert_SDL_JoystickGUID(SDL_JoystickGUID dev_guid);
SDL_JoystickGUID convert_SDL_JoystickGUID(JoystickGUID dev_guid);

} // namespace nom

#endif // include guard defined

/// \class nom::Joystick
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
