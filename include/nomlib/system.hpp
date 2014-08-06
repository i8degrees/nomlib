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
#ifndef NOMLIB_SYSTEM_HEADERS
#define NOMLIB_SYSTEM_HEADERS

// Public header file

#include <nomlib/config.hpp>
#include <nomlib/system/FPS.hpp>
#include <nomlib/system/StateMachine.hpp>
#include <nomlib/system/IState.hpp>
#include <nomlib/system/dialog_messagebox.hpp>
#include <nomlib/system/Path.hpp>
#include <nomlib/system/File.hpp>
#include <nomlib/system/SDLApp.hpp>
#include <nomlib/system/EventHandler.hpp>
#include <nomlib/system/Joystick.hpp>
#include <nomlib/system/Timer.hpp>
#include <nomlib/system/AnimationTimer.hpp>

// Engine initialization & shutdown
#include <nomlib/system/init.hpp>
#include <nomlib/system/PlatformSettings.hpp>

#include <nomlib/system/SDL_helpers.hpp>
#include <nomlib/system/Event.hpp>
#include <nomlib/system/EventCallback.hpp>
#include <nomlib/system/EventDispatcher.hpp>
#include <nomlib/system/InputMapper/InputAction.hpp>
#include <nomlib/system/InputMapper/InputStateMapper.hpp>
#include <nomlib/system/InputMapper/InputActionMapper.hpp>

// Resource management
#include <nomlib/system/resource_types.hpp>
#include <nomlib/system/resource_handlers.hpp>
#include <nomlib/system/ResourceFile.hpp>
#include <nomlib/system/ResourceCache.hpp>

#include <nomlib/system/ColorDatabase.hpp>

#endif // include guard defined
