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
#ifndef NOMLIB_GUI_ROCKET_SDL2_SYSTEM_INTERFACE_HPP
#define NOMLIB_GUI_ROCKET_SDL2_SYSTEM_INTERFACE_HPP

#include <functional>

#include "export.hpp"
#include "nomlib/config.hpp"

#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Input.h>

#include <SDL.h>

namespace nom {

/// \brief System interfacing bridge between libRocket and nomlib
///
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Input
class NOM_EXPORT RocketSDL2SystemInterface: public Rocket::Core::SystemInterface
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

#endif // include guard defined
