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
#include "nomlib/actions/ReversedAction.hpp"

#include "nomlib/core/helpers.hpp"

namespace nom {

// Static initializations
const char* ReversedAction::DEBUG_CLASS_NAME = "[ReversedAction]:";

ReversedAction::ReversedAction( const std::shared_ptr<IActionObject>& action,
                                const std::string& name )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);
  this->action_ = action;
}

ReversedAction::~ReversedAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> ReversedAction::clone() const
{
  auto cloned_obj = nom::make_unique<self_type>( self_type(*this) );
  if( cloned_obj != nullptr ) {

    cloned_obj->status_ = FrameState::PLAYING;

    if( this->action_ != nullptr ) {
      cloned_obj->action_ = this->action_->clone();
    }

    // IMPORTANT: This is done to prevent the cloned action from being erased
    // from a running queue at the same time as the original instance!
    cloned_obj->set_name( "__" + this->name() + "_cloned" );

    return std::move(cloned_obj);
  } else {
    return nullptr;
  }
}

IActionObject::FrameState ReversedAction::next_frame(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->status_ = this->action_->prev_frame(delta_time);
  } else {
    // No action to reverse!
    this->status_ = FrameState::COMPLETED;
  }

  return this->status_;
}

IActionObject::FrameState ReversedAction::prev_frame(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->status_ = this->action_->next_frame(delta_time);
    return this->status_;
  } else {
    // No action to reverse!
    this->status_ = FrameState::COMPLETED;
  }

  return this->status_;
}

void ReversedAction::pause(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->action_->pause(delta_time);
  }
}

void ReversedAction::resume(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->action_->resume(delta_time);
  }
}

void ReversedAction::rewind(real32 delta_time)
{
  this->status_ = FrameState::PLAYING;

  if( this->action_ != nullptr ) {
    this->action_->rewind(delta_time);
  }
}

void ReversedAction::release()
{
  if( this->action_ != nullptr ) {
    this->action_->release();
  }

  this->action_.reset();
}

void ReversedAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  if( this->action_ != nullptr ) {
    this->action_->set_speed(speed);
  }
}

void
ReversedAction::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  if( this->action_ != nullptr ) {
    this->action_->set_timing_curve(mode);
  }
}

} // namespace nom
