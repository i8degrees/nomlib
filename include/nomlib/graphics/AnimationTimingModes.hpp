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

Easing algorithms are Copyright (c) 2001 Robert Penner

******************************************************************************/
#ifndef NOMLIB_GRAPHICS_ANIMATION_TIMING_MODES_HPP
#define NOMLIB_GRAPHICS_ANIMATION_TIMING_MODES_HPP

#include "nomlib/config.hpp"

namespace nom {

// Standard easing classes

/// \brief The standard Linear easing class.
///
/// \note This easing class uses a power of one (1).
struct Linear
{
  /// \brief Linear ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Linear ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Linear ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quad easing class.
///
/// \note This easing class uses a power of two (2).
struct Quad
{
  /// \brief Quad ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quad ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quad ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Cubic easing class.
///
/// \note This easing class uses a power of three (3).
struct Cubic
{
  /// \brief Cubic ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Cubic ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Cubic ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quart easing class.
///
/// \note This easing class uses a power of four (4).
struct Quart
{
  /// \brief Quart ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quart ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quart ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quint easing class -- also known as "Strong".
///
/// \note This easing class uses a power of five (5).
struct Quint
{
  /// \brief Quint ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quint ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Quint ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

// Additional easing classes

struct Back
{
  /// \brief Back ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Back ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Back ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Bounce
{
  /// \brief Bounce ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Bounce ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Bounce ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Circ
{
  /// \brief Circ ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Circ ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Circ ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Elastic
{
  /// \brief Elastic ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Elastic ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Elastic ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \note This easing class uses a power of ten (10).
struct Expo
{
  /// \brief Expo ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Expo ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Expo ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Sine
{
  /// \brief Sine ease in.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \brief Sine ease out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \brief Sine ease in and out.
  ///
  /// \param t elapsed time in milliseconds
  /// \param b initial value
  /// \param c total change in value -- applied over time.
  /// \param d duration in milliseconds
  ///
  /// \returns ...
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

} // namespace nom

#endif // include guard defined

/// \ingroup graphics/Animation
///
/// These easing functions derive from jesusgollonet's [GitHub repository](https://github.com/jesusgollonet/ofpennereasing). The easing algorithms are the work of [Robbert Penner](http://www.robertpenner.com/easing/).
///
/// ## References
///
/// \see [Visual graphing of each algorithm](http://easings.net/)
/// \see [Explaining Penner’s equations](http://upshots.org/actionscript/jsas-understanding-easing)
/// \see [Chapter 7 of Penner's book](http://robertpenner.com/easing/penner_chapter7_tweening.pdf)
///
/// ## Usage examples of my implementation
///
/// \see AnimationTest (unit tests), nom::IAnimationObject
///
