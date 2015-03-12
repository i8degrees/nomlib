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
#ifndef NOMLIB_ACTIONS_ANIMATE_TEXTURES_ACTION_HPP
#define NOMLIB_ACTIONS_ANIMATE_TEXTURES_ACTION_HPP

#include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"

namespace nom {

// Forward declarations
class Texture;
class Sprite;

typedef std::vector<std::shared_ptr<Texture>> texture_frames;

/// \brief Animate changes to a sprite's texture
class AnimateTexturesAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef AnimateTexturesAction self_type;
    typedef IActionObject derived_type;

    /// \brief Animate changes to a sprite’s texture.
    ///
    /// \param drawable A valid nom::Sprite instance to use in rendering the
    /// frames to.
    ///
    /// \param textures A collection of textures to use to animate the sprite.
    /// sprite.
    ///
    /// \param frame_interval_seconds The duration (in seconds) that each
    /// texture is displayed.
    AnimateTexturesAction(  const std::shared_ptr<Sprite>& drawable,
                            const texture_frames& textures,
                            real32 frame_interval_seconds );

    virtual ~AnimateTexturesAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    static const char* DEBUG_CLASS_NAME;

    void initialize(const texture_frames& textures, real32 frame_interval);

    IActionObject::FrameState
    update(real32 t, real32 b, real32 c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    typedef texture_frames::iterator frame_iterator;

    /// \brief The total number of textures to animate.
    real32 total_displacement_;

    /// \brief The initial starting texture to animate from.
    uint32 initial_frame_;

    /// \brief The stored textures.
    texture_frames frames_;
    frame_iterator next_frame_;
    frame_iterator last_frame_;

    /// \brief The provided drawable used to render frames to.
    std::shared_ptr<Sprite> drawable_;

    /// \brief The delay (in seconds) before displaying the next texture.
    real32 frame_interval_;
    real32 last_delta_;
};

} // namespace nom

#endif // include guard defined
