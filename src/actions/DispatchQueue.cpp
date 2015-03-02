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

// Forward declarations
#include "nomlib/actions/IActionObject.hpp"
#include "nomlib/actions/SequenceAction.hpp"
#include "nomlib/actions/GroupAction.hpp"
#include "nomlib/actions/ActionPlayer.hpp"

namespace nom {

/// Internal representation of the action queue
///
/// \see nom::DispatchQueue::enqueue_action
struct DispatchEnqueue
{
  /// \brief The stored action object.
  std::shared_ptr<IActionObject> action;

  /// \brief An optional delegate to call upon when the action is completed.
  action_callback on_completion_callback;

  /// \brief Diagnostic iterator counter.
  nom::size_type action_pos = 0;
};

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
  return( this->enqueued_actions_.size() );
}

IActionObject* DispatchQueue::action(nom::size_type index) const
{
  if( index <= this->enqueued_actions_.size() ) {
    return this->enqueued_actions_[index]->action.get();
  } else {
    // Err -- empty container!
    return nullptr;
  }
}

bool DispatchQueue::
enqueue_action( const std::shared_ptr<IActionObject>& action,
                const action_callback& completion_func )
{
  DispatchEnqueue enqueue;

  if( action == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not enqueue the action; action was NULL." );
    return false;
  }

  // FIXME?
  // Do not erase the actions the end-user passes to us; things like unit
  // testing and accessing action elements after deletion break otherwise!
  GroupAction* grp = NOM_DYN_PTR_CAST(GroupAction*, action.get() );
  SequenceAction* seq = NOM_DYN_PTR_CAST(SequenceAction*, action.get() );

  if( grp != nullptr ) {
    enqueue.action = action->clone();
  } else if( seq != nullptr ) {
    enqueue.action = action->clone();
  } else {
    // Non-proxy action objects do not need special handling
    enqueue.action = action;
  }

  NOM_ASSERT(enqueue.action != nullptr);
  if( enqueue.action == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not enqueue the action; failed cloning action." );
    return false;
  }

  enqueue.on_completion_callback = completion_func;

  auto queue_ptr =
    std::make_shared<DispatchEnqueue>(enqueue);
  this->enqueued_actions_.emplace_back(queue_ptr);

  this->queue_iterator_ = this->enqueued_actions_.begin();

  return true;
}

bool DispatchQueue::update(uint32 player_state, real32 delta_time)
{
  if( this->enqueued_actions_.empty() == true ) {
    // Nothing to update
    return false;
  }

  auto res = this->queue_iterator_;
  auto itr = (*res)->action;

  std::string action_id = (*res)->action->name();
  action_callback completion_func =
    (*res)->on_completion_callback;

  // Diagnostics
  nom::size_type* action_pos = &(*res)->action_pos;
  nom::size_type num_actions = this->enqueued_actions_.size();
  NOM_ASSERT(*action_pos >= 0);
  NOM_ASSERT(*action_pos < 3);
  NOM_ASSERT(num_actions == 1);

  IActionObject::FrameState action_status =
    IActionObject::FrameState::DONE;

  action_status = (itr)->next_frame(delta_time);

  // Handle the current animation with respect to the player's state
  if( action_status != IActionObject::FrameState::DONE ) {

    if( player_state == ActionPlayer::State::PAUSED ) {
      (itr)->pause(delta_time);
    } else if( player_state == ActionPlayer::State::STOPPED ) {
      (itr)->rewind(delta_time);
    } else {
      // FIXME: Temporary workaround?
      (itr)->resume(delta_time);

    } // ActionPlayer::State::RUNNING
  } // end if status != DONE

  // EOF -- handle internal clean up
  if( action_status == IActionObject::FrameState::DONE ) {

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE, "DispatchQueue [erasing]:",
                    "[", *action_pos + 1, "/", num_actions, "]",
                    "[id]:", action_id );

    // completed actions counter
    ++(*action_pos);

    NOM_ASSERT(*action_pos < 3);

    // Goodbye cruel world!
    this->enqueued_actions_.erase(res);

    // Holla back
    if( completion_func != nullptr ) {
      completion_func.operator()();
    }

    // Update iteration is finished
    return true;

  } // end if itr == FrameState::DONE

  // Update iteration is **not** finished
  return false;
}

} // namespace nom
