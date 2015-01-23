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
#ifndef NOMLIB_GRAPHICS_ACTION_PLAYER_HPP
#define NOMLIB_GRAPHICS_ACTION_PLAYER_HPP

#include <memory>
#include <string>
#include <map>
#include <functional>
#include <deque>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class IActionObject;
struct AnimationEnqueue;
class ActionQueue;

// TODO: Remove?
class GroupAction;
class SequenceAction;

class ActionPlayer
{
  public:
    typedef ActionPlayer self_type;
    typedef std::function<void()> callback_type;

    enum State
    {
      RUNNING = 1,
      PAUSED,
      STOPPED,
      TOTAL_STATES
    };

    /// \brief nom::ActionPlayer::State enumerations as strings translation.
    ///
    /// \remarks Used internally by nom::ActionQueue for debugging clarity.
    static const char*
    ANIMATION_STATE_STR[ActionPlayer::State::TOTAL_STATES];

    ActionPlayer();

    /// \brief Destructor.
    ~ActionPlayer();

    nom::size_type num_actions() const;

    /// \brief Obtain the player state.
    ///
    /// \returns The state of the player as one of the
    /// nom::ActionPlayer::State enumeration values.
    ActionPlayer::State player_state() const;

    /// \brief Get the completion status of an action.
    ///
    /// \param action_name The action's name to query.
    ///
    /// \returns Boolean TRUE if the action has completed, and boolean FALSE
    /// if the action has **not** completed.
    ///
    /// \see IActionObject::set_name.
    bool action_running(const std::string& action_name) const;

    /// \brief Set the player state.
    ///
    /// \param state One of the ActionPlayer::State enumeration values.
    void set_player_state(ActionPlayer::State state);

    // void pause(const std::string& action_name);
    // void resume(const std::string& action_name);
    // void stop(const std::string& action_name);

    /// \brief Convenience method for setting the player state to
    /// nom::ActionPlayer::State::PAUSED.
    ///
    /// \remarks When the player is paused, the animation loop continues, but
    /// with the altered state of using the animation object's pause method
    /// instead of its next_frame method. This typically results in a frame
    /// freeze.
    ///
    /// \see nom::ActionQueue::resume, nom::IActionObject::pause
    void pause();

    /// \brief Convenience method for setting the player state to
    /// nom::ActionPlayer::State::RUNNING.
    ///
    /// \remarks This continues the animation player's animation cycle loop,
    /// from a paused or stopped state. When the previous state was
    /// nom::ActionPlayer::State::PAUSED, continuing means restoring the
    /// player state from where it was iterating from before pausing. When the
    /// previous state was nom::ActionPlayer::State::STOPPED, continuing
    /// means restoring to the initial (first) animation frame from where it
    /// was iterating from before stopping. In other words, this functionality
    /// is intended to match a video player.
    ///
    /// \see nom::ActionQueue::pause, nom::ActionQueue::stop
    void resume();

    /// \brief Convenience method for setting the player state to
    /// nom::ActionPlayer::State::STOPPED.
    ///
    /// \remarks When the player is stopped, the animation loop continues, but
    /// uses the animation object's rewind method instead of its next_frame
    /// method. This typically results in resetting the animation cycle back
    /// to its starting position.
    ///
    /// \see nom::ActionQueue::resume
    void stop();

    void remove_actions();

    /// \brief Erase an action from the queue.
    ///
    /// \param action_name The action's name to erase.
    ///
    /// \see IActionObject::set_name.
    bool remove_action(const std::string& action_name);

    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const callback_type& completion_func,
                      std::unique_ptr<ActionQueue> queue );

    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const callback_type& completion_func );

    bool run_action(const std::shared_ptr<IActionObject>& action);

    bool update(real32 delta_time);

  private:
    typedef std::map<std::string, std::unique_ptr<ActionQueue>> queue_type;
    typedef std::map<std::string, std::unique_ptr<ActionQueue>>::iterator queue_iterator;

    queue_type actions_;
    std::deque<queue_iterator> free_list_;

    // TODO: Rename to action_state_?
    ActionPlayer::State player_state_;
};

/// \brief [TODO: Description]
class ActionQueue
{
  public:
    /// \brief Allow access into our private parts for diagnostics and
    /// potentially mutation.
    typedef ActionQueue self_type;
    typedef std::function<void()> callback_type;

    /// \brief Default constructor.
    ///
    /// \remarks The default player state is initialized to
    /// nom::ActionQueue::RUNNING.
    ActionQueue();

    /// \brief Destructor.
    ~ActionQueue();

    /// \brief Get the current number of queued animation actions.
    nom::size_type num_actions() const;

    /// \brief Clear all actions from the animation queue.
    ///
    /// \remarks This effectively stops the animation loop by clearing its
    /// internal queue of animations to play in its run loop. You will need to
    /// enqueue new animation objects after this call is made.
    void remove_actions();

    /// \brief Append an action to the list of animations to be executed.
    ///
    /// \param action             A deep copy of the action is made.
    /// \param on_completion_func The function to call back when the action is
    /// completed (removed from the animation player's queue).
    ///
    /// \remarks Chaining callbacks together to run another animation action
    /// in any arrangement should be safe, and is part of the design of this
    /// interface.
    ///
    /// \see ActionQueue::update
    bool enqueue_action(  const std::shared_ptr<IActionObject>& action,
                          const callback_type& completion_func );

    /// \brief Execute the animation loop.
    ///
    /// \returns Boolean TRUE if the engine is playing enqueued animations, or
    /// boolean FALSE if the engine's animation queue is empty.
    ///
    /// \remarks This method must be implemented in the appropriate game loop.
    /// Applicable places include the main game loop or within a game state
    /// loop.
    bool update(ActionPlayer::State state, real32 delta_time);

  private:
    /// \brief Play each of the layer groups contained stored by the ID
    /// **simultaneously** (in parallel).

    /// \brief Play each of the layer groups contained stored by the ID **one**
    /// at a time.

    // TODO: Should this be a unique_ptr?
    typedef std::deque<std::shared_ptr<AnimationEnqueue>> queue_type;
    typedef std::deque<std::shared_ptr<AnimationEnqueue>>::iterator queue_iterator;

    /// \brief Enqueued action objects.
    queue_type enqueue_actions_;

    /// \brief Enqueued actions iterator state.
    queue_iterator queue_iterator_;
};

} // namespace nom

#endif // include guard defined
