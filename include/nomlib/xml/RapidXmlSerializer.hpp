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

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

// RapidXml library
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

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
///
/// \TODO Implement feature
const std::string RAPIDXML_INDENTION_LEVEL = "  ";

/// \brief Serialization of nom::Value objects to and fro XML using the
/// RapidXml library.
class RapidXmlSerializer: public ISerializer
{
  public:
    typedef RapidXmlSerializer SelfType;

    RapidXmlSerializer( void );
    ~RapidXmlSerializer( void );

    /// \brief Output a nom::Value object to a string.
    ///
    /// \param source nom::Value container to serialize from.
    ///
    /// \returns std::string with XML-conforming output on success, or a null-
    /// terminated string on err.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    std::string serialize ( const Value& source,
                            // enum Features options = HumanReadable
                           enum Features options = Compact
                          );

    /// \brief Parse JSON data as a nom::Value object.
    ///
    /// \param source std::string containing valid XML.
    ///
    /// \returns nom::Value object filled from XML-compliant input on success,
    /// or nom::Value::null on err.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    Value unserialize ( const std::string& source,
                        // enum Features options = HumanReadable
                        enum Features options = Compact
                      );

    /// \brief Save nom::Value values to XML nodes.
    ///
    /// \param source nom::Value container to serialize from.
    /// \param filename Absolute file path to output resulting data to.
    ///
    /// \remarks The source input will automatically have the standard XML
    /// DOCTYPE prepended onto the resulting output. See ::append_decl method
    /// for details.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    bool save ( const Value& source,
                const std::string& filename,
                // enum Features options = HumanReadable
                enum Features options = Compact
              );

    /// \brief Load data from a XML (.xml) file to a nom::Value object.
    ///
    /// \param filename Absolute file path to data to unserialize from.
    /// \param output nom::Value container to store resulting data in.
    ///
    /// \note Implements IJsonSerializer::serialize interface.
    bool load ( const std::string& filename,
                Value& output,
                // enum Features options = HumanReadable
                enum Features options = Compact
              );

    // const std::string stringify( const Value& source );

  private:
    bool write( const Value& source, rapidxml::xml_document<>& dest ) const;

    bool read( const std::string& input, Value& dest ) const;

    bool write_value( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const;

    /// \brief Internal helper method for serialization of array nodes.
    bool serialize_array( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const;

    /// \brief Internal helper method for serialization of object nodes.
    bool serialize_object( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const;

    bool read_value( const rapidxml::xml_node<>& object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of array nodes.
    ///
    /// \remarks rapidxml::xml_attribute<>
    bool unserialize_array( const rapidxml::xml_node<>* object, Value& dest ) const;

    /// \brief Internal helper method for un-serialization of object nodes.
    bool unserialize_object( const rapidxml::xml_node<>* node, Value& dest ) const;

    const char* stralloc( const std::string& buffer, rapidxml::xml_document<>& dest ) const;

    /// \brief Add the top-level XML node declaring the version and encoding
    /// used (think: DOCTYPE).
    ///
    /// \param dest The document to append the declaration onto.
    ///
    /// \remarks This is automatically added to all serialized XML output.
    ///
    /// \code
    /// <?xml version="1.0" encoding="utf-8"?>
    /// \endcode
    void append_decl( rapidxml::xml_document<>& dest ) const;
};

/// \brief Output the XML document object.
///
/// \remarks Utilizes the public stringify method of this class interface.
/// \TODO
// std::ostream& operator <<( std::ostream& os, const Value& val );

} // namespace nom

#endif // include guard defined

/// \class nom::RapidXmlSerializer
/// \ingroup xml
///
///   [TO BE WRITTEN]
///
/// See examples/values.cpp for usage examples.
///
/// References
///
/// 1. http://sfmlcoder.wordpress.com/2011/05/29/a-lightweight-xml-parser-rapid-xml/
/// 2. http://www.w3schools.com/xml/xml_attributes.asp
///
/// \TODO Implement XML comments
///
/// \TODO Finish implementation of ::serialize_object & related case handling,
/// err handling, unit tests, etc.
///
