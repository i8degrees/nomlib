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

#include <functional>

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

/// \see http://librocket.com/wiki/documentation/RML/Events
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Events
/// \see http://www.librocket.com/wiki/documentation/PythonManual/AttachingToEvents
class UIEventListener: public Rocket::Core::EventListener
{
  public:
    typedef UIEventListener self_type;

    typedef Rocket::Core::Event event_type;
    typedef std::function<void(event_type&)> callback_type;

    virtual ~UIEventListener();

    /// \brief Register an event listener for an element.
    ///
    /// \param event_type The event to listen for, i.e.: 'mouseup'.
    /// \param observer   The callback to execute inside ::ProcessEvent.
    void register_event_listener(
                                  Rocket::Core::Element* element,
                                  const std::string& event_type,
                                  const callback_type& observer
                                );

    // virtual void OnAttach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) );
    // virtual void OnDetach( Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) );

  protected:
    /// \brief Callback execution.
    ///
    /// \remarks The event's logic is done here.
    virtual void ProcessEvent( event_type& event );

  private:
    callback_type cb_;
};

} // namespace nom

#endif // include guard defined
