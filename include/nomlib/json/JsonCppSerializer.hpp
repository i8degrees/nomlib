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

#include <sstream>
#include <fstream>

#include "jsoncpp/json.h"

#include "nomlib/config.hpp"
#include "nomlib/system/ISerializer.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"
#include "nomlib/system/ptree/Value.hpp"
#include "nomlib/system/ptree/ValueIterator.hpp"
#include "nomlib/system/ptree/ValueConstIterator.hpp"

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_JSONCPP_SERIALIZER_VALUES

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES

namespace nom {

/// \brief Number of spaces to pass to JsonCpp for indention.
///
/// \remarks Two space tabbed indention.
const std::string JSONCPP_INDENTION_LEVEL = "  ";

/// \brief Serialization of nom::Value objects to and fro JSON using the
/// JsonCpp library.
class JsonCppSerializer: public ISerializer
{
  public:
    typedef JsonCppSerializer SelfType;

    JsonCppSerializer( void );
    ~JsonCppSerializer( void );

    /// \brief Output a nom::Value object to a string.
    ///
    /// \param source nom::Value container to serialize from.
    ///
    /// \returns std::string with JSON-conforming output on success, or a null-
    /// terminated string on err.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    std::string serialize ( const Value& source,
                            // enum Features options = HumanReadable
                           enum Features options = Compact
                          );

    /// \brief Parse JSON data as a nom::Value object.
    ///
    /// \param source std::string containing valid JSON.
    ///
    /// \returns nom::Value object filled from JSON-compliant input on success,
    /// or nom::Value::null on err.
    ///
    /// \note Implements IJsonSerializer::unserialize interface.
    Value unserialize ( const std::string& source,
                        // enum Features options = HumanReadable
                         enum Features options = Compact
                      );

    /// \brief Convenience method provided to output JSON-formatted data to a
    /// file.
    ///
    /// \param source nom::Value container to serialize from.
    /// \param filename Absolute file path to output resulting data to.
    ///
    /// \note Implements IJsonSerializer::save interface.
    bool save ( const Value& source,
                const std::string& filename,
                // enum Features options = HumanReadable
                enum Features options = Compact
              );

    /// \brief Convenience method provided to store JSON-formatted data from a
    /// file.
    ///
    /// \param filename Absolute file path to data to unserialize from.
    /// \param output nom::Value container to store resulting data in.
    ///
    /// \note Implements IJsonSerializer::load interface.
    bool load ( const std::string& filename,
                Value& output,
                // enum Features options = HumanReadable
                enum Features options = Compact
              );

  private:
    bool write( const Value& source, Json::Value& dest ) const;

    bool read( const Json::Value& source, Value& dest ) const;

    /// \brief Dump the object's complete value tree.
    ///
    /// \remarks This is a useful method for debugging.
    ///
    /// \note This method is used by the << overload function for nom::Value
    /// objects.
    const std::string dump( const Json::Value& object, int depth = 0 ) const;

    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_key( const Json::Value& key ) const;

    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_value( const Json::Value& val ) const;

    /// \brief Internal helper method for nom::Value::dump_key.
    const std::string print_key( const std::string& type, uint size ) const;

    /// \brief Internal helper method for nom::Value::dump_value.
    const std::string print_value( const std::string& value ) const;

    /// \TODO Err handling where used in code
    bool write_value( const Value& object, Json::Value& dest ) const;

    /// \TODO Err handling where used in code
    bool write_array_value( const Value& object, Json::Value& dest ) const;

    /// \brief Internal helper method for serialization of array nodes.
    ///
    /// \TODO Err handling where used in code
    bool serialize_array( const Value& object, Json::Value& dest ) const;

    /// \brief Internal helper method for serialization of object nodes.
    ///
    /// \TODO Err handling where used in code
    bool serialize_object( const Value& object, Json::Value& dest ) const;

    /// \TODO Err handling where used in code
    bool read_value( const Json::Value& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of array nodes.
    ///
    /// \TODO Err handling where used in code
    bool unserialize_array( const Json::Value& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of object nodes.
    ///
    /// \TODO Err handling where used in code
    bool unserialize_object( const Json::Value& object, Value& dest ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::JsonCppSerializer
/// \ingroup json
///
///   [TO BE WRITTEN]
///
/// See examples/values.cpp for usage examples.
///
/// \TODO Additional error handling via Json::Reader::getFormattedErrorMessage
///
