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
#ifndef NOMLIB_ACTIONS_SEQUENCE_ACTION_HPP
#define NOMLIB_ACTIONS_SEQUENCE_ACTION_HPP

#include <memory>
#include <deque>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"

namespace nom {

/// \brief Proxy action for running actions one at a time
class SequenceAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    friend class RepeatForAction;
    friend class RepeatForeverAction;

    typedef SequenceAction self_type;
    typedef IActionObject derived_type;

    /// \brief Default constructor.
    SequenceAction( const action_list& actions,
                    const std::string& name = "" );

    /// \brief Destructor.
    virtual ~SequenceAction();

    virtual std::unique_ptr<derived_type> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;
    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    /// \brief Resume logic for the animation object.
    ///
    /// \remarks Reserved for future implementation.
    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

    /// \brief Set the action's speed modifier.
    ///
    /// \remarks The speed modifier of every action is modified.
    virtual void set_speed(real32 speed) override;

    /// \brief Set the action's timing mode.
    ///
    /// \remarks The timing mode of every action is modified.
    virtual
    void set_timing_curve(const IActionObject::timing_curve_func& mode) override;

  private:
    typedef std::deque<std::shared_ptr<IActionObject>> container_type;

    const container_type& actions() const;

    /// \brief The enqueued actions.
    ///
    /// \remarks The stored actions are processed in a FIFO order.
    container_type actions_;

    /// \brief Iteration counter.
    nom::size_type itr_pos_;

    /// \brief Total number of actions at the time of construction.
    nom::size_type num_actions_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::SequenceAction
/// \ingroup graphics
///
/// \brief This is an action that acts on behalf of other action(s) as a proxy,
/// grouping actions together so that every action within its container is
/// executed to completion **one** at a time, in FIFO order. This is
/// particularly useful for transition-style animations, and is considered the
/// second fundamental animation logic flow type.
///
/// \remarks This action is not reversible, but the action(s) contained
/// within may be. Consult the documentation for each action type in question.
///
/// \see nom::WaitForDurationAction, nom::GroupAction
///
