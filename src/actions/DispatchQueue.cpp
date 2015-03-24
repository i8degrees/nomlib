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
#include "nomlib/actions/DispatchQueue.hpp"

#include "nomlib/core/helpers.hpp"

// Forward declarations
#include "nomlib/actions/IActionObject.hpp"
#include "nomlib/actions/ActionPlayer.hpp"

namespace nom {

// Internal representation of an enqueued action.
struct DispatchEnqueue
{
  // The enqueued action.
  std::shared_ptr<IActionObject> action;

  /// \brief The action's last known state in reference to the player's state.
  uint32 last_action_state = ActionPlayer::State::RUNNING;

  // An optional function pointer that is called when the action is completed.
  action_callback_func on_completion_callback;
};

// Static initializations
const char* DispatchQueue::DEBUG_CLASS_NAME = "[DispatchQueue]:";

DispatchQueue::DispatchQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
}

DispatchQueue::~DispatchQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
}

nom::size_type DispatchQueue::num_actions() const
{
  return this->num_actions_;
}

bool DispatchQueue::
enqueue_action( const std::shared_ptr<IActionObject>& action,
                const action_callback_func& completion_func )
{
  auto enqueued_action =
    nom::make_unique<DispatchEnqueue>();
  if( enqueued_action == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Failed to enqueue the action:",
                  "could not allocate memory for the dispatch entry!" );
    return false;
  }

  enqueued_action->action = action;
  enqueued_action->on_completion_callback = completion_func;

  this->actions_.emplace_back( std::move(enqueued_action) );
  this->actions_iterator_ = this->actions_.begin();
  ++this->num_actions_;

  return true;
}

DispatchQueue::State
DispatchQueue::update(uint32 player_state, real32 delta_time)
{
  auto itr = this->actions_iterator_;
  auto actions_end = this->actions_.end();

  if( itr == actions_end ) {
    // Finished updating; nothing left to do
    return State::IDLING;
  }

  auto action = (*itr)->action;
  if( action == nullptr ) {
    // Finished updating; nothing left to do
    return State::IDLING;
  }

  uint32 &last_action_state = (*itr)->last_action_state;

  IActionObject::FrameState action_status =
    IActionObject::FrameState::COMPLETED;

  action_status = action->next_frame(delta_time);

  // Handle the current action state with respect to the global player state
  if( action_status != IActionObject::FrameState::COMPLETED ) {

    if( player_state == ActionPlayer::State::PAUSED &&
        last_action_state != ActionPlayer::State::PAUSED )
    {
      action->pause(delta_time);
      last_action_state =
        ActionPlayer::State::PAUSED;
    } else if(  player_state == ActionPlayer::State::STOPPED &&
                last_action_state != ActionPlayer::State::STOPPED )
    {
      action->rewind(delta_time);
      last_action_state =
        ActionPlayer::State::STOPPED;
    } else if(  player_state == ActionPlayer::State::RUNNING &&
                last_action_state != ActionPlayer::State::RUNNING )
    {
      action->resume(delta_time);
      last_action_state =
        ActionPlayer::State::RUNNING;
    } // end if status != COMPLETED
  }

  // EOF -- handle internal clean up
  if( action_status == IActionObject::FrameState::COMPLETED ) {

    std::string action_id = action->name();
    action_callback_func completion_func =
      (*itr)->on_completion_callback;

    --this->num_actions_;
    ++this->actions_iterator_;

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE, DEBUG_CLASS_NAME,
                    "erasing:", action_id, "[remaining_actions]:",
                    this->num_actions_ );

    NOM_ASSERT(this->num_actions_ >= 0);

    // Holla back
    if( completion_func != nullptr ) {
      completion_func.operator()();
    }
  } // end if FrameState::COMPLETED

  if( this->actions_iterator_ == actions_end ) {
    // Finished update cycle
    return State::IDLING;
  } else {
    // Update cycle is **not** finished
    return State::RUNNING;
  }
}

} // namespace nom
