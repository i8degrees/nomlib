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
#ifndef NOMLIB_GRAPHICS_COLORIZE_ACTION_HPP
#define NOMLIB_GRAPHICS_COLORIZE_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IActionObject.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

// Forward declarations
class Sprite;

/// \brief [TODO: Description]
class ColorizeAction: public virtual IActionObject
{
  public:
    typedef ColorizeAction self_type;
    typedef IActionObject derived_type;

    /// \brief Construct an action that animates an object's color and blend
    /// factor.
    ///
    /// \param delta The total change in color over time.
    ///
    /// \param duration The time, in fractional seconds, to play the action to
    /// completion.
    ColorizeAction( const std::shared_ptr<Sprite>& action, const Color4i& delta,
                    BlendMode mode, real32 duration );

    /// \brief Destructor.
    virtual ~ColorizeAction();

    /// \brief Create a deep copy of this instance.
    ///
    /// \remarks This is necessary anytime you wish to re-use an object that
    /// has been used once before, due to its state not being reset after the
    /// first use.
    virtual std::unique_ptr<derived_type> clone() const override;

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
    IActionObject::FrameState
    update(real32 t, const Color4i& b, const Color4i& c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    /// \brief The total change over time.
    const Color4i total_displacement_;

    /// \brief The starting color of the action for blending.
    Color4i initial_color_;

    /// \brief The color blending mode to apply to the action.
    BlendMode blend_mode_;

    /// \brief The sprite to animate.
    std::shared_ptr<Sprite> drawable_;
};

} // namespace nom

#endif // include guard defined
