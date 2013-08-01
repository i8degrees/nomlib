/******************************************************************************

    SDL-based Frames Per Second Implementation; inherits from the Timer Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/system/FPS.hpp"

namespace nom {

FPS::FPS ( void )
{
//NOM_LOG_CLASSINFO;

  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

FPS::~FPS ( void )
{
//NOM_LOG_CLASSINFO;

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
