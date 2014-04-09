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
#include "nomlib/serializers/RapidXmlSerializer.hpp"

namespace nom {

RapidXmlSerializer::RapidXmlSerializer( void ) :
  options_{ SerializerOptions::HumanFriendly }
{
  //NOM_LOG_TRACE(NOM);
}

RapidXmlSerializer::~RapidXmlSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

RapidXmlSerializer::RapidXmlSerializer( enum SerializerOptions options ) :
  options_{ options }
{
  //NOM_LOG_TRACE(NOM);
}

std::string RapidXmlSerializer::serialize ( const Value& source )
{
  rapidxml::xml_document<> buffer;

  // TODO:
  //
  // Append XML DOCTYPE to very top of document
  // this->append_decl( buffer );

  // Output the XML document to a buffer for further processing.
  if( this->write( source, buffer ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not serialize the source object." );
    return "\0";
  }

  if( this->options_ == SerializerOptions::Compact )
  {
    std::string os;

    rapidxml::print ( std::back_inserter( os ),
                      buffer,
                      rapidxml::print_no_indenting
                    );

    return os;
  }
  else if( this->options_ == SerializerOptions::WriteComments )
  {
    // TODO: Handle comments serialization
    NOM_STUBBED( NOM );
    return "\0";
  }
  else // SerializerOptions::HumanFriendly
  {
    std::stringstream os;

    os << buffer;

    return os.str();
  }
}

bool RapidXmlSerializer::save ( const Value& source, const std::string& filename )
{
  std::ofstream fp;

  fp.open( filename );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Could not save output to file: " + filename );
    return false;
  }

  // nom::Value to file
  fp << this->serialize( source );

  fp.close();

  return true;
}

bool RapidXmlSerializer::write( const Value& source, rapidxml::xml_document<>& dest ) const
{
  std::string root_key, key;

  // Top-level node is an array -- this is not supported.
  if( source.array_type() )
  {
    return false;
  }

  // Iterate through each object and store every member's key / value pair
  // we find
  for( Value::ConstIterator itr = source.begin(); itr != source.end(); ++itr )
  {
    ValueConstIterator member( itr );

    key = member.key();

    #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
      NOM_DUMP( itr->type_name() );
      NOM_DUMP( key );
    #endif

    rapidxml::xml_node<>* root_node = dest.allocate_node( rapidxml::node_element, this->stralloc( key, dest ) );

    #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
      NOM_DUMP( itr->type_name() );
      NOM_DUMP( key );
    #endif

    if( this->write_object( itr->ref(), key, dest, root_node ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
      return false;
    }

    dest.append_node( root_node );
  } // end for source object loop

  return true;
}

bool RapidXmlSerializer::write_value( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  switch( object.type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    // Append the node, but without a value.
    case Value::ValueType::Null:
    {
      rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( "null", doc ) );
      parent->append_node( val );
      break;
    }

    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP(key);
        NOM_DUMP(object.stringify());
      #endif

      // Stupid hack to get things going ... no idea why this works, but nothing
      // else will. I probably have my node ordering all messed up ...
      if( key != "" )
      {
        rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( key, doc ), this->stralloc( object.stringify(), doc ) );
        parent->append_node( val );
      }
      else
      {
        rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_data, this->stralloc( key, doc ), this->stralloc( object.stringify(), doc ) );
        parent->append_node( val );
      }
      break;
    }

    case Value::ValueType::ArrayValues:
    {
      // Special case -- must be handled by a dedicated method in order to
      // support deep-depth hierarchies.
      break;
    }

    case Value::ValueType::ObjectValues:
    {
      // Special case -- must be handled by a dedicated method in order to
      // support deep-depth hierarchies.
      break;
    }
  }

  return true;
}

bool RapidXmlSerializer::write_array( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  Value array;

  // NOM_DUMP( member_value );

  for( auto itr = array.begin(); itr != array.end(); ++itr )
  {
    switch( itr->type() )
    {
      default: // Unknown type
      {
        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( itr->type() );
          NOM_DUMP( itr->ref() );
        #endif

        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;

        break;
      }

      case Value::ValueType::Null:
      {
        break;
      }

      case Value::ValueType::SignedInteger:
      case Value::ValueType::UnsignedInteger:
      case Value::ValueType::RealNumber:
      case Value::ValueType::String:
      case Value::ValueType::Boolean:
      {
        std::string member_value = itr->stringify();

        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( key );
          NOM_DUMP( member_value );
        #endif

        // FIXME:
        //
        rapidxml::xml_attribute<>* attrib = doc.allocate_attribute( this->stralloc( key, doc ), this->stralloc( member_value, doc ) );
        parent->append_attribute( attrib );

        break;
      }

      case Value::ValueType::ArrayValues:
      {
        NOM_LOG_ERR( NOM, "Could not serialize values; an array cannot exist within another array." );
        return false;

        break;
      }

      case Value::ValueType::ObjectValues:
      {
        if( this->write_object( itr->ref(), key, doc, parent ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
          return false;
        }

        break;
      }
    }
  } // end for array nodes loop

  return true;
}
bool RapidXmlSerializer::write_object( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  switch( object.type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    // Object within object containing values (top-level).
    case Value::ValueType::Null:
    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      if( this->write_value( object, "", doc, parent ) == false )
      {
        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;
      }

      break;
    }

    // Handle arrays within an object (top-level).
    case Value::ValueType::ArrayValues:
    {
      // Error -- not supported (we need a member key!)

      // TODO: Verify functionality
      //
      // if( this->write_array( object, "", doc, parent ) == false )
      // {
      //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
      //   return false;
      // }

      // TODO: Err handling
      NOM_STUBBED( NOM );
      // return false;

      break;
    }

    case Value::ValueType::ObjectValues:
    {
      for( auto itr = object.begin(); itr != object.end(); ++itr )
      {
        ValueConstIterator member( itr );
        Value value = itr->ref();

        std::string object_key = member.key();

        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( object_key );
        #endif

        switch( value.type() )
        {
          default: // Unknown type
          {
            #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
              NOM_DUMP( value.type() );
              NOM_DUMP( object_key );
              NOM_DUMP( value );
            #endif

            // TODO: Err handling
            NOM_STUBBED( NOM );
            return false;

            break;
          }

          case Value::ValueType::Null:
          case Value::ValueType::SignedInteger:
          case Value::ValueType::UnsignedInteger:
          case Value::ValueType::RealNumber:
          case Value::ValueType::String:
          case Value::ValueType::Boolean:
          {
            if( this->write_value( value, object_key, doc, parent ) == false )
            {
              // TODO: Err handling
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle arrays within an object within an object.
          case Value::ValueType::ArrayValues:
          {
            // FIXME:
            rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( object_key, doc ) );
            parent->append_node( val );

            if( this->write_array( value, object_key, doc, parent ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle iterating through objects within objects (unlimited depth).
          case Value::ValueType::ObjectValues:
          {
            rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( object_key, doc ) );
            parent->append_node( val );

            if( this->write_object( value, key, doc, val ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

        } // end switch objects[key] type
      } // end for object loop
    } // end Value::ValueType::ObjectValues type
  } // end switch object type

  return true;
}

void RapidXmlSerializer::append_decl( rapidxml::xml_document<>& dest ) const
{
  // Top-level XML DOCTYPE
  rapidxml::xml_node<>* decl = dest.allocate_node( rapidxml::node_declaration );
  decl->append_attribute( dest.allocate_attribute("version", "1.0") );
  decl->append_attribute( dest.allocate_attribute("encoding", "utf-8") );
  dest.append_node( decl );
}

const char* RapidXmlSerializer::stralloc( const std::string& buffer, rapidxml::xml_document<>& dest ) const
{
  return dest.allocate_string( buffer.c_str() );
}

} // namespace nom
