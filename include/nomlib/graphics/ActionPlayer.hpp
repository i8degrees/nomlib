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
#include <map>
#include <functional>
#include <deque>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class IActionObject;
typedef std::function<void()> action_callback;

class DispatchQueue;

/// \brief High-level actions interface
class ActionPlayer
{
  public:
    typedef ActionPlayer self_type;

    enum State
    {
      RUNNING = 1,
      PAUSED,
      STOPPED,
      TOTAL_STATES
    };

    /// \brief nom::ActionPlayer::State enumerations as strings translation.
    static const char*
    ANIMATION_STATE_STR[ActionPlayer::State::TOTAL_STATES];

    /// \brief Default constructor.
    ///
    /// \remarks The default player state is initialized to
    /// nom::ActionPlayer::RUNNING.
    ActionPlayer();

    /// \brief Destructor.
    ~ActionPlayer();

    /// \brief Disabled copy constructor.
    ActionPlayer(const ActionPlayer& rhs) = delete;

    /// \brief Disabled copy assignment operator.
    ActionPlayer& operator =(const ActionPlayer& rhs) = delete;

    bool idle() const;

    /// \brief Get the number of actions enqueued.
    nom::size_type num_actions() const;

    /// \brief Obtain the player state.
    ///
    /// \returns The state of the player as one of the
    /// nom::ActionPlayer::State enumeration values.
    ActionPlayer::State player_state() const;

    /// \brief Set the player state.
    ///
    /// \param state One of the ActionPlayer::State enumeration values.
    void set_player_state(ActionPlayer::State state);

    /// \brief Convenience method for setting the player state to
    /// nom::ActionPlayer::State::PAUSED.
    ///
    /// \remarks When the player is paused, the animation loop continues, but
    /// with the altered state of using the animation object's pause method
    /// instead of its next_frame method. This typically results in a frame
    /// freeze.
    ///
    /// \see nom::IActionObject::resume, nom::IActionObject::pause
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
    /// \see nom::IActionObject::pause, nom::IActionObject::stop
    void resume();

    /// \brief Convenience method for setting the player state to
    /// nom::ActionPlayer::State::STOPPED.
    ///
    /// \remarks When the player is stopped, the animation loop continues, but
    /// uses the animation object's rewind method instead of its next_frame
    /// method. This typically results in resetting the animation cycle back
    /// to its starting position.
    ///
    /// \see nom::IActionObject::resume
    void stop();

    /// \brief Get an enqueued action by its name.
    ///
    /// \param key  A string that uniquely identifies the action.
    ///
    /// \returns A non-owned object pointer to the action on success, or NULL
    /// on failure, such as when a non-existing action key has been passed.
    IActionObject* action(const std::string& action_name);

    /// \brief Get the completion status of an action.
    ///
    /// \param action_name The action's name to query.
    ///
    /// \returns Boolean TRUE if the action has completed, and boolean FALSE
    /// if the action has **not** completed.
    ///
    /// \see IActionObject::set_name.
    bool action_running(const std::string& action_name) const;

    /// \brief Enqueue an action.
    ///
    /// \param action A valid (non-NULL), constructed action object.
    ///
    /// \see ActionPlayer::update
    bool run_action(const std::shared_ptr<IActionObject>& action);

    /// \brief Enqueue an action with an action identifier.
    ///
    /// \param action           A valid (non-NULL), constructed action object.
    /// \param action_name      A unique name used to identify the action.
    ///
    /// \remarks If an action using the same key is already running, it is
    /// removed before the new action is added.
    ///
    /// \see ActionPlayer::update, ActionPlayer::action_running
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const char* action_name );

    /// \brief Enqueue an action with a completion callback.
    ///
    /// \param action           A valid (non-NULL), constructed action object.
    /// \param completion_func  The function to execute when the action has
    /// completed.
    ///
    /// \see ActionPlayer::update
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const action_callback& completion_func );

    /// \brief Enqueue an action that runs on a specific dispatch queue.
    ///
    /// \param action           A valid (non-NULL), constructed action object.
    /// \param completion_func  The function to execute when the action has
    /// completed -- this value may be NULL.
    /// \param queue            The queue to perform the action on.
    ///
    /// \remarks This can be used to constrain or maximize the number of actions
    /// per dispatch queue.
    ///
    /// \see nom::DispatchQueue
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const action_callback& completion_func,
                      std::unique_ptr<DispatchQueue> queue );

    /// \brief Erase an action from the queue.
    ///
    /// \param action_name The action's name to erase.
    ///
    /// \see IActionObject::set_name.
    bool remove_action(const std::string& action_name);

    /// \brief Clear all actions from the queue.
    ///
    /// \remarks This will instantly stop all running actions.
    void remove_actions();

    bool update(real32 delta_time);

  private:
    typedef std::map<std::string, std::unique_ptr<DispatchQueue>> queue_type;

    typedef std::map<std::string, std::unique_ptr<DispatchQueue>>
    ::iterator queue_iterator;

    ActionPlayer::State player_state_;

    queue_type actions_;

    /// \brief The queue of actions pending removal.
    std::deque<queue_iterator> free_list_;
};

} // namespace nom

#endif // include guard defined
