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
#include "nomlib/serializers/RapidXmlDeserializer.hpp"

namespace nom {

RapidXmlDeserializer::RapidXmlDeserializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

RapidXmlDeserializer::~RapidXmlDeserializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

Value RapidXmlDeserializer::deserialize ( const std::string& source )
{
  Value output;

  // Transform std::string filled with XML document to nom::Value
  if( this->read( source, output ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not deserialize the input source object." );
    return Value::null;
  }

  return output;
}

bool RapidXmlDeserializer::load ( const std::string& filename, Value& output )
{
  std::ifstream fp;
  std::stringstream buffer;

  fp.open( filename );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Could not access file: " + filename );
    return false;
  }

  buffer << fp.rdbuf();

  // File input to nom::Value
  output = this->deserialize( buffer.str() );

  fp.close();

  return true;
}

bool RapidXmlDeserializer::read( const std::string& input, Value& dest ) const
{
  int err_count = 0; // C++ exception handling
  rapidxml::xml_document<> doc;

  // Make a copy of the input buffer, just in case!
  std::vector<char> buffer( input.begin(), input.end() );
  buffer.push_back('\0');

  try
  {
    // 'parse_no_data_nodes' prevents RapidXML from using the somewhat surprising
    // behavior of having both values and data nodes, and having data nodes take
    // precedence over values when printing.
    //
    // Note that this will skip parsing of CDATA nodes
    //
    // Source: http://www.setnode.com/blog/quick-notes-on-how-to-use-rapidxml/
    //
    // FIXME:
    //
    // Enabling the flag 'rapidxml::parse_declaration_node' messes up our
    // parsing of the document, due to the non-handling of the node_declaration
    // node type -- see ::read_object.
    // doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>( &buffer[0 ]);
    doc.parse<rapidxml::parse_no_data_nodes>( &buffer[0 ]);

    // Object obj, objects;
    Value obj, objects;
    for( rapidxml::xml_node<> *node = doc.first_node(); node; node = node->next_sibling() )
    {
      std::string node_key = node->name();

      #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
        NOM_DUMP( node_key );
      #endif

      if( this->read_object( node, obj[node_key] ) == false )
      {
        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;
      }

      objects[node_key] = obj;
    }

    dest = obj;
  }
  catch( rapidxml::parse_error &e)
  {
    ++err_count;

    NOM_LOG_ERR( NOM, "Errors counted: " + std::to_string( err_count ) );
    NOM_LOG_ERR( NOM, e.what() );

    return false;
  }

  return true;
}

bool RapidXmlDeserializer::read_value( const rapidxml::xml_node<>& object, Value& dest ) const
{
  NOM_STUBBED( NOM );

  return false;
}

bool RapidXmlDeserializer::read_array( const rapidxml::xml_node<>* object, Value& dest ) const
{
  std::string key;
  std::string value;
  Value arr;

  for( rapidxml::xml_attribute<> *attr = object->first_attribute(); attr; attr = attr->next_attribute() )
  {
    key = attr->name();
    value = attr->value();

    #if defined( NOMLIB_XML_RAPIDXML_DESERIALIZER_HPP )
      NOM_DUMP( key );
      NOM_DUMP( value );
    #endif

    arr.push_back( attr->value() );
  }

  dest = arr;

  return true;
}

bool RapidXmlDeserializer::read_object( const rapidxml::xml_node<>* node, Value& dest ) const
{
  std::string key;
  std::string value;
  Value obj;
  Value arr;

  switch( node->type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
        NOM_DUMP( node->type() );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      // return false;

      break;
    }

    case rapidxml::node_document:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_element:
    {
      for( rapidxml::xml_node<> *object = node->first_node(); object; object = object->next_sibling() )
      {
         key = object->name();
         value = object->value();

        if( key != "" )
        {
          #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
            NOM_DUMP(key);
          #endif

          this->read_object( object, obj[key] );

          if( value != "" )
          {
            #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
               NOM_DUMP( value );
            #endif

             obj[key] = value;
          }

          this->read_array( object, arr );

          if( arr.size() > 0 )
          {
            obj[key] = arr;
          }
        }
      }

      break;
    }

    case rapidxml::node_data:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_cdata:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_comment:
    {
      // TODO: Handle case
      break;
    }

    // TODO: Handle case
    case rapidxml::node_declaration:
    {
      for( rapidxml::xml_node<> *object = node->first_node(); object; object = object->next_sibling() )
      {
         key = object->name();
         value = object->value();

        if( key != "" )
        {
          #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
            NOM_DUMP(key);
          #endif

          this->read_object( object, obj[key] );

          if( value != "" )
          {
            #if defined( NOM_DEBUG_RAPIDXML_DESERIALIZER_VALUES )
               NOM_DUMP( value );
            #endif

             // obj[key] = value;
          }
        }
      }

      break;
    }

    case rapidxml::node_doctype:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_pi:
    {
      // TODO: Handle case
      break;
    }
  }

  dest = obj;

  return true;
}

} // namespace nom
