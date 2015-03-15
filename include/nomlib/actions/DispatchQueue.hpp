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
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/core/helpers.hpp"

namespace nom {

// Forward declarations
class IActionObject;
struct DispatchEnqueue;

typedef std::function<void()> action_callback_func;

/// \brief The internal processing queue for actions
class DispatchQueue
{
  public:
    typedef DispatchQueue self_type;

    /// \brief The status of the queue.
    enum State
    {
      IDLING = 0,
      RUNNING,
    };

    DispatchQueue();

    ~DispatchQueue();

    /// \brief Get the total number of enqueued actions.
    nom::size_type num_actions() const;

    /// \brief Append an action to the list of actions to be executed.
    ///
    /// \param action The action to run; NULL actions are valid.
    /// \param completion_func An optional function to call when the action is
    /// completed -- passing NULL here will ignore this parameter.
    bool enqueue_action(  const std::shared_ptr<IActionObject>& action,
                          const action_callback_func& completion_func );

    /// \brief Run the enqueued actions' update loop.
    ///
    /// \param player_state One of the ActionPlayer::State enumeration values.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \returns DispatchQueue::State::RUNNING if one or more actions are
    /// executing, or DispatchQueue::State::IDLING when no actions are running,
    /// such as when the queue is empty.
    ///
    /// \fixme Pausing the action does not always take effect for every
    /// nom::MoveByAction in the group within
    /// ActionTest.RainingRectsStressTest -- some actions are sometimes skipped
    /// for some mysterious reason!
    ///
    /// \see nom::ActionPlayer::update.
    DispatchQueue::State
    update(uint32 player_state, real32 delta_time);

  private:
    static const char* DEBUG_CLASS_NAME;

    typedef std::vector<std::unique_ptr<DispatchEnqueue>> container_type;
    typedef container_type::iterator iterator_type;

    /// \brief Enqueued actions.
    container_type actions_;
    iterator_type actions_iterator_;

    /// \brief The total number of actions enqueued.
    nom::size_type num_actions_ = 0;
};

/// \brief Constructor function for creating a nom::DispatchQueue.
///
/// \param args The arguments to pass to the constructed object.
///
/// \relates nom::DispatchQueue
template<typename ObjectType, typename... ObjectArgs>
std::unique_ptr<ObjectType> create_dispatch_queue(ObjectArgs&&... args)
{
  auto dispatch_queue =
    nom::make_unique<ObjectType>( std::forward<ObjectArgs>(args) ... );

  return std::move(dispatch_queue);
}

} // namespace nom

#endif // include guard defined

/// \class nom::DispatchQueue
/// \ingroup actions
///
/// \brief This interface was inspired by Apple's [Grand Central Dispatch (GCD)](https://developer.apple.com/library/ios/documentation/Performance/Reference/GCD_libdispatch_Ref/index.html).
///
/// \see nom::ActionPlayer
///
