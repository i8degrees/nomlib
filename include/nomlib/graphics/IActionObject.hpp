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
#ifndef NOMLIB_GRAPHICS_IACTION_OBJECT_HPP
#define NOMLIB_GRAPHICS_IACTION_OBJECT_HPP

#include <functional>

#include "nomlib/config.hpp"
#include "nomlib/system/Timer.hpp"

namespace nom {

// Forward declarations
class Transformable;
// class Timer;

/// \brief [TODO: Description]
class IActionObject
{
  public:
    typedef IActionObject self_type;

    typedef std::function<real32(real32, real32, real32, real32)> timing_mode_func;

    /// \brief Allow access into our private parts for obtaining the current
    /// frame.
    // TODO: Remove
    NOM_GTEST_FRIEND(AnimationTest, TextureFramesDemo);

    /// \brief Allow access into our private parts for unit testing.
    friend class AnimationTest;

    /// \brief The animation object's state.
    ///
    /// \remarks This is used internally by nom::AnimationPlayer to determine
    /// the course of action to take in the proceeding object cycle.
    enum FrameState
    {
      // TODO: Rename to IDLE or COMPLETED?
      DONE,             /// Stop iterating the animation object.
      // TODO: Rename to PLAYING?
      PLAY_NEXT_FRAME,  /// Continue iterating the animation object.
      REWIND,
      PAUSED
    };

    /// \brief Default constructor.
    ///
    /// \remarks The speed modifier is initialized to 1.0f. The default timing
    /// mode function is nom::Linear::ease_in_out.
    IActionObject();

    /// \brief Destructor.
    virtual ~IActionObject();

    /// \brief Copy constructor.
    ///
    /// \fixme This does not work as intended because we cannot create a copy
    /// constructor that clones nom::Transformable -- it is considered a pure
    /// abstract class type, since it inherits off nom::IDrawable.
    // IActionObject(const self_type& rhs);

    const std::string& name() const;

    /// \brief Get the action's duration.
    ///
    /// \returns The duration in milliseconds.
    real32 duration() const;

    /// \brief Get the action's speed modifier.
    real32 speed() const;

    /// \brief Get the action's timing mode.
    const IActionObject::timing_mode_func& timing_mode() const;

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
    /// animation object is finished updating (signified by FrameState::DONE).
    virtual void release() = 0;

    void set_name(const std::string& action_id);

    /// \brief Set the animation object's speed modifier.
    virtual void set_speed(real32 speed);

    /// \brief Set the animation object's timing mode.
    virtual void set_timing_mode(const IActionObject::timing_mode_func& mode);

  protected:
    IActionObject::FrameState status() const;

    /// \brief Set the action's run duration.
    void set_duration(real32 seconds);

    /// \brief The animation object's frame state.
    IActionObject::FrameState status_ =
        IActionObject::FrameState::PLAY_NEXT_FRAME;

    /// \brief The elapsed frame count of the animation object.
    ///
    /// \remarks This value is incremented per frame when it is less than the
    /// total number of frames.
    ///
    /// \note This value is fractional to ease the addition of fractional speed
    /// modifiers, such as 0.5f.
    real32 curr_frame_ = 0.0f;

    /// \brief The number of milliseconds to run the action for.
    ///
    /// \remarks The duration of an animation correlates to the number of
    /// frames.
    real32 duration_ = 0.0f;

    /// \brief Internal time clock (milliseconds resolution).
    ///
    /// \remarks This provides a stable, fixed duration; frames will be skipped
    /// if performance suffers, but otherwise will not affected by variable
    /// frame rates (i.e.: a duration of one second should always be a minimum
    /// of one second, less and except frames that are missed).

    // TODO: We need to verify frame timings ... the above statement may not be
    // true! See CallbackActionWithNonZeroDuration test; --fps 0 produces many
    // more frames than --fps 30 ... also, on occasion, with --fps 30, we'll
    // see only a couple frames, with delta times way below expected values,
    // implying that a large slice of time went unaccounted for before coming
    // back to the action!

    // This must be used because of the way we iterate through animations in
    // the AnimationPlayer ... delta_time is inaccurate because the elapsed
    // delta is not up-to-date in reference to the time at which ::next_frame,
    // ::prev_frame gets called.
    Timer timer_;
    // std::unique_ptr<Timer> timer_;

    // virtual uint32 frame_time();
    // virtual void start_frame_timer();
    // virtual void stop_frame_timer();
    // virtual void pause_frame_timer();
    // virtual void resume_frame_timer();

  private:
    std::string name_;

    /// \brief A speed factor that modifies how fast an animation object runs.
    ///
    /// \remarks The speed factor adjusts how fast an animation's object runs.
    /// Setting the speed factor to zero (0.0f) will effectively pause the
    /// animation object by preventing the current frame counter from advancing.
    real32 speed_ = 1.0f;

    timing_mode_func timing_mode_;
};

typedef std::initializer_list<std::shared_ptr<IActionObject>> actions_list;
typedef std::vector<std::shared_ptr<IActionObject>> actions_container;

/// \brief Convenience non-member constructor function for creating new
/// animation objects.
///
/// \internal
///
/// \remarks This is a wrapper for
/// std::make_shared<AnimationObjectType>(ctor_args).
///
/// \endinternal
template<typename ObjectType, typename... ObjectArgs>
std::shared_ptr<ObjectType> create_action(ObjectArgs&&... args)
{
  return( std::make_shared<ObjectType>(
          ObjectType( std::forward<ObjectArgs>(args) ... ) )
        );
}

template<typename ObjectType>
std::shared_ptr<ObjectType>
create_action(const actions_list& actions, const std::string& name = "")
{
  return( std::make_shared<ObjectType>(actions, name) );
}

} // namespace nom

#endif // include guard defined
