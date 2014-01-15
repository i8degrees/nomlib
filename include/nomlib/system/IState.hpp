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
#include "nomlib/system/Input.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

/// \brief Abstract interface for game states
///
/// \remarks See also nom::StateMachine, nom::SDLApp
class IState: public Input
{
  public:
    typedef std::unique_ptr<IState> UniquePtr;

    enum StateFlags
    {
      Null = 0,       // Default
      BackRender = 1  // Signal to nom::StateMachine to let previous state
                      // continue rendering.
    };

    /// \brief Default constructor; initializes class variables to zero.
    IState ( void );

    /// \brief Destructor; declared virtual for inheritance
    virtual ~IState ( void );

    /// \brief Constructor for initializing the class variable fields.
    IState ( uint32 id, uint32 timestamp, uint32 flags );

    /// \brief Obtain the user-defined identifier of the state.
    uint32 id ( void ) const;

    /// \brief Obtain the user-defined time stamp of the state.
    uint32 timestamp ( void ) const;

    /// \brief Obtain the user-defined flags of the state.
    uint32 flags ( void ) const;

    /// \brief User-defined implementation of the state's update logic.
    ///
    /// \param float User-defined; typically a delta time (change in time).
    virtual void on_update ( float );

    /// User-defined implementation of the state's rendering logic
    ///
    /// \param IDrawable::RenderTarget Rendering target (nom::Window).
    ///
    /// \todo Consider removing RenderTarget argument; I *think* we can get
    /// away with this.
    virtual void on_draw ( IDrawable::RenderTarget );

    /// \brief User-defined implementation of the state's initialization logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    virtual void on_init ( void_ptr );

    /// \brief User-defined implementation of the state's destruction logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    virtual void on_exit ( void_ptr );

    /// \brief User-defined implementation of the state's paused state logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    virtual void on_pause ( void_ptr );

    /// \brief User-defined implementation of the state's resume from pause
    /// state logic.
    ///
    /// \param void_ptr A void pointer to user-defined data
    virtual void on_resume ( void_ptr );

  private:
    /// \brief Unique identifier for the state
    ///
    /// \remarks Reserved for future implementation
    uint32 id_;

    /// \brief Time of state's construction
    ///
    /// \remarks Reserved for future implementation
    uint32 timestamp_;

    /// \brief Control state management -- such as requesting for the manager
    /// to continue rendering the previous state on the stack in addition to the
    /// current one.
    ///
    /// \remarks Reserved for future implementation
    uint32 flags_;
};

} // namespace nom

#endif // include guard defined
