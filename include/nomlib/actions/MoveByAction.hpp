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

/// \brief Move a sprite relative to its current position
class MoveByAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef MoveByAction self_type;
    typedef IActionObject derived_type;

    /// \brief Move a sprite relative to its current position.
    ///
    /// \param drawable A valid nom::Sprite instance.
    /// \param delta The total change in position to apply to the drawable over
    /// time.
    /// \param seconds The duration of the animation.
    ///
    /// \remarks Negative delta values are valid.
    MoveByAction( const std::shared_ptr<Sprite>& drawable, const Point2i& delta,
                  real32 seconds );

    virtual ~MoveByAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    static const char* DEBUG_CLASS_NAME;

    IActionObject::FrameState
    update(real32 t, const Point2i& b, const Point2i& c, real32 d);

    void first_frame(real32 delta_time);

    void last_frame(real32 delta_time);

    const Point2i total_displacement_ = Point2i::zero;
    Point2i initial_position_ = Point2i::zero;

    std::shared_ptr<Sprite> drawable_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::MoveByAction
/// \ingroup actions
///
/// \brief ...
///
/// \remarks This action is reversible; the reverse is created as if the
/// following code was executed:
///
/// \code
///   MoveByAction(drawable, -delta, seconds);
/// \endcode
///
