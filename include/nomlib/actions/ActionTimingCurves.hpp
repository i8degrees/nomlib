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
#ifndef NOMLIB_ACTIONS_ACTION_TIMING_CURVES_HPP
#define NOMLIB_ACTIONS_ACTION_TIMING_CURVES_HPP

#include "nomlib/config.hpp"

namespace nom {

// Standard easing classes

/// \brief The standard Linear easing class.
///
/// \note This easing class uses a power of one (1).
struct Linear
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quad easing class.
///
/// \note This easing class uses a power of two (2).
struct Quad
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Cubic easing class.
///
/// \note This easing class uses a power of three (3).
struct Cubic
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quart easing class.
///
/// \note This easing class uses a power of four (4).
struct Quart
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \brief The standard Quint easing class -- also known as "Strong".
///
/// \note This easing class uses a power of five (5).
struct Quint
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

// Additional easing classes

struct Back
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Bounce
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Circ
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Elastic
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

/// \note This easing class uses a power of ten (10).
struct Expo
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

struct Sine
{
  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_out(real32 t, real32 b, real32 c, real32 d);

  /// \param t The elapsed time.
  /// \param b The initial value to interpolate from.
  /// \param c The total change in value over time.
  /// \param d The total duration in time.
  ///
  /// \remarks The elapsed time and total duration time must be expressed in
  /// the same time units.
  static real32 ease_in_out(real32 t, real32 b, real32 c, real32 d);
};

} // namespace nom

#endif // include guard defined

/// \ingroup graphics
///
/// These easing functions derive from jesusgollonet's [GitHub repository](https://github.com/jesusgollonet/ofpennereasing).
/// The easing algorithms are the work of [Robbert Penner](http://www.robertpenner.com/easing/).
///
/// ## References
///
/// \see [Visual graphing of each algorithm](http://easings.net/)
/// \see [Explaining Penner’s equations](http://upshots.org/actionscript/jsas-understanding-easing)
/// \see [Chapter 7 of Penner's book](http://robertpenner.com/easing/penner_chapter7_tweening.pdf)
///
