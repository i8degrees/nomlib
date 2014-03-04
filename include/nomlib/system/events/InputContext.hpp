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
#ifndef NOMLIB_SYSTEM_EVENTS_INPUT_CONTEXT_HPP
#define NOMLIB_SYSTEM_EVENTS_INPUT_CONTEXT_HPP

#include <map>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/events/EventCallback.hpp"
#include "nomlib/system/events/InputAction.hpp" // nom::InputMapping

namespace nom {

struct InputState
{
  bool active_;
  InputMapping actions_;
};

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

    bool active( const std::string& state ) const;

    /// \brief Insert an input mapping.
    // bool add_input_mapping( const std::string& key, const Action& action );

    /// \brief Remove an input mapping.
    // bool remove_input_mapping( const std::string& key );

    /// \brief Add an input mapping to the context map.
    ///
    /// \param state The name of the context.
    bool add_context( const std::string& state, const InputMapping& map );

    /// \brief Remove an input mapping from the context map.
    ///
    /// \param state The name of the context.
    bool remove_context( const std::string& state );

    bool activate_context( const std::string& state );
    bool disable_context( const std::string& state );

    /// \brief Empty the list of set context(s).
    void clear( void );

    bool on_input( const SDL_Event* ev );

  private:
    // InputMapping active_context_;
    ContextMap contexts_;
};

} // namespace nom

#endif // include guard defined
