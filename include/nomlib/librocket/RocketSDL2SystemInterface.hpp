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

/// \brief System interfacing bridge between libRocket and nomlib
///
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Input
class RocketSDL2SystemInterface: public Rocket::Core::SystemInterface
{
  public:
    virtual ~RocketSDL2SystemInterface();

    /// \brief Implements Rocket::Core::SystemInterface::Release.
    ///
    /// \remarks Called when this file interface is released.
    virtual void Release();

    /// \brief Begin text input mode (brings up virtual keyboard on mobile hardware)
    ///
    /// \remarks Optional implementation of Rocket::Core::SystemInterface::
    /// ActivateKeyboard.
    virtual void ActivateKeyboard();

    /// \brief End text input mode (brings up virtual keyboard on mobile hardware)
    ///
    /// \remarks Optional implementation of Rocket::Core::SystemInterface::
    /// DeactivateKeyboard.
    virtual void DeactivateKeyboard();

    // Rocket::Core::Input::KeyIdentifier TranslateKey(SDL_Keycode sdlkey);
    // int TranslateMouseButton(Uint8 button);
    // int TranslateMouseWheel( int32 axis );
    // int GetKeyModifiers();

    /// \brief Implements Rocket::Core::SystemInterface::GetElapsedTime.
    ///
    /// \remarks Required implementation.
    virtual float GetElapsedTime();

    /// \brief Implements Rocket::Core::SystemInterface::LogMessage.
    ///
    /// \remarks Optional implementation.
    ///
    /// \todo Take a look at libRocket.git/Samples/basic/customlog/src/main.cpp
    /// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Interfaces#Thesysteminterface
    virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
};

} // namespace nom

#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Types.h>

namespace nom {

/// \brief Interface for an event listener for libRocket's platform.
///
/// \note Observer pattern
///
/// \note This object must outlive its element's destruction.
///
/// \see ~~nom::UIEventDispatcher~~
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Events
/// \see http://www.librocket.com/wiki/documentation/PythonManual/AttachingToEvents
class UIEventListener: public Rocket::Core::EventListener
{
  public:
    typedef UIEventListener self_type;
    typedef Rocket::Core::Event event_type;
    typedef std::function<void(event_type&)> callback_type;

    /// \brief Register an event listener for an element.
    ///
    /// \param observer The callback to be executed when the element has an
    /// event.
    UIEventListener( const callback_type& observer );

    /// \brief Destructor.
    virtual ~UIEventListener();

    /// \brief Not used.
    virtual void OnAttach( Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) );

    /// \brief Not used.
    virtual void OnDetach( Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) );

  protected:
    /// \brief The element's event handler.
    ///
    /// \remarks The end-user's callback function is executed here; this feature
    /// is provided by nomlib.
    ///
    /// \note This method is called by libRocket internally when there is an
    /// event for the element ready to be processed.
    virtual void ProcessEvent( event_type& event );

  private:
    callback_type observer_;
};

/// \brief High-level events manager
///
/// \remarks Bridge interface between libRocket and nomlib.
///
/// \todo Consider relocation or removal of this class; it's nothing more than
/// a pointless wrapper at the moment.
class UIEventDispatcher
{
  public:
    typedef UIEventDispatcher self_type;

    /// \brief ~~A raw pointer to the object's instance.~~
    // typedef Rocket::Core::Event event_type;
    typedef std::string event_type;

    /// \brief The data container type of the callback.
    ///
    /// \remarks Delegate execution is done via C++'s object operator method.
    typedef UIEventListener callback_type;

    /// \brief Default constructor.
    UIEventDispatcher();

    /// \brief Destructor.
    virtual ~UIEventDispatcher();

    /// \brief Register an event listener for an element.
    ///
    /// \param element  The document's element to listen on behalf of.
    /// \param ev       The event name to listen for, i.e.: 'mouseup'; must use
    /// libRocket event names.
    ///
    /// \see http://librocket.com/wiki/documentation/RML/Events
    bool register_event_listener  (
                                    Rocket::Core::Element* element,
                                    const event_type& ev,
                                    callback_type* observer
                                  );

    // bool remove_event_listener  (
    //                                 Rocket::Core::Element* element,
    //                                 const event_type& ev
    //                             );
};

} // namespace nom

#endif // include guard defined
