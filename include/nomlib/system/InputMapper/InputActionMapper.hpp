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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_MAPPER_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_MAPPER_HPP

#include <memory>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/system/InputMapper/InputAction.hpp"

namespace nom {

/// \brief High-level API for associating names with input actions.
///
/// \remarks See also, nom::InputAction.
class InputActionMapper
{
  public:
    friend class InputStateMapper;

    typedef InputActionMapper SelfType;
    typedef std::multimap<std::string, std::shared_ptr<InputAction>> ActionMap;
    typedef std::pair<std::string, std::shared_ptr<InputAction>> ActionPair;

    /// \brief Default constructor.
    InputActionMapper( void );

    /// \brief Destructor.
    ~InputActionMapper( void );

    /// \brief Insert an action mapping.
    bool insert( const std::string& key, const InputAction& action, const EventCallback& callback );

    /// \brief Remove an action mapping.
    bool erase( const std::string& key );

    /// \brief Diagnostic output.
    ///
    /// remarks Dumps only the active states.
    void dump( void ) const;

  private:
    /// \brief Obtain the input map's contents.
    ///
    /// \remarks Used internally by nom::InputStateMapper.
    const ActionMap& get( void ) const;

    /// \brief Hash container of input actions, sorted by name.
    ///
    /// \remarks Note that we are a multi-map, therefore multiple duplicate
    /// action keys can be used.
    ActionMap input_map_;
};

#define NOM_CONNECT_INPUT_MAPPING( obj, key, action, func, ... ) \
  ( obj.insert( key, action, nom::EventCallback( [&] ( const nom::Event& evt ) { func( __VA_ARGS__ ); } ) ) )

} // namespace nom

#endif // include guard defined
