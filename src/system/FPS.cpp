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
#include "nomlib/system/FPS.hpp"

namespace nom {

FPS::FPS ( void )
{
  this->total_frames = 0;
  this->fps_timer.start();
  this->fps_update_timer.start();
}

FPS::~FPS ( void ) {}

void FPS::start ( void )
{
  this->total_frames = 0;
  this->fps_timer.start();
  this->fps_update_timer.start();
}

void FPS::stop ( void )
{
  this->total_frames = 0;
  this->fps_timer.stop();
  this->fps_update_timer.stop();
}

uint32 FPS::frames ( void ) const
{
  return this->total_frames;
}

std::string FPS::asString ( void ) const
{
  return std::to_string( NOM_SCAST( uint32, this->fps_float() ) );
}

uint32 FPS::fps() const
{
  return( this->fps_float() );
}

float FPS::fps_float() const
{
  return this->frames() / (this->fps_timer.ticks() / 1000.f);
}

void FPS::update ( void )
{
  this->total_frames++;
}

} // namespace nom
