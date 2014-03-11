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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_CONTEXT_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_CONTEXT_HPP

#include <map>

#include "nomlib/config.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/system/InputMapper/InputAction.hpp"

namespace nom {

struct InputState
{
  bool active_;
  InputMapping actions_;
};

// InputMapping = std::multimap<std::string, InputAction>

class InputContext
{
  public:
    typedef InputContext SelfType;
    // typedef std::pair<std::string, InputMapping> Pair;
    typedef std::pair<std::string, InputState> Pair;
    // typedef std::map<std::string, InputMapping> ContextMap;
    typedef std::map<std::string, InputState> ContextMap;

    /// \brief Default constructor.
    InputContext( void );

    /// \brief Destructor.
    ~InputContext( void );

    /// \brief Insert an input mapping.
    // bool add_input_mapping( const std::string& key, const InputAction& action );

    /// \brief Remove an input mapping.
    // bool remove_input_mapping( const std::string& key );

    bool active( const std::string& state ) const;

    /// \brief Add an input mapping to the context map.
    ///
    /// \param state The name of the context.
    bool add_context( const std::string& state, const InputMapping& map, bool active = true );

    /// \brief Remove an input mapping from the context map.
    ///
    /// \param state The name of the context.
    bool remove_context( const std::string& state );

    bool activate_context( const std::string& state );
    bool disable_context( const std::string& state );

    /// \brief Empty the list of set context(s).
    void clear( void );

    // void dump_actions( void );
    void dump_states( void );

    void on_event( const Event& ev );

  private:
    bool on_key_press( const InputAction& mapping, const Event& ev );

    bool on_mouse_button( const InputAction& mapping, const Event& ev );
    bool on_mouse_wheel( const InputAction& mapping, const Event& ev );

    bool on_joystick_button( const InputAction& mapping, const Event& ev );

    /// \fixme The implementation is incomplete.
    bool on_joystick_axis( const InputAction& mapping, const Event& ev );

    // InputMapping active_context_;
    ContextMap contexts_;
};

} // namespace nom

#endif // include guard defined
