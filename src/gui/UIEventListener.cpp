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
#include "nomlib/gui/UIEventListener.hpp"

namespace nom {

UIEventListener::UIEventListener(const callback_type& observer)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  this->observer_ = observer;
}

UIEventListener::~UIEventListener()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::OnAttach( Rocket::Core::Element* element )
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::OnDetach( Rocket::Core::Element* element )
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void UIEventListener::ProcessEvent(event_type& event)
{
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "event", event.GetType().CString(), "for", event.GetTargetElement()->GetInnerRML().CString() );

  this->observer_.operator()(event);
}

} // namespace nom
