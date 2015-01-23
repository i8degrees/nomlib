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
#ifndef NOMLIB_GRAPHICS_WAIT_FOR_DURATION_ACTION_HPP
#define NOMLIB_GRAPHICS_WAIT_FOR_DURATION_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IActionObject.hpp"

namespace nom {

/// \brief [TODO: Description]
class WaitForDurationAction: public virtual IActionObject
{
  public:
    typedef WaitForDurationAction self_type;
    typedef IActionObject derived_type;

    /// \brief Default constructor.
    ///
    /// \param duration The time, in fractional seconds to idle (tick) for
    /// before completion of the object occurs.
    WaitForDurationAction(real32 duration);

    /// \brief Destructor.
    virtual ~WaitForDurationAction();

    virtual std::unique_ptr<derived_type> clone() const override;

    /// \brief Play the animation logic starting from the first frame to the
    /// last frame.
    ///
    /// \note This is internally called by nom::AnimationPlayer.
    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    /// \brief Play the animation logic in reverse -- starting from the last
    /// frame to the first frame.
    ///
    /// \note This is internally called by nom::AnimationReversed on each
    /// animation update cycle.
    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    /// \brief Reserved for future implementation.
    ///
    /// \note This is called internally called by nom::AnimationPlayer when the
    /// player state is set to nom::AnimationPlayer::PAUSED.
    virtual void pause(real32 delta_time) override;

    /// \brief Reserved for future implementation.
    virtual void resume(real32 delta_time) override;

    /// \brief Reset the object's state back to its first frame.
    ///
    /// \note This is called internally called by nom::AnimationPlayer when the
    /// player state is set to nom::AnimationPlayer::STOPPED.
    virtual void rewind(real32 delta_time) override;

    /// \brief Stubbed implementation (no object resources to free).
    ///
    /// \note This is called internally by nom::AnimationPlayer when the
    /// animation object is finished updating (signified by FrameState::DONE).
    virtual void release() override;
};

} // namespace nom

#endif // include guard defined
