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
#ifndef NOMLIB_SYSTEM_HIGH_RESOLUTION_TIMER_HPP
#define NOMLIB_SYSTEM_HIGH_RESOLUTION_TIMER_HPP

#include "nomlib/config.hpp"

namespace nom {

/// \brief High-granularity time keeper
class HighResolutionTimer
{
  public:
    /// \brief Default constructor.
    HighResolutionTimer();

    /// \brief Destructor.
    ~HighResolutionTimer();

    bool started() const;
    bool paused() const;

    uint64 ticks() const;

    std::string ticks_str() const;

    void start();
    void stop();

    /// \brief Alias for start.
    void restart();

    void pause();
    void unpause();

    static real64 elapsed_ticks(uint64 start_time, uint64 end_time);

    /// \brief Convert a timing value from this interface to milliseconds.
    ///
    /// \param elapsed_ticks The timing value from ::ticks to convert.
    ///
    /// \see nom::hires_ticks
    static real64 to_milliseconds(uint64 elapsed_ticks);

    /// \brief Convert a timing value from this interface to seconds.
    ///
    /// \param elapsed_ticks The timing value from ::ticks to convert.
    ///
    /// \see nom::hires_ticks
    static real64 to_seconds(uint64 elapsed_ticks);

  private:
    /// \brief Activity status of the timer.
    bool paused_;

    /// \brief Running status of the timer.
    bool started_;

    /// \brief The recorded timestamp at the time of starting the timer.
    uint64 elapsed_ticks_;

    /// \brief The recorded timestamp at the time of pausing the timer.
    uint64 paused_ticks_;
};

} // namespace nom

#endif // include guard defined
