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
#include "nomlib/actions/RemoveAction.hpp"

namespace nom {

RemoveAction::RemoveAction(const std::shared_ptr<IActionObject>& action)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->object_ = action;
}

RemoveAction::~RemoveAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<RemoveAction::derived_type> RemoveAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState RemoveAction::next_frame(real32 delta_time)
{
  if( this->object_ != nullptr ) {

    std::string action_id = this->object_->name();
    if( action_id == "" ) {
      action_id = "action";
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "[RemoveAction]:",
                    "removing", action_id, "from container",
                    "[id]:", this->name() );

    this->release();
    this->status_ = FrameState::DONE;
    return this->status_;
  }

  // No proxy action to remove
  this->status_ = FrameState::DONE;
  return this->status_;
}

IActionObject::FrameState RemoveAction::prev_frame(real32 delta_time)
{
  return this->next_frame(delta_time);
}

void RemoveAction::pause(real32 delta_time)
{
  // Not supported
}

void RemoveAction::resume(real32 delta_time)
{
  // Not supported
}

void RemoveAction::rewind(real32 delta_time)
{
  // Not supported
}

void RemoveAction::release()
{
  if( this->object_ != nullptr ) {
    this->object_->release();
  }

  this->object_.reset();
}

} // namespace nom
