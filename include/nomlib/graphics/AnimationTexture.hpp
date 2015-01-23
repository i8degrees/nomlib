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
#ifndef NOMLIB_GRAPHICS_ANIMATION_TEXTURE_HPP
#define NOMLIB_GRAPHICS_ANIMATION_TEXTURE_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IActionObject.hpp"

namespace nom {

// Forward declarations
class Texture;

// TODO: Rename to TextureAction or so?

// TODO: Change type to nom::Sprite?
typedef std::vector<std::shared_ptr<Texture>> texture_frames;

/// \brief [TODO: Description]
class AnimationTexture: public virtual IActionObject
{
  public:
    typedef AnimationTexture self_type;
    typedef IActionObject derived_type;

    /// \brief Allow access into our private parts for obtaining the current
    /// frame.
    friend class AnimationTest;

    /// \brief Default constructor; construct an action with a single
    /// nom::Texture instance.
    ///
    /// \param frame_interval The amount of time (in fractional seconds) that
    /// each texture is displayed.
    AnimationTexture( const std::shared_ptr<Texture>& frame,
                      real32 frame_interval );

    /// \brief Destructor.
    virtual ~AnimationTexture();

    /// \brief Construct an action with a container of valid (i.e.: pre-loaded)
    /// nom::Texture objects.
    ///
    /// \param frame_interval The amount of time (in fractional seconds) that
    /// each texture is displayed.
    AnimationTexture(const texture_frames& textures, real32 frame_interval);

    virtual std::unique_ptr<derived_type> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;
    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    bool render(real32 delta_time) const;
    bool render(real32 delta_time, real64 angle) const;

    virtual void pause(real32 delta_time) override;

    /// \brief Resume logic for the animation object.
    ///
    /// \remarks Reserved for future implementation.
    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    void initialize(const texture_frames& textures, real32 frame_interval);
    void set_frame_interval(real32 seconds);

    typedef std::vector<std::shared_ptr<Texture>>::iterator frame_iterator;

    IActionObject::FrameState
    update(real32 t, real32 b, real32 c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    /// \brief The number of textures to animate.
    real32 total_displacement_;

    /// \brief The animation proxy object.
    texture_frames frames_;
    frame_iterator frame_iterator_;

    real32 frame_interval_;
    uint64 last_delta_;
};

} // namespace nom

#endif // include guard defined
