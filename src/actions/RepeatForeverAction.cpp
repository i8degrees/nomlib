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
#include "nomlib/actions/RepeatForeverAction.hpp"

#include "nomlib/core/unique_ptr.hpp"

namespace nom {

enum FrameStateDirection
{
  NEXT_FRAME,
  PREV_FRAME
};

// Static initializations
const char* RepeatForeverAction::DEBUG_CLASS_NAME = "[RepeatForeverAction]:";

RepeatForeverAction::
RepeatForeverAction(const std::shared_ptr<IActionObject>& action)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->action_ = action;
  this->elapsed_repeats_ = 0;
}

RepeatForeverAction::~RepeatForeverAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> RepeatForeverAction::clone() const
{
  auto cloned_obj = nom::make_unique<self_type>( self_type(*this) );
  if( cloned_obj != nullptr ) {

    cloned_obj->set_status(FrameState::PLAYING);
    cloned_obj->elapsed_repeats_ = 0;

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

IActionObject::FrameState
RepeatForeverAction::update(real32 delta_time, uint32 direction)
{
  IActionObject::FrameState obj_status;
  IActionObject* action = this->action_.get();

  if( action == nullptr ) {
    // No action to repeat!
    this->set_status(FrameState::COMPLETED);
    return this->status();
  }

  if( direction == FrameStateDirection::NEXT_FRAME ) {
    obj_status = action->next_frame(delta_time);
  } else {
    obj_status = action->prev_frame(delta_time);
  }

  if( obj_status == FrameState::COMPLETED ) {

    ++this->elapsed_repeats_;
    action->rewind(delta_time);

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "[elapsed_repeats]:", this->elapsed_repeats_ );
  }

  this->set_status(FrameState::PLAYING);
  return this->status();
}

IActionObject::FrameState RepeatForeverAction::next_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::NEXT_FRAME);
}

IActionObject::FrameState RepeatForeverAction::prev_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::PREV_FRAME);
}

void RepeatForeverAction::pause(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->action_->pause(delta_time);
  }
}

void RepeatForeverAction::resume(real32 delta_time)
{
  if( this->action_ != nullptr ) {
    this->action_->resume(delta_time);
  }
}

void RepeatForeverAction::rewind(real32 delta_time)
{
  this->elapsed_repeats_ = 0;
  this->set_status(FrameState::PLAYING);

  if( this->action_ != nullptr ) {
    this->action_->rewind(delta_time);
  }
}

void RepeatForeverAction::release()
{
  if( this->action_ != nullptr ) {
    this->action_->release();
  }

  this->action_.reset();
}

void RepeatForeverAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  if( this->action_ != nullptr ) {
    this->action_->set_speed(speed);
  }
}

void RepeatForeverAction::
set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  if( this->action_ != nullptr ) {
    this->action_->set_timing_curve(mode);
  }
}

} // namespace nom
