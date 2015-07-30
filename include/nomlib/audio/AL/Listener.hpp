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
#ifndef NOMLIB_AL_LISTENER_HEADERS
#define NOMLIB_AL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/IListener.hpp"

namespace nom {

/// \brief Global volume control
///
/// \remarks The use of this class is optional; the OpenAL API provides one by
/// default.
class Listener: public IListener
{
  public:
    Listener();
    virtual ~Listener();

    /// \brief Get the gain level of the current audio context.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 volume() const override;

    /// \brief Get the minimum gain level of the current audio context.
    ///
    /// \returns A number between 0..100 (min/max).
    static real32 min_volume();

    /// \brief Get the maximum gain level of the current audio context.
    ///
    /// \returns A number between 0..100 (min/max).
    static real32 max_volume();

    /// Obtain position
    Point3f position() const override;

    /// \brief Get the velocity of incoming volume.
    Point3f velocity() const override;

    /// Obtain direction
    Point3f direction() const override;

    /// Set position
    void set_position(const Point3f& position) override;

    /// Set velocity
    void set_velocity(const Point3f& velocity) override;

    /// Set direction
    void set_direction(const Point3f& direction, const Point3f& up) override;

    /// \brief Set a new master gain.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max) to
    /// represent the volume.
    ///
    /// \remarks This controls the "master" (global) volume level of the audio
    /// context.
    void set_volume(real32 gain) override;
};

} // namespace nom

#endif // NOMLIB_AL_LISTENER_HEADERS defined
