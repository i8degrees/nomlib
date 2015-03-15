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
#ifndef NOMLIB_ACTIONS_FADE_IN_ACTION_HPP
#define NOMLIB_ACTIONS_FADE_IN_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"
#include "nomlib/math/Color4.hpp"

namespace nom {

// Forward declarations
class Sprite;

/// \brief Fade a sprite in to full opacity
class FadeInAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef FadeInAction self_type;
    typedef IActionObject derived_type;

    /// \brief Fade a sprite in to full opacity.
    ///
    /// \param drawable A valid nom::Sprite instance.
    /// \param seconds The duration of the animation.
    FadeInAction(const std::shared_ptr<Sprite>& drawable, real32 seconds);

    virtual ~FadeInAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    static const char* DEBUG_CLASS_NAME;

    IActionObject::FrameState update(real32 t, real32 b, real32 c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    const real32 total_displacement_ = Color4i::ALPHA_OPAQUE;
    real32 initial_alpha_ = 0.0f;

    std::shared_ptr<Sprite> drawable_;

    /// \brief The current alpha value of the drawable.
    uint8 alpha_ = 0;
};

} // namespace nom

#endif // include guard defined

/// \class nom::FadeInAction
/// \ingroup actions
///
/// \brief ...
///
/// \remarks This action is reversible; the reverse is created as if the
/// following code was executed:
///
/// \code
///   FadeOutAction(drawable, seconds);
/// \endcode
///
