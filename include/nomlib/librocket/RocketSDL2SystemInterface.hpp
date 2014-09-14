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
#ifndef NOMLIB_LIBROCKET_SDL2_SYSTEM_INTERFACE_HPP
#define NOMLIB_LIBROCKET_SDL2_SYSTEM_INTERFACE_HPP

//#include <memory>

#include "nomlib/config.hpp"

#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Input.h>

#include <SDL.h>

namespace nom {

class RocketSDL2SystemInterface: public Rocket::Core::SystemInterface
{
  public:
    Rocket::Core::Input::KeyIdentifier TranslateKey(SDL_Keycode sdlkey);
    int TranslateMouseButton(Uint8 button);
    int TranslateMouseWheel( int32 axis );
    int GetKeyModifiers();
    float GetElapsedTime();
    bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
};

} // namespace nom

#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Types.h>

namespace nom {

class MouseButtonEventListener: public Rocket::Core::EventListener
{
  public:
    virtual ~MouseButtonEventListener();

    /// \brief Registers an elemenet for the on mouse button click action event.
    static void RegisterEvent(Rocket::Core::Element* element);

    virtual void OnAttach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) );

  protected:
    /// \brief Event action logic
    virtual void ProcessEvent(Rocket::Core::Event& ev);
};

} // namespace nom

#endif // include guard defined
