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
#include "nomlib/graphics/sprite/AnimatedSprite.hpp"

namespace nom {

void AnimatedSprite::initialize ( void )
{
  this->setMaxFrames ( 1 );
  this->setCurrentFrame ( 0 );
  this->setFrameIncrement ( 1 );
  this->setAnimationStyle ( AnimatedSprite::AnimationStyle::NoStyle );
  this->setAnimationStatus ( AnimatedSprite::AnimationStatus::Stopped );
  this->fps.setFrameRate ( 100 );
}

AnimatedSprite::AnimatedSprite ( void )
{
NOM_LOG_TRACE ( NOM );

  this->initialize();
}

AnimatedSprite::AnimatedSprite ( const SpriteSheet& sheet ) :
  SpriteBatch ( sheet )
{
NOM_LOG_TRACE ( NOM );

  this->initialize();

  this->setMaxFrames ( this->sprite_sheet.frames() - 1 ); // We start from zero
  this->fps.setFrameRate ( 100 );
}

AnimatedSprite::AnimatedSprite ( const std::string& filename ) :
  SpriteBatch ( filename )
{
NOM_LOG_TRACE ( NOM );

  this->initialize();

  this->setMaxFrames ( this->sprite_sheet.frames() - 1 ); // We start from zero
  this->fps.setFrameRate ( 100 );
}

AnimatedSprite::~AnimatedSprite ( void )
{
NOM_LOG_TRACE ( NOM );
}

int32 AnimatedSprite::total_frames ( void ) const
{
  return this->max_frames;
}

int32 AnimatedSprite::frame_inc ( void ) const
{
  return this->frame_increment;
}

int32 AnimatedSprite::frame ( void ) const
{
  return this->current_frame;
}

AnimatedSprite::AnimationStyle AnimatedSprite::style ( void ) const
{
  return this->animation_style;
}

AnimatedSprite::AnimationStatus AnimatedSprite::status ( void ) const
{
  return this->animation_status;
}

void AnimatedSprite::setMaxFrames ( int32 max )
{
  this->max_frames = max;
}

void AnimatedSprite::setFrameIncrement ( int32 increment )
{
  this->frame_increment = increment;
}

void AnimatedSprite::setFrameRate ( int32 rate )
{
  this->fps.setFrameRate ( rate );
}

void AnimatedSprite::setCurrentFrame ( int32 frame )
{
  if ( frame < 0 || frame > this->total_frames() )
  {
NOM_LOG_ERR ( NOM, "Could not update animation frame: requested frame value is too low or too high." );
    return;
  }

  this->current_frame = frame;
}

void AnimatedSprite::setAnimationStyle ( AnimatedSprite::AnimationStyle style )
{
  this->animation_style = style;
}

void AnimatedSprite::setAnimationStatus ( AnimatedSprite::AnimationStatus status )
{
  this->animation_status = status;
}

void AnimatedSprite::update ( void )
{
  if ( this->status() != AnimatedSprite::AnimationStatus::Playing ) return;

  if ( this->fps.ticks() + this->fps.framerate() > SDL_GetTicks() )
  {
    return;
  }

  this->fps.start();

  this->setCurrentFrame ( this->current_frame + this->frame_increment );

  if ( this->style() == AnimatedSprite::AnimationStyle::Oscillate )
  {
    if ( this->frame_inc() > 0 )
    {
      if ( this->frame() >= this->total_frames() )
      {
        this->setFrameIncrement ( -( this->frame_inc() ) );
      }
    }
    else
    {
      if ( this->frame() <= 0 )
      {
        this->setFrameIncrement ( -( this->frame_inc() ) );
      }
    }
  }
  else if ( this->style() == AnimatedSprite::AnimationStyle::Blink )
  {
    if ( this->frame() >= this->total_frames() )
    {

#if defined ( NOM_DEBUG_ANIMATED_SPRITE )
  NOM_DUMP_VAR( this->frame() );
#endif

      this->setCurrentFrame ( this->frame() - 2 );

#if defined ( NOM_DEBUG_ANIMATED_SPRITE )
  NOM_DUMP_VAR( this->frame() );
#endif

    }
  }
  else
  {
    if ( this->frame() >= this->total_frames() )
    {
      this->setCurrentFrame ( 0 );
    }
  }

  this->setSheetID ( this->frame() );
  //this->Update();
}

void AnimatedSprite::play ( void )
{
  this->setAnimationStatus ( AnimatedSprite::AnimationStatus::Playing );
  this->update();
}

void AnimatedSprite::stop ( void )
{
  this->setAnimationStatus ( AnimatedSprite::AnimationStatus::Stopped );
}

void AnimatedSprite::pause ( void )
{
  this->setAnimationStatus ( AnimatedSprite::AnimationStatus::Paused );
}

void AnimatedSprite::unpause ( void )
{
  this->setAnimationStatus ( AnimatedSprite::AnimationStatus::Playing );
}

} // namespace nom