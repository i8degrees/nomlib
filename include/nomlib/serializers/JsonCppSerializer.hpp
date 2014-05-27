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
#ifndef NOMLIB_SERIALIZERS_JSONCPP_SERIALIZER_HPP
#define NOMLIB_SERIALIZERS_JSONCPP_SERIALIZER_HPP

#include <sstream>
#include <fstream>

#include "nomlib/config.hpp"
#include "nomlib/serializers/serializers_config.hpp"
#include "nomlib/serializers/IValueSerializer.hpp"
#include "nomlib/ptree.hpp"

// Enable dumping output of each key, value pair, sizes, etc. as we
// traverse the object.
// #define NOM_DEBUG_JSONCPP_SERIALIZER_VALUES

// Forward declarations for third-party libraries used; these allow us to not
// worry about including third-party header files in outside-project builds,
// such as is the case with TTcards.
namespace Json {

class Value;

} // namespace Json

namespace nom {

/// \brief Number of spaces to pass to JsonCpp for indention.
///
/// \remarks Two space tabbed indention.
const std::string JSONCPP_INDENTION_LEVEL = "  ";

/// \brief Saving of nom::Value objects to JSON documents using the JsonCpp
/// library.
class JsonCppSerializer: public IValueSerializer
{
  public:
    typedef JsonCppSerializer SelfType;

    /// \brief Default constructor; initialize the writer with human friendly
    /// output.
    ///
    /// \remarks See the SerializerOptions enumeration for more information.
    JsonCppSerializer( void );

    /// \brief Destructor.
    ~JsonCppSerializer( void );

    /// \brief Construct a parser with a feature set enumeration.
    JsonCppSerializer( enum SerializerOptions options );

    /// \brief Output a nom::Value object to a string.
    ///
    /// \param source nom::Value container to serialize from.
    ///
    /// \returns std::string with JSON-conforming output on success, or a null-
    /// terminated string on err.
    ///
    /// \note Implements ISerializer interface.
    std::string serialize( const Value& source );

    /// \brief Convenience method provided to output JSON-formatted data to a
    /// file.
    ///
    /// \param source nom::Value container to serialize from.
    /// \param filename Absolute file path to output resulting data to.
    ///
    /// \note Implements ISerializer interface.
    bool save( const Value& source, const std::string& filename );

  private:
    enum SerializerOptions options_;

    bool write( const Value& source, Json::Value& dest ) const;

    /// \todo Err handling where used in code
    bool write_value( const Value& object, Json::Value& dest ) const;

    /// \todo Err handling where used in code
    bool write_array_value( const Value& object, Json::Value& dest ) const;

    /// \brief Internal helper method for serialization of array nodes.
    ///
    /// \todo Err handling where used in code
    bool write_array( const Value& object, Json::Value& dest ) const;

    /// \brief Internal helper method for serialization of object nodes.
    ///
    /// \todo Err handling where used in code
    bool write_object( const Value& object, Json::Value& dest ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::JsonCppSerializer
/// \ingroup json
///
///   [TO BE WRITTEN]
///
