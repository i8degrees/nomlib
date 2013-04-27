/******************************************************************************
    fps.cpp

    SDL-based Frames Per Second Implementation; inherits from the Timer Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "fps.h"

FPS::FPS ( void )
{
  #ifdef DEBUG_TIMER_FPS_OBJ
    std::cout << "FPS::FPS (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->total_frames = 0;
  this->fps.Start();
  this->fps_update.Start();
}

FPS::~FPS ( void )
{
  #ifdef DEBUG_TIMER_FPS_OBJ
    std::cout << "FPS::~FPS (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

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

unsigned int FPS::GetFrames ( void )
{
  return this->total_frames;
}

unsigned int FPS::GetFPS ( void )
{
  return this->GetFrames() / ( this->fps.GetTicks() / 1000.f );
}

void FPS::Update ( void )
{
  this->total_frames++;
}
