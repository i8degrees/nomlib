/******************************************************************************
    fps.cpp

  SDL-based Frames Per Second Implementation; inherits from the Timer Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "fps.h"

nom::FPS::FPS ( void )
{
  #ifdef DEBUG_TIMER_FPS_OBJ
    std::cout << "nom::FPS::FPS (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

nom::FPS::~FPS ( void )
{
  #ifdef DEBUG_TIMER_FPS_OBJ
    std::cout << "nom::FPS::~FPS (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->total_frames = 0;
  this->fps.Stop();
  this->fps_update.Stop();
}

void nom::FPS::Start ( void )
{
  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

void nom::FPS::Stop ( void )
{
  this->total_frames = 0;
  this->fps.Stop();
  this->fps_update.Stop();
}

unsigned int nom::FPS::getFrames ( void )
{
  return this->total_frames;
}

unsigned int nom::FPS::getFPS ( void )
{
  return this->getFrames() / ( this->fps.getTicks() / 1000.f );
}

void nom::FPS::Update ( void )
{
  this->total_frames++;
}
