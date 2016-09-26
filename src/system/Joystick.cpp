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
#include "nomlib/system/Joystick.hpp"

// Private helpers
#include "nomlib/core/err.hpp"
#include "nomlib/core/unique_ptr.hpp"
#include "nomlib/system/SDL_helpers.hpp"

// Forward declarations
#include <SDL.h>

static_assert(  sizeof(SDL_JoystickGUID) == nom::GUID_MAX_LENGTH,
                "SDL_JoystickGUID struct mismatch" );

namespace nom {

bool init_joystick_subsystem()
{
  if( SDL_WasInit(SDL_INIT_JOYSTICK) == 0 ) {
    if( SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0 ) {
      nom::set_error( SDL_GetError() );
      return false;
    }
  }

  return true;
}

void shutdown_joystick_subsystem()
{
  if( SDL_WasInit(SDL_INIT_JOYSTICK) != 0 ) {
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }
}

void JoystickDeleter(SDL_Joystick* dev)
{
  if( dev != nullptr && SDL_JoystickGetAttached(dev) == true ) {
    SDL_JoystickClose(dev);
  }
}

Joystick::Joystick() :
  device_( joystick_dev(nullptr, JoystickDeleter) )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );
}

Joystick::~Joystick()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );
}

SDL_Joystick* Joystick::device() const
{
  return this->device_.get();
}

bool Joystick::attached() const
{
  SDL_bool result = SDL_JoystickGetAttached( this->device_.get() );
  if( result == SDL_FALSE ) {
    nom::set_error( SDL_GetError() );
    return false;
  }

  return true;
}

JoystickID Joystick::device_id() const
{
  SDL_JoystickID dev_id = SDL_JoystickInstanceID( this->device_.get() );
  if( dev_id < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return dev_id;
}

std::string Joystick::name() const
{
  const char* dev_name = nullptr;
  std::string result;

  dev_name = SDL_JoystickName( this->device_.get() );
  if( dev_name != nullptr ) {
    result = dev_name;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

bool Joystick::open(JoystickIndex device_index)
{
  SDL_Joystick* dev = SDL_JoystickOpen(device_index);
  if( dev != nullptr ) {
    this->device_.reset(dev);

    // Success!
    return( this->attached() == true );
  } else {
    nom::set_error( SDL_GetError() );
    return false;
  }
}

void Joystick::close()
{
  if( this->device_ != nullptr && this->attached() == true ) {
    SDL_JoystickClose( this->device_.get() );
  }
}

int Joystick::num_axes()
{
  int num_axes = SDL_JoystickNumAxes( this->device_.get() );
  if( num_axes < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return num_axes;
}

int Joystick::num_track_balls()
{
  int num_balls = SDL_JoystickNumBalls( this->device_.get() );
  if( num_balls < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return num_balls;
}

int Joystick::num_buttons()
{
  int num_buttons = SDL_JoystickNumButtons( this->device_.get() );
  if( num_buttons < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return num_buttons;
}

int Joystick::num_hats()
{
  int num_hats = SDL_JoystickNumHats( this->device_.get() );
  if( num_hats < 0 ) {
    nom::set_error( SDL_GetError() );
  }

  return num_hats;
}

// static
int Joystick::set_event_state(int state)
{
  int result = SDL_JoystickEventState(state);

  return result;
}

// static
void Joystick::update()
{
  SDL_JoystickUpdate();
}

// static
int Joystick::num_joysticks()
{
  int num_joysticks = SDL_NumJoysticks();

  return num_joysticks;
}

// static
std::string Joystick::name(JoystickIndex device_index)
{
  const char* dev_name = nullptr;
  std::string result;

  dev_name = SDL_JoystickNameForIndex(device_index);
  if( dev_name != nullptr ) {
    result = dev_name;
  } else {
    nom::set_error( SDL_GetError() );
  }

  return result;
}

JoystickGUID Joystick::device_guid() const
{
  JoystickGUID cloned_guid = {};

  if( this->attached() == true ) {

    SDL_JoystickGUID dev_guid = SDL_JoystickGetGUID( this->device_.get() );

    cloned_guid = nom::convert_SDL_JoystickGUID(dev_guid);
  }

  return cloned_guid;
}

// static
JoystickGUID Joystick::device_guid(JoystickIndex device_index)
{
  JoystickGUID cloned_guid = {};
  SDL_JoystickGUID dev_guid = SDL_JoystickGetDeviceGUID(device_index);

  cloned_guid = nom::convert_SDL_JoystickGUID(dev_guid);

  return cloned_guid;
}

// static
std::string Joystick::device_guid_string(JoystickGUID guid)
{
  std::string result;

  // NOTE: Match the GUID output string length in SDL2.hg/test/testjoystick.c
  char output_guid[64];

  SDL_JoystickGUID dev_guid = nom::convert_SDL_JoystickGUID(guid);

  SDL_JoystickGetGUIDString(dev_guid, output_guid, sizeof(output_guid) );

  result = output_guid;

  return result;
}

std::string Joystick::device_guid_string() const
{
  JoystickGUID dev_guid = {};
  std::string result;

  if( this->attached() == true ) {
    dev_guid = this->device_guid();

    result = Joystick::device_guid_string(dev_guid);

    if( result.length() < 1 ) {
      nom::set_error( SDL_GetError() );
    }
  }

  return result;
}

// static
JoystickID Joystick::device_id(JoystickIndex device_index)
{
  JoystickID dev_id = -1;

  Joystick jdev;
  if( jdev.open(device_index) == true ) {
    dev_id = jdev.device_id();
  }

  return dev_id;
}

std::unique_ptr<Joystick> make_unique_joystick()
{
  auto dev = nom::make_unique<Joystick>();

  return std::move(dev);
}

std::shared_ptr<Joystick> make_shared_joystick()
{
  auto dev = std::make_shared<Joystick>();

  return dev;
}

JoystickGUID convert_SDL_JoystickGUID(SDL_JoystickGUID dev_guid)
{
  JoystickGUID cloned_guid = {};

  std::memcpy(cloned_guid.data, dev_guid.data, sizeof(dev_guid.data) );

  return cloned_guid;
}

SDL_JoystickGUID convert_SDL_JoystickGUID(JoystickGUID dev_guid)
{
  SDL_JoystickGUID cloned_guid = {};

  std::memcpy(cloned_guid.data, dev_guid.data, sizeof(dev_guid.data) );

  return cloned_guid;
}

} // namespace nom
