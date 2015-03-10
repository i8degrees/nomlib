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
#ifndef NOMLIB_ACTIONS_MOVE_BY_ACTION_HPP
#define NOMLIB_ACTIONS_MOVE_BY_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"
#include "nomlib/math/Point2.hpp"

namespace nom {

// Forward declarations
class Sprite;

/// \brief [TODO: Description]
class MoveByAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for obtaining the current
    /// frame.
    friend class ActionTest;

    static const char* DEBUG_CLASS_NAME;

    typedef MoveByAction self_type;
    typedef IActionObject derived_type;

    /// \brief Construct an action that moves an object relative to its starting
    /// position.
    ///
    /// \param delta The total change (displacement) over time to translate to.
    ///
    /// \param duration The time, in fractional seconds, to play the animation
    /// to completion.
    ///
    /// \remarks Negative values are valid deltas for displacement.
    MoveByAction( const std::shared_ptr<Sprite>& action,
                  const Point2i& delta, real32 duration );

    /// \brief Destructor.
    virtual ~MoveByAction();

    /// \brief Create a deep copy of this instance.
    ///
    /// \remarks This is necessary anytime you wish to re-use an object that
    /// has been used once before, due to its state not being reset after the
    /// first use.
    virtual std::unique_ptr<IActionObject> clone() const override;

    /// \brief Displace the current animation frame.
    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    /// \brief Displace the previous animation frame (inverse of ::next_frame).
    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    /// \brief Frame-freeze the animation.
    virtual void pause(real32 delta_time) override;

    /// \brief Resume logic for the animation object.
    ///
    /// \remarks Reserved for future implementation.
    virtual void resume(real32 delta_time) override;

    /// \brief Reset state back to the first frame of the animation.
    ///
    /// \remarks This is used by nom::AnimationPlayer.
    virtual void rewind(real32 delta_time) override;

    /// \brief Free resources associated with this object.
    virtual void release() override;

  private:
    /// \brief Execute the displacement logic for the animation.
    IActionObject::FrameState
    update(real32 t, const Point2i& b, const Point2i& c, real32 d);

    /// \brief Initialize timer and initial position.
    void first_frame(real32 delta_time);

    /// \brief Clean up logic.
    void last_frame(real32 delta_time);

    /// \brief The total change (displacement) over time (duration).
    const Point2i total_displacement_;

    /// \brief The starting position of the animation for displacement from.
    Point2i initial_position_;

    /// \brief The texture to animate.
    std::shared_ptr<Sprite> drawable_;
};

} // namespace nom

#endif // include guard defined
