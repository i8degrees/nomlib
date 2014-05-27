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
#ifndef NOMLIB_ANIMATED_SPRITE_HPP
#define NOMLIB_ANIMATED_SPRITE_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/graphics/sprite/SpriteBatch.hpp"
#include "nomlib/system/AnimationTimer.hpp"

//#define NOM_DEBUG_ANIMATED_SPRITE

namespace nom {

class AnimatedSprite:
                        public SpriteBatch
{
  public:
    enum AnimationStyle
    {
      NoStyle = 0,
      Blink,
      Oscillate
    };

    enum AnimationStatus
    {
      Stopped = 0,
      Playing,
      Paused
    };

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    AnimatedSprite ( void );

    /// Construct an AnimatedSprite object, initializing it with a SpriteSheet
    /// object.
    AnimatedSprite ( const SpriteSheet& sheet );

    /// Construct an AnimatedSprite from an existing filename saved with
    /// SpriteSheet.
    AnimatedSprite ( const std::string& filename );

    /// Destructor.
    virtual ~AnimatedSprite ( void );

    /// Get the maximum number of animation frames for this object
    int32 total_frames ( void ) const;

    /// Get the frame increment value of this object
    int32 frame_inc ( void ) const;

    /// Get the current frame of this object
    int32 frame ( void ) const;

    /// Get the current animation style for this object
    AnimatedSprite::AnimationStyle style ( void ) const;

    /// Get the current status of this animation
    AnimatedSprite::AnimationStatus status ( void ) const;

    /// Set a new frame rate for the animation
    ///
    /// The frame rate value needs to be in milliseconds.
    void setFrameRate ( int32 rate );

    /// Set the current frame for animation
    ///
    /// The frame you are setting must be greater than or equal to
    /// total_frames().
    void set_frame ( int32 frame );

    /// Set a new style of animation for this object
    void setAnimationStyle ( AnimatedSprite::AnimationStyle style );

    /// Play the animation
    void play ( void );

    /// Stop the animation playback.
    void stop ( void );

    /// Pause the animation playback.
    void pause ( void );

    /// Un-pause the animation playback.
    void unpause ( void );

  protected:
    /// Updates the playback of the animation
    void update_animation ( void );

    /// Initialize values to their respective defaults
    void initialize ( void );

    /// Set the total number of animated frames for this object
    void setMaxFrames ( int32 max );

    /// Set the increment value of each animated frame of this object
    void setFrameIncrement ( int32 increment );

    /// Set a new status state for this object
    void setAnimationStatus ( AnimatedSprite::AnimationStatus status );

    /// Total number of animation frames
    int32 max_frames;

    /// Current animation frame
    int32 current_frame;

    /// Value that we increment the current_frame by
    int32 frame_increment;

    /// Animation playback logic
    enum AnimatedSprite::AnimationStyle animation_style;

    /// Status info
    enum AnimatedSprite::AnimationStatus animation_status;

    /// Keeps record of our current tick and frame rate (think: frame delay)
    AnimationTimer fps;
};


} // namespace nom

#endif // include guard defined
