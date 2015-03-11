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
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"

namespace nom {

/// \brief Run a collection of actions sequentially
class SequenceAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef SequenceAction self_type;
    typedef IActionObject derived_type;

    /// \brief Run a collection of actions sequentially.
    ///
    /// \param actions The group of actions to execute; NULL actions are
    /// valid.
    /// \param name An optional unique identifier to assign to this instance.
    SequenceAction( const action_list& actions,
                    const std::string& name = "" );

    virtual ~SequenceAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

    /// \brief Set the speed factor of the child actions.
    ///
    /// \remarks This has no effect on the parent (this object).
    virtual void set_speed(real32 speed) override;

    /// \brief Set the timing mode of the child actions.
    ///
    /// \remarks This has no effect on the parent (this object).
    ///
    /// \see nom::IActionObject::timing_curve_func
    virtual void
    set_timing_curve(const IActionObject::timing_curve_func& mode) override;

  private:
    static const char* DEBUG_CLASS_NAME;

    IActionObject::FrameState
    update(real32 delta_time, uint32 direction);

    typedef std::vector<std::shared_ptr<IActionObject>> container_type;
    typedef container_type::iterator container_iterator;

    const container_type& actions() const;

    /// \brief The child actions.
    container_type actions_;
    container_iterator actions_iterator_;

    /// \brief The total number of completed actions.
    nom::size_type num_completed_;

    /// \brief The total number of actions at the time of construction.
    nom::size_type num_actions_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::SequenceAction
/// \ingroup actions
///
/// \brief This action acts on behalf of other actions as a proxy, running
/// actions to completion, one at a time, before iterating to the next
/// action, in FIFO order. The total duration of the sequence action is the sum
/// of every action's durations in the sequence.
///
/// \remarks This action is not reversible, but the actions contained within
/// may be. Consult the documentation for the action in question for
/// implementation details.
///
/// \note Sequence actions are particularly useful for transition-style
/// animations.
///
/// \see nom::WaitForDurationAction, nom::GroupAction
///
