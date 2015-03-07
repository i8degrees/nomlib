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
#include "nomlib/actions/RepeatForAction.hpp"

namespace nom {

enum FrameStateDirection
{
  NEXT_FRAME,
  PREV_FRAME
};

RepeatForAction::RepeatForAction( const std::shared_ptr<IActionObject>& action,
                                  nom::size_type num_repeats,
                                  const std::string& name ) :
  num_repeats_(num_repeats)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);
  this->object_ = action;
  this->elapsed_repeats_ = 0;
}

RepeatForAction::~RepeatForAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<RepeatForAction::derived_type> RepeatForAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
RepeatForAction::update(real32 delta_time, uint32 direction)
{
  IActionObject::FrameState obj_status;
  IActionObject* action = this->object_.get();

  if( action == nullptr ) {
    // No proxy object to repeat for!
    this->status_ = FrameState::COMPLETED;
    return this->status_;
  }

  if( direction == FrameStateDirection::NEXT_FRAME ) {
    obj_status = action->next_frame(delta_time);
  } else {
    obj_status = action->prev_frame(delta_time);
  }

  if( obj_status == FrameState::COMPLETED ) {

    ++this->elapsed_repeats_;
    if( this->elapsed_repeats_ < this->num_repeats_ ) {
      action->rewind(delta_time);
    } else {
      NOM_ASSERT(this->num_repeats_ == this->elapsed_repeats_);
    }
  }

  NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "[RepeatForAction]",
                  "[elapsed_repeats]:", this->elapsed_repeats_,
                  "[num_repeats]:", this->num_repeats_ );

  if( this->elapsed_repeats_ == this->num_repeats_ ) {
    this->status_ = FrameState::COMPLETED;
  } else {
    this->status_ = FrameState::PLAYING;
  }

  return this->status_;
}

IActionObject::FrameState RepeatForAction::next_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::NEXT_FRAME);
}

IActionObject::FrameState RepeatForAction::prev_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::PREV_FRAME);
}

void RepeatForAction::pause(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->object_->pause(delta_time);
  }
}

void RepeatForAction::resume(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->object_->resume(delta_time);
  }
}

void RepeatForAction::rewind(real32 delta_time)
{
  // Reset our object back to its initial value
  this->elapsed_repeats_ = 0;

  if( this->object_ != nullptr ) {
    // Reset proxy object back to initial value
    this->object_->rewind(delta_time);
  }
}

void RepeatForAction::release()
{
  if( this->object_ != nullptr ) {
    this->object_->release();
  }

  this->object_.reset();
}

void RepeatForAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  // Propagate the speed modifier to our proxy / child object
  if( this->object_ != nullptr ) {
    this->object_->set_speed(speed);
  }
}

void
RepeatForAction::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  // Propagate the timing mode modifier to our proxy / child object
  if( this->object_ != nullptr ) {
    this->object_->set_timing_curve(mode);
  }
}

} // namespace nom
