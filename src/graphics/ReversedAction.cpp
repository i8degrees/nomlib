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
#include "nomlib/graphics/ReversedAction.hpp"

// Private headers
#include "nomlib/graphics/GroupAction.hpp"
#include "nomlib/graphics/SequenceAction.hpp"

namespace nom {

ReversedAction::ReversedAction( const std::shared_ptr<IActionObject>& action,
                                const std::string& name )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);
  this->object_ = action;
}

ReversedAction::~ReversedAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<ReversedAction::derived_type> ReversedAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState ReversedAction::next_frame(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->status_ = this->object_->prev_frame(delta_time);
    return this->status_;
  }

  // No proxy object to reverse
  this->status_ = FrameState::DONE;
  return this->status_;
}

IActionObject::FrameState ReversedAction::prev_frame(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->status_ = this->object_->next_frame(delta_time);
    return this->status_;
  }

  // No proxy object to reverse
  this->status_ = FrameState::DONE;
  return this->status_;
}

void ReversedAction::pause(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->object_->pause(delta_time);
  }
}

void ReversedAction::resume(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    this->object_->resume(delta_time);
  }
}

void ReversedAction::rewind(real32 delta_time)
{
  if( this->object_ != nullptr ) {
    // Reset proxy object back to initial starting values
    this->object_->rewind(delta_time);
  }
}

void ReversedAction::release()
{
  if( this->object_ != nullptr ) {
    this->object_->release();
  }
}

void ReversedAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  // Propagate the speed modifier to our proxy / child object
  if( this->object_ != nullptr ) {
    this->object_->set_speed(speed);
  }
}

void
ReversedAction::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  // Propagate the timing mode modifier to our proxy / child object
  if( this->object_ != nullptr ) {
    this->object_->set_timing_curve(mode);
  }
}

} // namespace nom
