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
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/Timer.hpp"

namespace nom {

/// \brief Pure virtual base class interface for actions.
class IActionObject
{
  public:
    typedef IActionObject self_type;

    /// \brief A function pointer to a timing mode.
    ///
    /// \see nom::Linear, nom::Quad, nom::Cubic, nom::Quart, nom::Quint,
    /// nom::Back, nom::Bounce, nom::Circ, nom::Elastic, nom::Expo, nom::Sine
    typedef
    std::function<real32(real32, real32, real32, real32)>  timing_curve_func;

    /// \brief The update status of the action.
    enum FrameState
    {
      COMPLETED,  /// The action has finished its update loop.
      PLAYING,    /// The action is still updating; duration remains.
    };

    IActionObject();

    virtual ~IActionObject();

    /// \brief Get the unique identifier of the action.
    const std::string& name() const;

    /// \brief Get the duration of the action.
    ///
    /// \returns The duration in fractional seconds.
    real32 duration() const;

    /// \brief Get the speed modifier of the action.
    ///
    /// \returns The speed factor of the action.
    ///
    /// \remarks A value of zero (0.0f) stops the action from progressing
    /// forward in time.
    real32 speed() const;

    /// \brief Get the timing mode function used by the action.
    ///
    /// \returns The function pointer to the timing curve function.
    ///
    /// \see nom::IActionObject::timing_curve_func
    const IActionObject::timing_curve_func& timing_curve() const;

    /// \brief Set the unique identifier of the action.
    void set_name(const std::string& action_id);

    /// \brief Set the speed factor of the action.
    virtual void set_speed(real32 speed);

    /// \brief Set the timing mode of the action.
    ///
    /// \see nom::IActionObject::timing_curve_func
    virtual void set_timing_curve(const IActionObject::timing_curve_func& mode);

    /// \brief Create a deep copy instance of the action.
    ///
    /// \remarks A cloned instance is created using the action's attributes
    /// at the time of construction. External resources of the action --
    /// i.e.: nom::Sprite -- are not modified, and you may need to reset the
    /// state appropriately if the action has been ran previously.
    virtual std::unique_ptr<IActionObject> clone() const = 0;

    /// \brief Play the action forward in time by one time step.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \see nom::DispatchQueue
    virtual IActionObject::FrameState next_frame(real32 delta_time) = 0;

    /// \brief Play the action backwards in time by one time step.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \remarks Not all actions are reversible -- see the action's
    /// documentation for its implementation details.
    ///
    /// \see nom::ReversedAction
    virtual IActionObject::FrameState prev_frame(real32 delta_time) = 0;

    /// \brief Freeze the action's internal state.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \see nom::DispatchQueue
    virtual void pause(real32 delta_time) = 0;

    /// \brief Resume the internal state of the action.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \see nom::DispatchQueue
    virtual void resume(real32 delta_time) = 0;

    /// \brief Reset the internal state of the action back to its initial
    /// starting values.
    ///
    /// \param delta_time Reserved for application-defined implementations.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \see nom::RepeatForAction, nom::RepeatForeverAction
    virtual void rewind(real32 delta_time) = 0;

    /// \brief Free externally referenced resources held by the action.
    ///
    /// \note <b>This method should not be called externally!</b>
    ///
    /// \see nom::RemoveAction
    virtual void release() = 0;

  protected:
    /// \brief Get the current state of the action.
    ///
    /// \see nom::IActionObject::FrameState
    IActionObject::FrameState status() const;

    /// \brief Set the duration of the action.
    ///
    /// \param seconds The duration in seconds.
    void set_duration(real32 seconds);

    FrameState status_ = FrameState::PLAYING;
    real32 elapsed_frames_ = 0.0f;
    real32 duration_ = 0.0f;

    /// \brief Internal time clock (milliseconds resolution).
    ///
    /// \remarks Each action is responsible for keeping track of its clock --
    /// this provides a stable, fixed duration that yields to reliable results
    /// across any frame rate -- at the cost of potentially skipping frames
    /// when performance suffers.
    Timer timer_;

  private:
    std::string name_;
    real32 speed_ = 1.0f;
    timing_curve_func timing_curve_ = nullptr;
};

/// \brief A collection of actions.
///
/// \relates nom::IActionObject
typedef std::vector<std::shared_ptr<IActionObject>> action_list;

/// \brief Constructor function for creating an action.
///
/// \param args The arguments to pass to the constructed object.
///
/// \relates nom::IActionObject
template<typename ObjectType, typename... ObjectArgs>
std::shared_ptr<ObjectType> create_action(ObjectArgs&&... args)
{
  // IMPORTANT: We risk object slicing if we use std::make_shared here! The
  // problem occurs when the end-user tries to return the created action
  // pointer by value.
  return( std::shared_ptr<ObjectType>(
          new ObjectType( std::forward<ObjectArgs>(args) ... ) ) );
}

/// \brief Constructor function for creating an action that uses a collection
/// of actions.
///
/// \param actions The collection of actions to pass to the constructed object.
///
/// \param name An optional unique identifier to pass to the constructed
/// object.
///
/// \remarks The collection can be constructed in-place with a
/// [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)
/// object.
///
/// \note If you are building with Visual Studio 2013 on the Windows platform,
/// you will want to ensure that you have Update 2 or better applied before
/// using this function call with a std::initializer_list object. See also:
/// [stackoverflow.com: std::shared_ptr in an std::initializer_list appears to be getting destroyed prematurely](http://stackoverflow.com/questions/22924358/stdshared-ptr-in-an-stdinitializer-list-appears-to-be-getting-destroyed-prem/22924473#22924473)
///
/// \see nom::GroupAction, nom::SequenceAction
///
/// \relates nom::IActionObject
template<typename ObjectType>
std::shared_ptr<ObjectType>
create_action(const action_list& actions, const std::string& name = "")
{
  // IMPORTANT: We risk object slicing if we use std::make_shared here! The
  // problem occurs when the end-user tries to return the created action
  // pointer by value.
  return( std::shared_ptr<ObjectType>( new ObjectType(actions, name) ) );
}

} // namespace nom

#endif // include guard defined

/// \class nom::IActionObject
/// \ingroup actions
///
/// **TODO:** This documentation section is a *STUB*!
///
/// ## Creating Custom Actions
///
/// A simple, bare-bones example:
/// \snippet src/actions/WaitForDurationAction.cpp creating_custom_actions
///
/// # References (Conceptual)
/// \see [SpriteKit: SKAction Class Reference](https://developer.apple.com/library/prerelease/ios/documentation/SpriteKit/Reference/SKAction_Ref/index.html)
/// \see [SpriteKit: Creating Actions That Run Other Actions](https://developer.apple.com/library/ios/documentation/GraphicsAnimation/Conceptual/SpriteKit_PG/AddingActionstoSprites/AddingActionstoSprites.html)
///
