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
#ifndef NOMLIB_SYSTEM_ISTATE_HPP
#define NOMLIB_SYSTEM_ISTATE_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/EventHandler.hpp"

namespace nom {

// Forward declarations
class RenderWindow;

/// \brief Abstract interface for game states
///
/// \remarks See also nom::StateMachine, nom::SDLApp
class IState: public EventHandler
{
  public:
    typedef std::unique_ptr<IState> unique_ptr;

    /// \brief Specialized control over state management.
    ///
    /// \remarks Multiple flags can be combined via bit masks.
    enum Flags
    {
      /// Request to nom::StateMachine to let previous state continue updating
      /// itself.
      BackUpdate = 0x2,

      /// Request to nom::StateMachine to let previous state continue rendering
      /// itself.
      BackRender = 0x4
    };

    /// \brief State type enumeration.
    ///
    /// \remarks The type of state defines the placement of it on the stack.
    enum Type
    {
      /// \brief Top-level / top-most placement on the stack.
      Parent = 0,
      Child
    };

    /// \brief Default constructor; initializes class variables to zero.
    IState( void );

    /// \brief Destructor; declared virtual for inheritance
    virtual ~IState( void );

    /// \brief Constructor for user-defined initialization of the class
    /// variable fields.
    ///
    /// \param id Unique identifier of the state
    IState( uint32 id );

    /// \brief Constructor for user-defined initialization of the class
    /// variable fields.
    ///
    /// \param id     Unique identifier of the state
    /// \param flags  One or more of the nom::IState::StateFlags enumerations.
    /// \param type   The type of state; one of the IState::Type enumeration
    /// values.
    IState( uint32 id, uint32 flags, IState::Type type );

    /// \brief Obtain the user-defined identifier of the state.
    ///
    /// \returns ID of the state
    ///
    /// \remarks It is not required that the state has an identifier set.
    uint32 id ( void ) const;

    /// \brief Obtain the recorded time stamp of the state.
    ///
    /// \returns Number of ticks recorded at the time of construction
    ///
    /// \remarks Value recorded is from the return value of nom::ticks
    uint32 timestamp ( void ) const;

    /// \brief Obtain the user-defined flags of the state.
    ///
    /// \returns Bit masked enumeration of nom::IState::StateFlags.
    ///
    /// \remarks It is not required that the state has any flags set.
    uint32 flags ( void ) const;

    /// \brief Get the type of state.
    ///
    /// \returns One of the IState::Type enumeration values.
    IState::Type type( void ) const;

    /// \brief User-defined implementation of the state's event handling logic.
    ///
    /// \param ev The passed nom::Event object.
    ///
    /// \remarks This method provides a means of control for event propagation
    /// flow.
    ///
    /// \fixme This is currently required for GUI events processing in TTcards,
    /// and probably can be handled better...
    ///
    /// \returns This method should return Boolean TRUE when the nom::Event
    /// object has been processed (think: consumed) by the user-implemented
    /// method, and boolean FALSE when the nom::Event object has not been
    /// processed (think: consumed). The default implementation returns false.
    ///
    /// \see StateMachine::on_event
    virtual bool on_event( const nom::Event& ev );

    /// \brief User-defined implementation of the state's update logic.
    ///
    /// \param float User-defined; typically a delta time (change in time).
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    virtual void on_update ( float );

    /// User-defined implementation of the state's rendering logic
    ///
    /// \param IDrawable::RenderTarget Rendering target (nom::RenderWindow).
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    ///
    /// \todo Consider removing RenderTarget argument; I *think* we can get
    /// away with this.
    virtual void on_draw ( RenderWindow& );

    /// \brief User-defined implementation of the state's initialization logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    virtual void on_init ( void_ptr );

    /// \brief User-defined implementation of the state's destruction logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    virtual void on_exit ( void_ptr );

    /// \brief User-defined implementation of the state's paused state logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    virtual void on_pause ( void_ptr );

    /// \brief User-defined implementation of the state's resume from pause
    /// state logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    ///
    /// \remarks When not defined, a do-nothing implementation is provided.
    virtual void on_resume ( void_ptr );

  private:
    /// \brief Unique identifier for the state
    uint32 id_;

    /// \brief Number of ticks recorded at the time of state's construction
    uint32 timestamp_;

    /// \brief Specialized state management
    uint32 flags_;

    /// \brief The type of state.
    Type type_;
};

} // namespace nom

#endif // include guard defined
