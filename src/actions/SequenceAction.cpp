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
#include "nomlib/actions/SequenceAction.hpp"

namespace nom {

enum FrameStateDirection
{
  NEXT_FRAME,
  PREV_FRAME
};

// Static initializations
const char* SequenceAction::DEBUG_CLASS_NAME = "[SequenceAction]:";

SequenceAction::SequenceAction( const action_list& actions,
                                const std::string& name )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);

  for( auto itr = actions.begin(); itr != actions.end(); ++itr ) {
    this->actions_.push_back(*itr);
  }

  this->itr_pos_ = 0;
  this->num_actions_ = this->actions_.size();
  this->actions_iterator_ = this->actions_.begin();
}

SequenceAction::~SequenceAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<SequenceAction::derived_type> SequenceAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
SequenceAction::update(real32 delta_time, uint32 direction)
{
  IActionObject::FrameState action_status =
    FrameState::COMPLETED;

  // Program flow is structured to never call back here after the actions are
  // finished -- this serves only as a reminder to the intended flow.
  if( this->status_ == FrameState::COMPLETED ) {
    NOM_ASSERT_INVALID_PATH();
    return this->status_;
  }

  auto &itr = this->actions_iterator_;
  auto actions_end = this->actions_.end();
  NOM_ASSERT(itr != actions_end);

  NOM_ASSERT(*itr != nullptr);
  if( *itr != nullptr ) {
    if( direction == FrameStateDirection::NEXT_FRAME ) {
      action_status = (*itr)->next_frame(delta_time);
    } else {
      action_status = (*itr)->prev_frame(delta_time);
    }
  }

  if( action_status == FrameState::COMPLETED ) {

    std::string action_id;
    if( *itr != nullptr ) {
      action_id = (*itr)->name();
    }

    if( action_id == "" ) {
      action_id = "action";
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    action_id, "has finished",
                    "[", this->itr_pos_ + 1, "/", this->num_actions_, "]",
                    "[id]:", this->name() );

    ++itr;
    ++this->itr_pos_;
    this->status_ = FrameState::PLAYING;
  }

  if( this->itr_pos_ == this->num_actions_ ) {
    this->status_ = FrameState::COMPLETED;
    return this->status_;
  }

  return this->status_;
}

IActionObject::FrameState SequenceAction::next_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::NEXT_FRAME);
}

IActionObject::FrameState SequenceAction::prev_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::PREV_FRAME);
}

void SequenceAction::pause(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->pause(delta_time);
    }
  }
}

void SequenceAction::resume(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->resume(delta_time);
    }
  }
}

void SequenceAction::rewind(real32 delta_time)
{
  this->itr_pos_ = 0;
  this->actions_iterator_ = this->actions_.begin();

  // NOTE: Since this object type does not handle its frame state based on time
  // intervals like the other actions do, we must explicitly reset it here for
  // looping actions to work correctly.
  this->status_ = FrameState::PLAYING;

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->rewind(delta_time);
    }
  }
}

void SequenceAction::release()
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->release();
      (*itr).reset();
    }
  }
}

void SequenceAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  // Propagate the speed modifier for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->set_speed(speed);
    }
  }
}

void
SequenceAction::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  // Propagate the timing mode for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->set_timing_curve(mode);
    }
  }
}

// Private scope

const SequenceAction::container_type& SequenceAction::actions() const
{
  return this->actions_;
}

} // namespace nom
