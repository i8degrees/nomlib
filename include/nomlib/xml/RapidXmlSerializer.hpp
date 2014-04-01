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
#ifndef NOMLIB_XML_RAPIDXML_SERIALIZER_HPP
#define NOMLIB_XML_RAPIDXML_SERIALIZER_HPP

#include <sstream>
#include <fstream>

#include "rapidxml.hpp" // RapidXml library

#include "nomlib/config.hpp"
#include "nomlib/system/ISerializer.hpp"
#include "nomlib/system/ptree/ptree_forwards.hpp"
#include "nomlib/system/ptree/Value.hpp"
#include "nomlib/system/ptree/ValueIterator.hpp"
#include "nomlib/system/ptree/ValueConstIterator.hpp"

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES

namespace nom {

/// \brief Number of spaces to pass to JsonCpp for indention.
///
/// \remarks Two space tabbed indention.
const std::string RAPIDXML_INDENTION_LEVEL = "  ";

/// \brief Serialization of nom::Value objects to and fro XML using the RapidXml
/// interface.
class RapidXmlSerializer: public ISerializer
{
  public:
    typedef RapidXmlSerializer SelfType;

    /// \brief Serialization options.
    ///
    /// \TODO Implement
    enum Feature
    {
      Compact = 0,  // Default
      HumanReadable // json_spirit::pretty_print (single line arrays)
    };

    RapidXmlSerializer( void );
    ~RapidXmlSerializer( void );

    /// \brief Save nom::Value values to XML nodes.
    ///
    /// \param source nom::Value container to serialize.
    /// \param output Absolute file path to save resulting data to.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    bool serialize( const Value& source, const std::string& output ) const;

    /// \brief Load data from a XML (.xml) file to a nom::Value object.
    ///
    /// \param input  Absolute file path file to un-serialize.
    /// \param dest   nom::Value container to store values in.
    ///
    /// \note Implements IJsonSerializer::unserialize interface.
    bool unserialize( const std::string& input, Value& dest ) const;

  private:
    // ...
};

} // namespace nom

#endif // include guard defined

/// \class nom::RapidXmlSerializer
/// \ingroup xml
///
///   [TO BE WRITTEN]
///
/// See examples/values.cpp for usage examples.
///
