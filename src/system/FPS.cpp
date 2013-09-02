/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
//NOM_LOG_TRACE;

  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

FPS::~FPS ( void )
{
//NOM_LOG_TRACE;

  this->total_frames = 0;
  this->fps.Stop();
  this->fps_update.Stop();
}

void FPS::Start ( void )
{
  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

void FPS::Stop ( void )
{
  this->total_frames = 0;
  this->fps.Stop();
  this->fps_update.Stop();
}

unsigned int FPS::getFrames ( void )
{
  return this->total_frames;
}

unsigned int FPS::getFPS ( void )
{
  return this->getFrames() / ( this->fps.getTicks() / 1000.f );
}

void FPS::Update ( void )
{
  this->total_frames++;
}


} // namespace nom
