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
#ifndef NOMLIB_JSON_JSONCPP_DESERIALIZER_HPP
#define NOMLIB_JSON_JSONCPP_DESERIALIZER_HPP

#include <sstream>
#include <fstream>

#include "jsoncpp/json.h"

#include "nomlib/config.hpp"
#include "nomlib/serializers/serializers_config.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/ptree.hpp"

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES

namespace nom {

/// \brief Restoring of nom::Value objects from JSON documents using the
/// JsonCpp library.
class JsonCppDeserializer: public IValueDeserializer
{
  public:
    /// \brief Default constructor; initialize the parser to strict mode
    /// compliance.
    ///
    /// \remarks See the DeserializerOptions enumeration for more information.
    JsonCppDeserializer( void );

    /// \brief Destructor.
    ~JsonCppDeserializer( void );

    /// \brief Construct a parser with a feature set enumeration.
    ///
    /// \remarks Bit-mask friendly.
    JsonCppDeserializer( uint32 options );

    /// \brief Parse JSON data as a nom::Value object.
    ///
    /// \param source std::string containing valid JSON.
    ///
    /// \returns nom::Value object filled from JSON-compliant input on success,
    /// or nom::Value::null on err.
    ///
    /// \note Implements IDeserializer interface.
    Value deserialize ( const std::string& source );

    /// \brief Convenience method provided to store JSON-formatted data from a
    /// file.
    ///
    /// \param filename Absolute file path to data to deserialize from.
    /// \param output nom::Value container to store resulting data in.
    ///
    /// \note Implements IDeserializer interface.
    bool load ( const std::string& filename, Value& output );

  private:
    uint32 options_;

    bool read( const Json::Value& source, Value& dest ) const;

    /// \TODO Err handling where used in code
    bool read_value( const Json::Value& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of array nodes.
    ///
    /// \TODO Err handling where used in code
    bool read_array( const Json::Value& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of object nodes.
    ///
    /// \TODO Err handling where used in code
    bool read_object( const Json::Value& object, Value& dest ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::JsonCppDeserializer
/// \ingroup json
///
///   [TO BE WRITTEN]
///
/// \TODO Additional error handling via Json::Reader::getFormattedErrorMessage
///
