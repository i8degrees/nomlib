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
#include <Rocket/Core.h>

#include "nomlib/librocket/RocketSDL2SystemInterface.hpp"

namespace nom {

RocketSDL2SystemInterface::~RocketSDL2SystemInterface()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void RocketSDL2SystemInterface::Release()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  delete this;
}

void RocketSDL2SystemInterface::ActivateKeyboard()
{
  SDL_StartTextInput();
}

void RocketSDL2SystemInterface::DeactivateKeyboard()
{
  SDL_StopTextInput();
}

float RocketSDL2SystemInterface::GetElapsedTime()
{
  return SDL_GetTicks() / 1000;
}

bool RocketSDL2SystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
  switch( type )
  {
    case Rocket::Core::Log::LT_ALWAYS:
    {
      NOM_LOG_DEBUG( NOM, message.CString() );
      break;
    }

    case Rocket::Core::Log::LT_ERROR:
    {
      NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, message.CString() );
      break;
    }

    // TODO: Take a look at libRocket.git/Samples/basic/customlog/src/main.cpp
    case Rocket::Core::Log::LT_ASSERT:
    {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_ASSERT, message.CString() );
      break;
    }

    case Rocket::Core::Log::LT_WARNING:
    {
      NOM_LOG_WARN( NOM_LOG_CATEGORY_GUI, message.CString() );
      break;
    }

    case Rocket::Core::Log::LT_INFO:
    {
      NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, message.CString() );
      break;
    }

    case Rocket::Core::Log::LT_DEBUG:
    {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_GUI, message.CString() );
      break;
    }

    case Rocket::Core::Log::LT_MAX: // ???
    {
      break;
    }
  }

  return true;
}

} // namespace nom

// #include "MouseButtonEventListener.h"
#include <Rocket/Core/Element.h>

namespace nom {

UIEventDispatcher::UIEventDispatcher()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );
}

UIEventDispatcher::~UIEventDispatcher()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );
}

bool
UIEventDispatcher::register_event_listener  (
                                              Rocket::Core::Element* element,
                                              const event_type& ev,
                                              callback_type* observer
                                            )
{
  // if( element == nullptr )
  // {
  //   // TODO: Err message
  //   return false;
  // }

  // if( observer == nullptr )
  // {
  //   // TODO: Err message
  //   return false;
  // }

  element->AddEventListener( ev.c_str(), observer, false );

  return true;
}

// bool
// UIEventDispatcher::remove_event_listener  (
//                                             Rocket::Core::Element* element,
//                                             const event_type& ev
//                                           )
// {
//   // element->RemoveEventListener( ev.c_str(), observer, false );

//   // TODO
//   return true;
// }

UIEventListener::UIEventListener( const callback_type& observer )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);

  this->observer_ = observer;
}

UIEventListener::~UIEventListener()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::OnAttach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::OnDetach( Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element) )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::ProcessEvent( event_type& event )
{
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "event", event.GetType().CString(), "for", event.GetTargetElement()->GetInnerRML().CString() );

  this->observer_.operator()( event );
}

} // namespace nom
