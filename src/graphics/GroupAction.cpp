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
#include "nomlib/graphics/GroupAction.hpp"

namespace nom {

GroupAction::GroupAction( const action_list& actions,
                          const std::string& name )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);

  for( auto itr = actions.begin(); itr != actions.end(); ++itr ) {
    this->actions_.push_back(*itr);
  }

  this->itr_pos_ = 0;
  this->num_actions_ = this->actions_.size();
}

GroupAction::~GroupAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<GroupAction::derived_type> GroupAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState GroupAction::next_frame(real32 delta_time)
{
  if( this->actions_.empty() == true ) {
    NOM_LOG_DEBUG( NOM_LOG_CATEGORY_TEST, "GRP_DONE1");
    this->status_ = FrameState::DONE;
    return FrameState::DONE;
  }

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject::FrameState action_status =
      FrameState::DONE;

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      action_status = (*itr)->next_frame(delta_time);
    }

    if( action_status == FrameState::DONE ) {

      std::string action_id = (*itr)->name();
      NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE,
                      "GroupAction: releasing", (*itr)->name(),
                      "[", this->itr_pos_ + 1, "/", this->num_actions_, "]",
                      "[id]:", this->name() );

      // Erase the group action in the order in which it is completed
      itr = this->actions_.erase(itr);

      // Debug counter
      ++this->itr_pos_;

      this->status_ = FrameState::PLAY_NEXT_FRAME;

      if( itr == this->actions_.end() ) {
        break;
      }

      if( this->actions_.empty() == true ) {
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_TEST, "GRP_DONE2");
        this->status_ = FrameState::DONE;
        return this->status_;
      }
    }
  }

  return this->status_;
}

IActionObject::FrameState GroupAction::prev_frame(real32 delta_time)
{
  if( this->actions_.empty() == true ) {
    NOM_LOG_DEBUG( NOM_LOG_CATEGORY_TEST, "GRP_DONE1");
    this->status_ = FrameState::DONE;
    return FrameState::DONE;
  }

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject::FrameState action_status =
      FrameState::DONE;

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      action_status = (*itr)->prev_frame(delta_time);
    }

    if( action_status == FrameState::DONE ) {

      std::string action_id = (*itr)->name();
      NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE,
                      "GroupAction: releasing", (*itr)->name(),
                      "[", this->itr_pos_ + 1, "/", this->num_actions_, "]",
                      "[id]:", this->name() );

      // Erase the group action in the order in which it is completed
      itr = this->actions_.erase(itr);

      // Debug counter
      ++this->itr_pos_;

      this->status_ = FrameState::PLAY_NEXT_FRAME;

      if( itr == this->actions_.end() ) {
        break;
      }

      if( this->actions_.empty() == true ) {
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_TEST, "GRP_DONE2");
        this->status_ = FrameState::DONE;
        return this->status_;
      }
    }
  }

  return this->status_;
}

void GroupAction::pause(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->pause(delta_time);
    }
  } // end for loop
}

void GroupAction::resume(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->resume(delta_time);
    }
  } // end for loop
}

void GroupAction::rewind(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);

    // FIXME: We cannot rewind our objects until their memory store is released,
    // otherwise the memory never gets released!

    // FIXME: Memory stops being released when stopped or paused -- we want it
    // to occur regardless if the object is in the DONE frame state!
#if 0
    if( (*itr) != nullptr && (*itr)->status() == FrameState::DONE &&
        this->status() == FrameState::DONE )
    {
      (*itr)->rewind(delta_time);
    }
#else
    if( (*itr) != nullptr ) {
      (*itr)->rewind(delta_time);
    }
#endif
  } // end for loop
}

void GroupAction::release()
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->release();
    }
  } // end for loop
}

void GroupAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  // Propagate the speed modifier for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->set_speed(speed);
    }
  } // end for loop
}

void
GroupAction::set_timing_mode(const IActionObject::timing_mode_func& mode)
{
  IActionObject::set_timing_mode(mode);

  // Propagate the timing mode for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    NOM_ASSERT(*itr != nullptr);
    if( *itr != nullptr ) {
      (*itr)->set_timing_mode(mode);
    }
  } // end for loop
}

// Private scope

const GroupAction::container_type& GroupAction::actions() const
{
  return this->actions_;
}

} // namespace nom
