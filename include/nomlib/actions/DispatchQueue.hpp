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
#ifndef NOMLIB_ACTIONS_DISPATCH_QUEUE_HPP
#define NOMLIB_ACTIONS_DISPATCH_QUEUE_HPP

#include <memory>
#include <functional>
#include <deque>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class IActionObject;
typedef std::function<void()> action_callback;

// Forward declarations
struct DispatchEnqueue;

/// \brief Internal queue that drives each action's update logic
///
/// \see nom::ActionPlayer
class DispatchQueue
{
  public:
    typedef DispatchQueue self_type;

    DispatchQueue();
    ~DispatchQueue();

    /// \brief Disabled copy constructor.
    DispatchQueue(const DispatchQueue& rhs) = delete;

    /// \brief Disabled copy assignment operator.
    DispatchQueue& operator =(const DispatchQueue& rhs) = delete;

    /// \brief Get the current number of queued animation actions.
    nom::size_type num_actions() const;

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
    /// \see DispatchQueue::update
    bool enqueue_action(  const std::shared_ptr<IActionObject>& action,
                          const action_callback& completion_func );

    /// \brief Execute the animation loop.
    ///
    /// \param player_state One of the ActionPlayer::State enumeration values.
    ///
    /// \returns Boolean TRUE if the engine is playing enqueued animations, or
    /// boolean FALSE if the engine's animation queue is empty.
    ///
    /// \remarks This method must be implemented in the appropriate game loop.
    /// Applicable places include the main game loop or within a game state
    /// loop.
    bool update(uint32 player_state, real32 delta_time);

  private:
    typedef std::deque<std::shared_ptr<DispatchEnqueue>> container_type;

    /// \brief Enqueued actions.
    container_type actions_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::DispatchQueue
/// \ingroup graphics
///
/// \see nom::ActionPlayer
///
/// \internal
/// \see [Grand Central Dispatch (GCD) Reference](https://developer.apple.com/library/ios/documentation/Performance/Reference/GCD_libdispatch_Ref/index.html)
/// \endinternal
