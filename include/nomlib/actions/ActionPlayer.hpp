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
#ifndef NOMLIB_ACTIONS_ACTION_PLAYER_HPP
#define NOMLIB_ACTIONS_ACTION_PLAYER_HPP

#include <memory>
#include <functional>
#include <map>
#include <deque>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class IActionObject;
class DispatchQueue;

typedef std::function<void()> action_callback_func;

/// \brief Interface for running and controlling action flow
class ActionPlayer
{
  public:
    typedef ActionPlayer self_type;

    /// \brief The status of the player.
    enum State
    {
      RUNNING = 0,
      PAUSED,
      STOPPED,
    };

    ActionPlayer();

    ~ActionPlayer();

    /// \brief Disabled copy constructor.
    ActionPlayer(const ActionPlayer& rhs) = delete;

    /// \brief Disabled copy assignment operator.
    ActionPlayer& operator =(const ActionPlayer& rhs) = delete;

    /// \brief Get the status of the action queue.
    ///
    /// \returns Boolean FALSE when one or more actions are running, and
    /// boolean TRUE when all actions have been completed and have been removed
    /// from the queue.
    bool idle() const;

    /// \brief Get the number of actions enqueued.
    nom::size_type num_actions() const;

    /// \brief Get the control status of the queue.
    ///
    /// \returns One of the nom::ActionPlayer::State enumeration values.
    ActionPlayer::State player_state() const;

    /// \brief Freeze the enqueued actions from advancing forward in time.
    ///
    /// \remarks Resuming from this control state will continue iterating the
    /// enqueued actions forward in time from where it last left off.
    ///
    /// \note The player state is set to nom::ActionPlayer::State::PAUSED.
    void pause();

    /// \brief Resume the advancement of time for the enqueued actions.
    ///
    /// \note The player state is set to ActionPlayer::State::RUNNING.
    ///
    /// \see nom::ActionPlayer::pause, nom::ActionPlayer::stop
    void resume();

    /// \brief Reset the enqueued actions back to its initial starting state.
    ///
    /// \remarks This resets the state of the enqueued actions back to its
    /// initial state, i.e.: before being executed. Resuming from this control
    /// state will restart the enqueued actions from their initial state.
    ///
    /// \note The player state is set to nom::ActionPlayer::State::STOPPED.
    void stop();

    /// \brief Get the completion status of an action.
    ///
    /// \param action_id The unique identifier of the action.
    ///
    /// \returns Boolean TRUE if the action has completed, and boolean FALSE
    /// if the action has **not** completed.
    ///
    /// \see nom::IActionObject::set_name.
    bool action_running(const std::string& action_id) const;

    /// \brief Stop executing an action.
    ///
    /// \param action_id The unique identifier of the action to stop.
    ///
    /// \see nom::IActionObject::set_name.
    bool cancel_action(const std::string& action_id);

    /// \brief Stop executing the enqueued actions.
    ///
    /// \remarks This will instantaneously stop running all enqueued actions.
    void cancel_actions();

    /// \brief Enqueue an action.
    ///
    /// \param action The action to run; NULL actions are valid.
    ///
    /// \remarks If an action using the same key is already running, it is
    /// removed before the new action is added.
    ///
    /// \see nom::ActionPlayer::update
    bool run_action(const std::shared_ptr<IActionObject>& action);

    /// \brief Enqueue an action with an action identifier.
    ///
    /// \param action The action to run; NULL actions are valid.
    ///
    /// \param action_id A unique identifier for the action.
    ///
    /// \remarks If an action using the same key is already running, it is
    /// removed before the new action is added.
    ///
    /// \see nom::ActionPlayer::update, nom::ActionPlayer::action_running
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const char* action_id );

    /// \brief Enqueue an action with a completion callback.
    ///
    /// \param action The action to run; NULL actions are valid.
    ///
    /// \param completion_func The function to call when the action is
    /// completed -- passing NULL here is valid.
    ///
    /// \remarks If an action using the same key is already running, it is
    /// removed before the new action is added.
    ///
    /// \see nom::ActionPlayer::update
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      const action_callback_func& completion_func );

    /// \brief Run the enqueued actions' update loop.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \returns Boolean TRUE when one or more actions are running, and boolean
    /// FALSE when all actions have been completed.
    ///
    /// \remarks This method must be called from your application's loop.
    ///
    /// \see nom::ActionPlayer::idle
    bool update(real32 delta_time);

  private:
    /// \brief Enqueue an action that runs on a specific dispatch queue.
    ///
    /// \param action The action to run; NULL actions are valid.
    ///
    /// \param completion_func The function to call when the action is
    /// completed -- passing NULL here is valid.
    ///
    /// \param dispatch_queue A valid nom::DispatchQueue to run the action on.
    ///
    /// \remarks It is **not** safe to enqueue more than one action per
    /// nom::DispatchQueue instance. When two or more actions share the same
    /// instance, it runs the risk of causing memory access violations in the
    /// form of double-freeing their dispatch queues upon completion (I believe
    /// this actually happens in ActionPlayer::update, but alas!). Using a
    /// reference counting mechanism, such as shared_ptr, resolves the issue,
    /// but if only that was the end of it ...
    ///   The update loop in nom::DispatchQueue iterates through actions
    /// sequentially, which breaks actions that are intended to run in parallel,
    /// i.e.: nom::GroupAction -- but even that problem is trivial to solve.
    /// The ~25..50% performance penalty observed made me decide that it was
    /// not worth the trouble, at least until I can find a use case for this
    /// issue.
    bool run_action(  const std::shared_ptr<IActionObject>& action,
                      std::unique_ptr<DispatchQueue> dispatch_queue,
                      const action_callback_func& completion_func );

    static const char* DEBUG_CLASS_NAME;

    typedef
    std::map<std::string, std::unique_ptr<DispatchQueue>> container_type;

    typedef container_type::iterator container_iterator;

    ActionPlayer::State player_state_;

    /// \brief Enqueued actions.
    container_type actions_;

    /// \brief The actions pending removal.
    std::deque<container_iterator> free_list_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::ActionPlayer
/// \ingroup actions
///
/// **TODO:** This documentation section is a *STUB*!
///
/// \brief This is the high-level interface for scheduling and controlling the
/// flow of actions. You may find that using multiple instances of the
/// interface is ideal if you have several action groups that can be managed
/// independently from each other.
///
/// ## Usage Examples
///
/// \code
///
/// // Declared somewhere accessible by the game
/// nom::ActionPlayer actions;
///
/// // Your main game loop
/// while(game_running == true)
/// {
///   // calculate delta_time for this frame
///
///   // ...Process game, input, etc. events
///
///   // ...Process game updates...
///
///   actions.update(delta_time);
///
///   // ...Process rendering...
/// }
///
/// \endcode
///
/// \snippet tests/src/actions/ActionTest.cpp usage_example
///
/// # References (Conceptual)
/// [SKNode](https://developer.apple.com/library/prerelease/ios/documentation/SpriteKit/Reference/SKNode_Ref/index.html#//apple_ref/occ/cl/SKNode)
///
