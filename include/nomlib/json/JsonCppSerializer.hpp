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
#ifndef NOMLIB_JSON_JSONCPP_SERIALIZER_HPP
#define NOMLIB_JSON_JSONCPP_SERIALIZER_HPP

#include <fstream>
#include <sstream>

#include "nomlib/json/jsoncpp/json.h" // JsonCpp

#include "nomlib/config.hpp"
#include "nomlib/json/JsonCppValue.hpp" // JsonCpp API wrapper

// #include "nomlib/system/IJsonSerializer.hpp"

/// \brief Enable dumping of each key name, value & type pairs as we traverse
/// each object.
// #define NOM_DEBUG_JSONCPP_SERIALIZER_VALUES

namespace nom {

/// \brief Number of spaces to pass to JsonCpp for indention.
const std::string JSONCPP_INDENTION_LEVEL = "  "; // Two spaced single tab
                                                  // indention.

/// \brief Serialization of objects using JsonCpp API
class JsonCppSerializer//: public IJsonSerializer
{
  public:
    typedef JsonCppSerializer SelfType;

    /// \brief Serialization options
    ///
    /// \TODO Implement
    enum Feature
    {
      Compact = 0,  // Default
      HumanReadable // json_spirit::pretty_print (single line arrays)
    };

    JsonCppSerializer( void );
    ~JsonCppSerializer( void );

    /// \brief Save nom::Value values to JSON objects.
    ///
    /// \param source nom::JsonCppValue container to serialize.
    /// \param output Absolute file path to save resulting data to.
    ///
    /// \note ~~Implements IJsonSerializer::serialize interface~~
    bool serialize( const JsonCppValue& source, const std::string& output ) const;

    /// \brief Load data from a JSON (.json) file to a nom::Value object.
    ///
    /// \param input  Absolute file path file to un-serialize.
    /// \param dest   nom::JsonCppValue container to store values in.
    ///
    /// \note ~~Implements IJsonSerializer::unserialize interface.~~
    bool unserialize( const std::string& input, JsonCppValue& dest ) const;

    /// \brief Obtain a C++ string of the JSON object(s) stored.
    ///
    /// \param obj The nom::JsonCppValue object to be serialized.
    ///
    /// \returns Serialized object as a std::string.
    const std::string stringify( const JsonCppValue& obj ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::JsonCppSerializer
/// \ingroup json
///
///   [TO BE WRITTEN]
///
