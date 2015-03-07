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
#ifndef NOMLIB_ACTIONS_IACTION_OBJECT_HPP
#define NOMLIB_ACTIONS_IACTION_OBJECT_HPP

#include <functional>

#include "nomlib/config.hpp"
#include "nomlib/system/Timer.hpp"

namespace nom {

/// \brief Base interface class for action objects.
class IActionObject
{
  public:
    typedef IActionObject self_type;

    typedef std::function<void()> action_callback;
    typedef std::function<real32(real32, real32, real32, real32)> timing_curve_func;

    /// \brief The animation object's state.
    ///
    /// \remarks This is used internally by nom::AnimationPlayer to determine
    /// the course of action to take in the proceeding object cycle.
    enum FrameState
    {
      /// Stop iterating the animation object.
      COMPLETED,
      /// Continue iterating the animation object.
      PLAYING,
      /// Frozen in place
      PAUSED,
    };

    /// \brief Default constructor.
    ///
    /// \remarks The default timing mode function is initialized to
    /// nom::Linear::ease_in_out.
    IActionObject();

    virtual ~IActionObject();

    const std::string& name() const;

    /// \brief Get the action's duration.
    ///
    /// \returns The duration in seconds.
    real32 duration() const;

    /// \brief Get the action's speed modifier.
    real32 speed() const;

    /// \brief Get the action's timing mode.
    const IActionObject::timing_curve_func& timing_curve() const;

    /// \brief Create a deep copy of this instance.
    ///
    /// \remarks This is necessary anytime you wish to re-use an object that
    /// has been used once before, due to its state not being reset after the
    /// first use.
    virtual std::unique_ptr<self_type> clone() const = 0;

    /// \brief Play the animation logic starting from the first frame to the
    /// last frame.
    ///
    /// \note This is internally called by nom::AnimationPlayer.
    virtual IActionObject::FrameState next_frame(real32 delta_time) = 0;

    /// \brief Play the animation logic in reverse -- starting from the last
    /// frame to the first frame.
    ///
    /// \note This is internally called by nom::AnimationReversed on each
    /// animation update cycle.
    virtual IActionObject::FrameState prev_frame(real32 delta_time) = 0;

    /// \brief Reserved for future implementation.
    ///
    /// \note This is called internally called by nom::AnimationPlayer when
    /// the player state is set to nom::AnimationPlayer::PAUSED.
    virtual void pause(real32 delta_time) = 0;

    /// \brief Reserved for future implementation.
    virtual void resume(real32 delta_time) = 0;

    /// \brief Reset the object's state back to its first frame.
    ///
    /// \note This is called internally called by nom::AnimationPlayer when the
    /// player state is set to nom::AnimationPlayer::STOPPED.
    virtual void rewind(real32 delta_time) = 0;

    /// \brief Free the object's resources.
    ///
    /// \note This is called internally by nom::AnimationPlayer when the
    /// animation object is finished updating (signified by FrameState::COMPLETED).
    virtual void release() = 0;

    void set_name(const std::string& action_id);

    /// \brief Set the animation object's speed modifier.
    virtual void set_speed(real32 speed);

    /// \brief Set the animation object's timing mode.
    virtual void set_timing_curve(const IActionObject::timing_curve_func& mode);

  protected:
    IActionObject::FrameState status() const;

    /// \brief Set the action's play time.
    void set_duration(real32 seconds);

    /// \brief The animation object's frame state.
    IActionObject::FrameState status_ =
      IActionObject::FrameState::PLAYING;

    /// \brief The recorded number of frames incremented.
    ///
    /// \remarks This is used for debugging diagnostics.
    real32 elapsed_frames_ = 0.0f;

    /// \brief The number of seconds to play the action for.
    real32 duration_ = 0.0f;

    /// \brief Internal time clock (milliseconds resolution).
    ///
    /// \remarks This provides a stable, fixed duration; frames will be skipped
    /// if performance suffers, but otherwise will not affected by variable
    /// frame rates (i.e.: a duration of one second should always be a minimum
    /// of one second, less and except frames that are missed).
    ///
    /// \note Fixed time step.
    Timer timer_;

  private:
    std::string name_;

    /// \brief A speed factor that modifies how fast an animation object runs.
    ///
    /// \remarks The speed factor adjusts how fast an animation's object runs.
    /// Setting the speed factor to zero (0.0f) will effectively pause the
    /// animation object by preventing the current frame counter from advancing.
    real32 speed_ = 1.0f;

    timing_curve_func timing_curve_;
};

typedef std::vector<std::shared_ptr<IActionObject>> action_list;

/// \brief Convenience non-member constructor function for creating new
/// animation objects.
template<typename ObjectType, typename... ObjectArgs>
std::shared_ptr<ObjectType> create_action(ObjectArgs&&... args)
{
  // IMPORTaNT: We risk object slicing if we use std::make_shared here! The
  // problem occurs when the end-user tries to return the created action
  // pointer by value.
  return( std::shared_ptr<ObjectType>(
          new ObjectType( std::forward<ObjectArgs>(args) ... ) ) );
}

template<typename ObjectType>
std::shared_ptr<ObjectType>
create_action(const action_list& actions, const std::string& name = "")
{
  // IMPORTaNT: We risk object slicing if we use std::make_shared here! The
  // problem occurs when the end-user tries to return the created action
  // pointer by value.
  return( std::shared_ptr<ObjectType>( new ObjectType(actions, name) ) );
}

} // namespace nom

#endif // include guard defined

/// \internal
/// \see [SKAction](https://developer.apple.com/library/prerelease/ios/documentation/SpriteKit/Reference/SKAction_Ref/index.html)
/// \see [SpriteKit Programming Guide](https://developer.apple.com/library/prerelease/ios/documentation/GraphicsAnimation/Conceptual/SpriteKit_PG/Introduction/Introduction.html)
/// \endinternal
