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
#include "nomlib/system/GameController.hpp"

// Private helpers
#include "nomlib/core/err.hpp"
#include "nomlib/core/helpers.hpp"
#include "nomlib/system/SDL_helpers.hpp"

// Forward declarations
#include <SDL.h>

static_assert(  nom::GameController::AXIS_INVALID ==
                SDL_CONTROLLER_AXIS_INVALID, "Event mismatch" );
static_assert(  nom::GameController::AXIS_MAX ==
                SDL_CONTROLLER_AXIS_MAX, "Event mismatch" );

static_assert(  nom::GameController::BUTTON_INVALID ==
                SDL_CONTROLLER_BUTTON_INVALID, "Event mismatch" );
static_assert(  nom::GameController::BUTTON_MAX ==
                SDL_CONTROLLER_BUTTON_MAX, "Event mismatch" );

namespace nom {

bool init_game_controller_subsystem()
{
  uint32 init_flags = SDL_INIT_GAMECONTROLLER;

  if( nom::init_joystick_subsystem() == false ) {
    return false;
  }

  if( SDL_WasInit(init_flags) == 0 ) {
    if( SDL_InitSubSystem(init_flags) < 0 ) {
      nom::set_error( SDL_GetError() );
      return false;
    }
  }

  return true;
}

void shutdown_game_controller_subsystem()
{
  if( SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0 ) {
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
  }

  nom::shutdown_joystick_subsystem();
}

bool GameController::device_closed_ = false;

void GameControllerDeleter(SDL_GameController* dev)
{
  if( GameController::device_closed_ == true ) {
    return;
  }

  if( dev != nullptr && SDL_GameControllerGetAttached(dev) == true ) {
    SDL_GameControllerClose(dev);
  }
}

GameController::GameController() :
  device_( joystick_dev(nullptr, GameControllerDeleter) )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );
}

GameController::~GameController()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );
}

SDL_Joystick* GameController::device() const
{
  SDL_Joystick* joy_dev =
    SDL_GameControllerGetJoystick( this->device_.get() );
  if( joy_dev == nullptr ) {
    nom::set_error( SDL_GetError() );
  }

  return joy_dev;
}

bool GameController::attached() const
{
  SDL_bool result = SDL_GameControllerGetAttached( this->device_.get() );

  if( result == SDL_FALSE ) {
    nom::set_error( SDL_GetError() );
    return false;
  }

  return true;
}

JoystickID GameController::device_id() const
{
  SDL_JoystickID dev_id = -1;

  SDL_Joystick* joy_dev = this->device();
  if( joy_dev != nullptr ) {
    dev_id = SDL_JoystickInstanceID(joy_dev);
  }

  if( dev_id < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return dev_id;
}

std::string GameController::name() const
{
  const char* dev_name = nullptr;
  std::string result;

  dev_name = SDL_GameControllerName( this->device_.get() );
  if( dev_name != nullptr ) {
    result = dev_name;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

bool GameController::open(JoystickIndex device_index)
{
  SDL_GameController* dev = SDL_GameControllerOpen(device_index);
  if( dev != nullptr ) {
    this->device_.reset(dev);

    // Success!
    return( this->attached() == true );
  } else {
    nom::set_error( SDL_GetError() );
    return false;
  }
}

void GameController::close()
{
  if( GameController::device_closed_ == true ) {
    return;
  }

  if( this->device_ != nullptr ) {
    SDL_GameControllerClose( this->device_.get() );
    GameController::device_closed_ = true;
  }
}

// static
std::string GameController::name(JoystickIndex device_index)
{
  const char* dev_name = nullptr;
  std::string result;

  dev_name = SDL_GameControllerNameForIndex(device_index);
  if( dev_name != nullptr ) {
    result = dev_name;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

// static
int GameController::set_event_state(int state)
{
  int result = SDL_GameControllerEventState(state);

  return result;
}

// static
void GameController::update()
{
  SDL_GameControllerUpdate();
}

std::string GameController::mapping_string()
{
  char* result_cstr = nullptr;
  std::string result;

  result_cstr = SDL_GameControllerMapping( this->device_.get() );
  if( result_cstr != nullptr ) {
    result = result_cstr;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

// static
std::string GameController::mapping_string(JoystickGUID guid)
{
  char* result_cstr = nullptr;
  std::string result;

  auto dev_guid = nom::convert_SDL_JoystickGUID(guid);

  result_cstr = SDL_GameControllerMappingForGUID(dev_guid);
  if( result_cstr != nullptr ) {
    result = result_cstr;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

// static
int GameController::load_mapping_memory(const char* buffer, int buffer_size)
{
  // int result = SDL_GameControllerAddMappingsFromRW();
  return -1;
  NOM_ASSERT_INVALID_PATH();
}

// static
int GameController::load_mapping_file(const std::string& filename)
{
  int result = -1;

  result = SDL_GameControllerAddMappingsFromFile( filename.c_str() );
  if( result < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

// static
int GameController::load_mapping_string(const std::string& mapping)
{
  int result = -1;

  result = SDL_GameControllerAddMapping( mapping.c_str() );
  if( result < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

// static
JoystickID GameController::device_id(JoystickIndex device_index)
{
  JoystickID dev_id = -1;

  Joystick jdev;
  if( jdev.open(device_index) == true ) {
    dev_id = jdev.device_id();
  }

  return dev_id;
}

// static
bool GameController::compatible_joystick(JoystickIndex device_index)
{
  SDL_bool result = SDL_IsGameController(device_index);
  if( result == SDL_FALSE ) {
    nom::set_error( SDL_GetError() );
    return false;
  }

  return true;
}

std::unique_ptr<GameController> make_unique_game_controller()
{
  auto dev = nom::make_unique<GameController>();

  return std::move(dev);
}

std::shared_ptr<GameController> make_shared_game_controller()
{
  auto dev = std::make_shared<GameController>();

  return dev;
}

} // namespace nom
