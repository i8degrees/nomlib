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
#ifndef NOMLIB_JSON_JSON_SERIALIZER_HPP
#define NOMLIB_JSON_JSON_SERIALIZER_HPP

#include <sstream>
#include <fstream>

#include "nomlib/json/jsoncpp/json.h" // JsonCpp library

#include "nomlib/config.hpp"
#include "nomlib/system/IJsonSerializer.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"
#include "nomlib/system/ptree/Value.hpp"
#include "nomlib/system/ptree/ValueIterator.hpp"
#include "nomlib/system/ptree/ValueConstIterator.hpp"

/// \brief Enable dumping of each key name, value & type pairs as we traverse
/// each object.
// #define NOM_DEBUG_JSON_SERIALIZER_VALUES

namespace nom {

/// \brief Number of spaces to pass to JsonCpp for indention.
///
/// \remarks Two space tabbed indention.
const std::string JSONCPP_INDENTION_LEVEL = "  ";

/// \brief Serialization of nom::Value objects to JSON
class JsonSerializer: public IJsonSerializer
{
  public:
    typedef JsonSerializer SelfType;

    /// \brief Serialization options.
    ///
    /// \TODO Implement
    enum Feature
    {
      Compact = 0,  // Default
      HumanReadable // json_spirit::pretty_print (single line arrays)
    };

    JsonSerializer( void );
    ~JsonSerializer( void );

    /// \brief Save nom::Value values to JSON objects.
    ///
    /// \param source nom::Value container to serialize.
    /// \param output Absolute file path to save resulting data to.
    ///
    /// \note ~~Implements IJsonSerializer::serialize interface.~~
    bool serialize( const Value& source, const std::string& output ) const;

    /// \brief Load data from a JSON (.json) file to a nom::Value object.
    ///
    /// \param input  Absolute file path file to un-serialize.
    /// \param dest   nom::Value container to store values in.
    ///
    /// \note ~~Implements IJsonSerializer::unserialize interface.~~
    bool unserialize( const std::string& input, Value& dest ) const;

    /// \brief Obtain a C++ string of the JSON object(s) stored.
    ///
    /// \param obj The nom::Value object to be serialized.
    ///
    /// \returns Serialized object as a std::string.
    const std::string stringify( const Value& obj ) const;

    /// \brief Dump the object's complete value tree.
    ///
    /// \remarks This is a useful method for debugging.
    ///
    /// \note This method is used by the << overload function for nom::Value
    /// objects.
    const std::string dump( const Json::Value& object, int depth = 0 ) const;

  private:
    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_key( const Json::Value& key ) const;

    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_value( const Json::Value& val ) const;

    /// \brief Internal helper method for nom::Value::dump_key.
    const std::string print_key( const std::string& type, uint size ) const;

    /// \brief Internal helper method for nom::Value::dump_value.
    // const std::string print_value( const std::string& val ) const;
    template <typename T>
    const std::string print_value( const T& val ) const
    {
      std::stringstream os;

      os << val;

      return os.str();
    }

    /// \NOTE Reserved for future implementation.
    ///
    /// \TODO Iterate through nom::Value objects via recursion in order to
    /// support deep hierarchies of arrays.
    bool serialize_array( const Value& object, Json::Value& dest ) const;

    /// \NOTE Reserved for future implementation.
    ///
    /// \TODO Iterate through nom::Value objects via recursion in order to
    /// support deep hierarchies of objects.
    bool serialize_object( const Value& object, Json::Value& dest ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::JsonSerializer
/// \ingroup json
///
///   [TO BE WRITTEN]
///
