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
#include "nomlib/graphics/SequenceAction.hpp"

namespace nom {

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

IActionObject::FrameState SequenceAction::next_frame(real32 delta_time)
{
  IActionObject::FrameState action_status =
    FrameState::DONE;

  if( this->actions_.empty() == true ) {
    this->status_ = FrameState::DONE;
    return FrameState::DONE;
  }

  auto itr = this->actions_.begin();
  auto actions_end = this->actions_.end();
  NOM_ASSERT(itr != actions_end);

  NOM_ASSERT(*itr != nullptr);
  if( *itr != nullptr ) {
    action_status = (*itr)->next_frame(delta_time);
  }

  if( action_status == FrameState::DONE ) {

    std::string action_id = (*itr)->name();
    if( action_id == "" ) {
      action_id = "action";
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "[SequenceAction]:"
                    "removing", action_id, "from container",
                    "[", this->itr_pos_ + 1, "/", this->num_actions_, "]",
                    "[id]:", this->name() );

    this->actions_.pop_front();
    itr = this->actions_.begin();
    ++this->itr_pos_;
    this->status_ = FrameState::PLAY_NEXT_FRAME;

    if( this->actions_.empty() == true ) {
      this->status_ = FrameState::DONE;
      return this->status_;
    }
  }

  return this->status_;
}

IActionObject::FrameState SequenceAction::prev_frame(real32 delta_time)
{
  IActionObject::FrameState action_status =
    FrameState::DONE;

  if( this->actions_.empty() == true ) {
    this->status_ = FrameState::DONE;
    return FrameState::DONE;
  }

  auto itr = this->actions_.begin();
  auto actions_end = this->actions_.end();
  NOM_ASSERT(itr != actions_end);

  NOM_ASSERT(*itr != nullptr);
  if( *itr != nullptr ) {
    action_status = (*itr)->prev_frame(delta_time);
  }

  if( action_status == FrameState::DONE ) {

    std::string action_id = (*itr)->name();
    if( action_id == "" ) {
      action_id = "action";
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "[SequenceAction]:",
                    "removing", action_id, "from container",
                    "[", this->itr_pos_ + 1, "/", this->num_actions_, "]",
                    "[id]:", this->name() );

    this->actions_.pop_front();
    itr = this->actions_.begin();
    ++this->itr_pos_;
    this->status_ = FrameState::PLAY_NEXT_FRAME;

    if( this->actions_.empty() == true ) {
      this->status_ = FrameState::DONE;
      return this->status_;
    }
  }

  return this->status_;
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
