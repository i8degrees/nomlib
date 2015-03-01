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
#include "nomlib/graphics/IActionObject.hpp"

// Private headers
#include "nomlib/graphics/ActionTimingCurves.hpp"

namespace nom {

IActionObject::IActionObject() :
  timing_curve_(nom::Linear::ease_in_out)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      NOM_LOG_PRIORITY_VERBOSE );
}

IActionObject::~IActionObject()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      NOM_LOG_PRIORITY_VERBOSE );
}

const std::string& IActionObject::name() const
{
  return this->name_;
}

real32 IActionObject::duration() const
{
  return this->duration_;
}

real32 IActionObject::speed() const
{
  return this->speed_;
}

const
IActionObject::timing_curve_func& IActionObject::timing_curve() const
{
  return this->timing_curve_;
}

void IActionObject::set_name(const std::string& action_id)
{
  this->name_ = action_id;
}

void IActionObject::set_speed(real32 speed)
{
  this->speed_ = speed;
}

void
IActionObject::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  this->timing_curve_ = mode;
}

// Protected scope

IActionObject::FrameState IActionObject::status() const
{
  return this->status_;
}

void IActionObject::set_duration(real32 seconds)
{
  this->duration_ = seconds * 1000.0f;
}

} // namespace nom
