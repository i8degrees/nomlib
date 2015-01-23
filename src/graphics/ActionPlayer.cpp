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
#include "nomlib/graphics/ActionPlayer.hpp"

// Private headers
// #include "nomlib/core/helpers.hpp"

// Forward declarations
#include "nomlib/graphics/IActionObject.hpp"
#include "nomlib/graphics/SequenceAction.hpp"
#include "nomlib/graphics/GroupAction.hpp"

namespace nom {

// A unique identifier that is auto-generated for actions without a given name.
static uint64 next_action_id_ = 0;

static uint64 generate_action_id()
{
  return( ++(next_action_id_) );
}

// Internal representation of the action queue
// TODO: Rename to ActionEnqueue
struct AnimationEnqueue
{
  /// \brief An object pointer to a container of animation actions.
  ///
  /// \see nom::ActionQueue::enqueue_action

  // TODO: Should this be a unique_ptr?
  std::shared_ptr<IActionObject> action;

  /// \brief The requested delegate to execute upon the completion of the
  /// animation action.
  ///
  /// \see nom::ActionQueue::enqueue_action
  ActionQueue::callback_type on_completion_callback;

  /// \brief Iterator counter.
  ///
  /// \remarks Used for diagnostics.
  // nom::size_type actions_pos = 0;

  /// \brief Total size of action container at the time of appending onto
  /// the queue.
  ///
  /// \remarks Used for diagnostics.
  // nom::size_type actions_size = 0;
};

// ActionPlayer

// Static initializations
const char*
ActionPlayer::ANIMATION_STATE_STR[ActionPlayer::State::TOTAL_STATES] = {
  NULL,
  "RUNNING",
  "PAUSED",
  "STOPPED"
};

ActionPlayer::ActionPlayer() :
  player_state_(ActionPlayer::State::RUNNING)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
}

ActionPlayer::~ActionPlayer()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
}

nom::size_type ActionPlayer::num_actions() const
{
  return this->actions_.size();
}

ActionPlayer::State ActionPlayer::player_state() const
{
  return this->player_state_;
}

void ActionPlayer::set_player_state(ActionPlayer::State state)
{
  this->player_state_ = state;
}

// void ActionPlayer::pause(const std::string& action_name)
// {
//   bool ret = false;

//   auto res = this->players_.find(action_name);

//   if( res == this->players_.end() ) {
//     // Err -- no action by that name was found
//   } else {
//     auto player = res->second.get();
//     NOM_ASSERT(player != nullptr);
//     if( player != nullptr ) {
//       // Success!
//       player->pause();
//       ret = true;
//     }
//   }

//   if( ret == true ) {
//     // this->set_player_state(ActionPlayer::State::PAUSED);
//   }
// }

// void ActionPlayer::resume(const std::string& action_name)
// {
//   bool ret = false;

//   auto res = this->players_.find(action_name);

//   if( res == this->players_.end() ) {
//     // Err -- no action by that name was found
//   } else {
//     auto player = res->second.get();
//     NOM_ASSERT(player != nullptr);
//     if( player != nullptr ) {
//       // Success!
//       player->resume();
//       ret = true;
//     }
//   }

//   if( ret == true ) {
//     // this->set_player_state(ActionPlayer::State::RUNNING);
//   }
// }

// void ActionPlayer::stop(const std::string& action_name)
// {
//   bool ret = false;

//   auto res = this->players_.find(action_name);

//   if( res == this->players_.end() ) {
//     // Err -- no action by that name was found
//   } else {
//     auto player = res->second.get();
//     NOM_ASSERT(player != nullptr);
//     if( player != nullptr ) {
//       // Success!
//       player->stop();
//       ret = true;
//     }
//   }

//   if( ret == true ) {
//     // this->set_player_state(ActionPlayer::State::STOPPED);
//   }
// }

void ActionPlayer::pause()
{
  // bool ret = false;

  // auto queue_end = this->actions_.end();
  // for( auto itr = this->actions_.begin(); itr != queue_end; ++itr ) {

  //   auto enqueue = itr->second.get();
  //   NOM_ASSERT(enqueue != nullptr);
  //   if( enqueue != nullptr ) {
  //     enqueue->pause();
  //     ret = true;
  //   }
  // }

  // if( ret == true ) {
    this->set_player_state(ActionPlayer::State::PAUSED);
  // }
}

void ActionPlayer::resume()
{
  // bool ret = false;

  // auto queue_end = this->actions_.end();
  // for( auto itr = this->actions_.begin(); itr != queue_end; ++itr ) {

  //   auto enqueue = itr->second.get();
  //   NOM_ASSERT(enqueue != nullptr);
  //   if( enqueue != nullptr ) {
  //     enqueue->resume();
  //     ret = true;
  //   }
  // }

  // if( ret == true ) {
    this->set_player_state(ActionPlayer::State::RUNNING);
  // }
}

void ActionPlayer::stop()
{
  // bool ret = false;

  // auto queue_end = this->actions_.end();
  // for( auto itr = this->actions_.begin(); itr != queue_end; ++itr ) {

  //   auto enqueue = itr->second.get();
  //   NOM_ASSERT(enqueue != nullptr);
  //   if( enqueue != nullptr ) {
  //     enqueue->stop();
  //     ret = true;
  //   }
  // }

  // if( ret == true ) {
    this->set_player_state(ActionPlayer::State::STOPPED);
  // }
}

bool ActionPlayer::action_running(const std::string& action_name) const
{
  auto res = this->actions_.find(action_name);

  if( res == this->actions_.end() ) {
    // The action is still running
    return true;
  } else {
    // The action is **not** running
    return false;
  }
}

void ActionPlayer::remove_actions()
{
  this->free_list_.clear();
  this->actions_.clear();
}

bool ActionPlayer::remove_action(const std::string& action_name)
{
  auto res = this->actions_.find(action_name);

  if( res == this->actions_.end() ) {
    // Err -- no action by that name found
    return false;
  } else {

    // Success -- action was found
    this->actions_.erase(res);

    return true;
  }

  // Err -- no action by that name found
  return false;
}

// TODO: Clean up action running checks
bool ActionPlayer::run_action(  const std::shared_ptr<IActionObject>& action,
                                const callback_type& completion_func,
                                std::unique_ptr<ActionQueue> queue )
{
  std::string action_id;

  NOM_ASSERT(action != nullptr);
  if( action == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not enqueue the action; action was NULL." );
    return false;
  }

  NOM_ASSERT(queue != nullptr);
  if( queue == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not enqueue the action; queue was NULL." );
    return false;
  }

  action_id = action->name();

  if( action_id.length() > 0 ) {
    // Use the action name assigned by the end-user for the unique identifier
  } else {
    uint64 id = nom::generate_action_id();
    NOM_ASSERT(id <= std::numeric_limits<uint64>::max() );
    action_id = std::to_string(id);
    action->set_name(action_id);
  }

  if( queue->enqueue_action(action, completion_func) == false ) {
    return false;
  }

  auto pair =
    std::make_pair(action_id, std::move(queue) );
  auto res = this->actions_.emplace( std::move(pair) );
  if( res.second == false ) {

    // Insertion failed; perhaps an existing entry caused the err?
    if( this->action_running(action_id) == false) {

      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Failed to enqueue action: a duplicate action key exists",
                    "for", action_id );
      return false;
    }

    // Let's try overwriting the existing action with the new one

    this->actions_.erase(res.first);
    auto pair =
      std::make_pair(action_id, std::move(queue) );
    auto res = this->actions_.emplace( std::move(pair) );
    if( res.second == false ) {
      // Unknown err; perhaps out of memory???
      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Could not enqueue the action with the given id:",
                    action_id );
      return false;
    }
  } // end if res.second == FALSE

  return true;
}

bool ActionPlayer::run_action(  const std::shared_ptr<IActionObject>& action,
                                const callback_type& completion_func )
{
  std::unique_ptr<ActionQueue> enqueue;
  enqueue.reset( new ActionQueue() );

  if( enqueue != nullptr ) {
    return this->run_action(action, completion_func, std::move(enqueue) );
  }

  std::string action_id;
  if( action != nullptr ) {
    action_id = action->name();
  }

  NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Failed to enqueue action: ",
                "could not allocate memory for the dispatch queue!\n",
                 "[action_id]:", action_id );
  return false;
}

bool ActionPlayer::run_action(const std::shared_ptr<IActionObject>& action)
{
  return this->run_action(action, nullptr);
}

bool ActionPlayer::update(real32 delta_time)
{
  bool running = true;
  ActionPlayer::State enqueue_state = ActionPlayer::State::RUNNING;

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    auto enqueue = itr->second.get();
    NOM_ASSERT(enqueue != nullptr);
    if( enqueue != nullptr ) {

      enqueue_state = this->player_state();
      running = (enqueue)->update(enqueue_state, delta_time);

      if( (itr->second)->num_actions() == 0 ) {

        NOM_LOG_VERBOSE(  NOM_LOG_CATEGORY_ACTION_PLAYER,
                          "ActionPlayer: enqueue erasable",
                          "[id]:", itr->first );
        this->free_list_.push_back(itr);
      } // end if num_actions == 0
    } // end if enqueue NOT NULL
  } // end for loop

  if( this->free_list_.empty() == false ) {

    while( this->free_list_.empty() == false ) {
      auto res = this->free_list_.front();

      NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_PLAYER,
                      "ActionPlayer: erasing action",
                      "[id]:", res->first );

      this->actions_.erase(res);
      this->free_list_.pop_front();
    } // end while free_list_ NOT empty
  } else if( running == false && this->free_list_.empty() == true ) {
    // return false;
    return running;
  }

  // return true;
  return running;
}

ActionQueue::ActionQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
}

ActionQueue::~ActionQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
}

nom::size_type ActionQueue::num_actions() const
{
  return( this->enqueue_actions_.size() );
}

void ActionQueue::remove_actions()
{
  this->enqueue_actions_.clear();
}

bool ActionQueue::enqueue_action( const std::shared_ptr<IActionObject>& action,
                                  const callback_type& completion_func )
{
  AnimationEnqueue enqueue;

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

  auto queue_ptr = std::make_shared<AnimationEnqueue>(enqueue);
  this->enqueue_actions_.emplace_back(queue_ptr);

  this->queue_iterator_ = this->enqueue_actions_.begin();

  return true;
}

// Called within the main loop
bool ActionQueue::update(ActionPlayer::State state, real32 delta_time)
{
  if( this->enqueue_actions_.empty() == true ) {
    // Nothing to update
    return false;
  }

  uint32 player_state = state;

  auto res = this->queue_iterator_;
  auto itr = (*res)->action;

  std::string action_id = (*res)->action->name();
  callback_type completion_func = (*res)->on_completion_callback;
  // nom::size_type* pos = &(*res)->actions_pos;
  // nom::size_type actions_size = (*res)->actions_size;

  // Extended debugging diagnostics
  const char* player_state_str =
    ActionPlayer::ANIMATION_STATE_STR[player_state];

  NOM_LOG_VERBOSE(  NOM_LOG_CATEGORY_ACTION_QUEUE,
                    "[id]:", action_id,
                    "[state]:", player_state_str );

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
      // ActionPlayer::State::RUNNING

      // FIXME: Temporary workaround?
      (itr)->resume(delta_time);
    }
  } // end if status != DONE

  // End of animation object; handle cleaning up resources and advancing
  // onwards to the next in queue
  if( action_status == IActionObject::FrameState::DONE ) {

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE,
                    "ActionQueue: releasing...",
                    //"[", *pos + 1, "/", actions_size, "]",
                    "[id]:", action_id );

    // Advance the internal count of completed animation objects
    // ++(*pos);

    // Pop the animation action from the queue only when **every** object in
    // the group has finished playing

    // Debugging diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE,
                    "ActionQueue: erasing [id]:", action_id );

    // Goodbye cruel world!
    this->enqueue_actions_.erase(res);

    // Done executing the enqueued group action -- holla back to the API
    // user!
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
