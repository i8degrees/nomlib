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
#ifndef NOMLIB_GRAPHICS_SCALE_BY_ACTION_HPP
#define NOMLIB_GRAPHICS_SCALE_BY_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IActionObject.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"

namespace nom {

// Forward declarations
class Sprite;

/// \brief [TODO: Description]
class ScaleByAction: public virtual IActionObject
{
  public:
    typedef ScaleByAction self_type;
    typedef IActionObject derived_type;

    /// \brief Construct an animation action using a scale factor.
    ///
    /// \param delta The width, height offsets to use for a scale factor.
    ScaleByAction(  const std::shared_ptr<Sprite>& obj, const Size2f& delta,
                    real32 duration );

    /// \brief Destructor.
    virtual ~ScaleByAction();

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

  private:
    /// \brief Execute the alpha blending logic for the animation.
    IActionObject::FrameState
    update(real32 t, const Size2i& b, const Size2f& c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    /// \brief The recorded scale value from frame displacement.
    Size2i size_;

    /// \brief The scale factor offset with respect to the duration.
    const Size2f total_displacement_;

    /// \brief The starting size dimensions of the set drawable.
    ///
    /// \remarks This is used in ::rewind.
    Size2i initial_size_;

    /// \brief The action to animate.
    std::shared_ptr<Sprite> drawable_;
};

} // namespace nom

#endif // include guard defined
