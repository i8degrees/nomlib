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
#ifndef NOMLIB_XML_RAPIDXML_DESERIALIZER_HPP
#define NOMLIB_XML_RAPIDXML_DESERIALIZER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

// RapidXml library
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "nomlib/config.hpp"
#include "nomlib/serializers/serializers_config.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/ptree.hpp"

/// \brief Enable dumping output of each key, value pair, sizes, etc. as we
/// traverse the object.
// #define NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES

namespace nom {

/// \brief Loading of nom::Value objects from XML documents using the RapidXml
/// library.
class RapidXmlDeserializer: public IValueDeserializer
{
  public:
    typedef RapidXmlDeserializer SelfType;

    /// \brief Default constructor; initialize the parser with no additional
    /// options.
    ///
    /// \remarks No options are currently implemented for the XML parser.
    RapidXmlDeserializer( void );

    /// \brief Destructor.
    ~RapidXmlDeserializer( void );

    /// \brief Construct a parser with a feature set enumeration.
    ///
    /// \remarks No options are currently implemented for the XML parser.
    RapidXmlDeserializer( uint32 options );

    /// \brief Parse XML data to a nom::Value object.
    ///
    /// \param source std::string containing valid XML.
    ///
    /// \returns nom::Value object filled from XML-compliant input on success,
    /// or nom::Value::null on err.
    ///
    /// \note Implements IDeserializer interface.
    Value deserialize ( const std::string& source );

    /// \brief Restore data from a XML (.xml) file to a nom::Value object.
    ///
    /// \param filename Absolute file path to data to deserialize from.
    /// \param output nom::Value container to store resulting data in.
    ///
    /// \note Implements IDeserializer interface.
    bool load ( const std::string& filename, Value& output );

  private:
    uint32 options_;

    // rapidxml::xml_document<> doc;

    bool read( const std::string& input, Value& dest ) const;

    bool read_value( const rapidxml::xml_node<>& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of array nodes.
    ///
    /// \remarks rapidxml::xml_attribute<>
    bool read_array( const rapidxml::xml_node<>* object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of object nodes.
    bool read_object( const rapidxml::xml_node<>* node, Value& dest ) const;
};

} // namespace nom

#endif // include guard defined

/// \class nom::RapidXmlDeserializer
/// \ingroup xml
///
///   [TO BE WRITTEN]
///
/// References
///
/// 1. http://sfmlcoder.wordpress.com/2011/05/29/a-lightweight-xml-parser-rapid-xml/
/// 2. http://www.w3schools.com/xml/xml_attributes.asp
///
/// \TODO Implement XML comments
///
/// \TODO Finish implementation of ::serialize_object & related case handling,
/// err handling
///
