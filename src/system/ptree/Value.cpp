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
#include "nomlib/system/ptree/Value.hpp"

// Forward declarations
#include "nomlib/system/ptree/ValueIterator.hpp"
#include "nomlib/system/ptree/ValueConstIterator.hpp"

namespace nom {

// Static initialization
// const Value Value::null = Value();

Value::Value( void )  :
  type_ ( ValueType::Null ) // type 0
{
  // NOM_LOG_TRACE(NOM);
}

// FIXME
Value::~Value( void )
{
  // NOM_LOG_TRACE(NOM);

  if( this->array_valid() )
  {
    // this->value_.array_->clear();
    for( auto itr = this->value_.array_->begin(); itr != this->value_.array_->end(); ++itr )
    {
      if( itr->array_valid() )
      {
        // NOM_DUMP("a");
        // delete itr->value_.array_;
      }
      else if( itr->object_valid() )
      {
        // NOM_DUMP("o");
      }
    }
    // delete this->value_.array_;
    // this->value_.array_ = nullptr;
  }
  else if( this->object_valid() )
  {
    // this->value_.object_->clear();
    for( auto itr = this->value_.object_->end(); itr != this->value_.object_->begin(); --itr )
    {
      if( itr->second.array_valid() )
      {
        // NOM_DUMP("aa");
        // delete itr->second.value_.array_;
      }
      else if( itr->second.object_valid() )
      {
        // NOM_DUMP("oo");
        // delete itr->second.value_.object_;
      }
    }
    // delete this->value_.object_;
    // this->value_.object_ = nullptr;
  }
  else if( this->string_type() )
  {
    if( this->string_allocated_ )
    {
      // See priv::duplicate_string
      free( const_cast<char*> ( this->get_cstring() ) );
      this->value_.string_ = nullptr;
    }
  }
  else
  {
    // Do nothing
  }
}

Value::Value( const Value& copy ) :
  type_ ( copy.type() ),
  string_allocated_ ( false )
{
  // NOM_LOG_TRACE(NOM);

  switch( this->type() )
  {
    default:
    case ValueType::Null:
    case ValueType::SignedInteger:
    case ValueType::UnsignedInteger:
    case ValueType::RealNumber:
    case ValueType::Boolean:
    {
      this->value_ = copy.value_;
      break;
    }

    case ValueType::String:
    {
      if( copy.get_cstring() )
      {
        uint size = strlen( copy.get_cstring() );
        this->value_.string_ = priv::duplicate_string( copy.get_cstring(), size );
        this->string_allocated_ = true;
        break;
      }
      else
      {
        this->value_.string_ = nullptr;
        this->string_allocated_ = false;
        break;
      }
    }

    case ValueType::ArrayValues:
    {
      // this->value_.array_ = new Array( *copy.value_.array_ );
      // this->array_ = copy.array_;
      this->value_.array_ = copy.value_.array_;
      break;
    }
    case ValueType::ObjectValues:
    {
      // this->value_.object_ = new Object( *copy.value_.object_ );
      // this->object_ = copy.object_;
      this->value_.object_ = copy.value_.object_;
      break;
    }
  } // end switch type
}

Value::SelfType& Value::operator =( const SelfType& other )
{
  this->value_ = other.value_;
  // this->array_ = other.array_;
  // this->value_.array_ = other.value_.array_;
  // this->object_ = other.object_;
  // this->value_.object_ = other.value_.object_;
  this->type_ = other.type();
  this->string_allocated_ = other.string_allocated_;

  return *this;
}

Value::Value( sint val ) :
  type_ ( ValueType::SignedInteger ) // type 1
{
  //NOM_LOG_TRACE(NOM);
  this->value_.int_ = val;
}

Value::Value( uint val ) :
  type_ ( ValueType::UnsignedInteger ) // type 2
{
  //NOM_LOG_TRACE(NOM);
  this->value_.uint_ = val;
}

Value::Value( double val ) :
  type_ ( ValueType::RealNumber ) // type 3
{
  //NOM_LOG_TRACE(NOM);
  this->value_.real_ = val;
}

Value::Value( const char* val ) :
  type_ ( ValueType::String ) // type 4
{
  //NOM_LOG_TRACE(NOM);
  uint size = strlen( val );
  this->value_.string_ = priv::duplicate_string( val, size );
  this->string_allocated_ = true;
}

Value::Value( const std::string& val ) :
  type_ ( ValueType::String ) // type 4
{
  //NOM_LOG_TRACE(NOM);
  uint size = val.length();
  this->value_.string_ = priv::duplicate_string( val.c_str(), size );
  this->string_allocated_ = true;
}

Value::Value( bool val ) :
  type_ ( ValueType::Boolean ) // type 5
{
  //NOM_LOG_TRACE(NOM);
  this->value_.bool_ = val;
}

Value::Value( const std::vector<SelfType>& val ) :
  type_ ( ValueType::ArrayValues ) // type 6

{
  //NOM_LOG_TRACE(NOM);
  this->value_.array_ = new Array( val );
  // this->array_ = val;
}

Value::Value( const Object& val ) :
  type_ ( ValueType::ObjectValues ) // type 7
{
  //NOM_LOG_TRACE(NOM);
  this->value_.object_ =  new Object( val );
  // this->object_ = val;
}

Value::RawPtr Value::get( void )
{
  return this;
}

const Value::Reference Value::ref( void )
{
  return *this;
}

enum Value::ValueType Value::type( void ) const
{
  return this->type_;
}

const std::string Value::type_name( void ) const
{
  switch ( this->type() )
  {
    default: // Type 0
    case ValueType::Null: return "Null"; break;
    case ValueType::SignedInteger: return "SignedInteger"; break; // Type 1
    case ValueType::UnsignedInteger: return "UnsignedInteger"; break; // Type 2
    case ValueType::RealNumber: return "RealNumber"; break; // Type 3
    case ValueType::String: return "String"; break; // Type 4
    case ValueType::Boolean: return "Boolean"; break; // Type 5
    case ValueType::ArrayValues: return "ArrayValues"; break; // Type 6
    case ValueType::ObjectValues: return "ObjectValues"; break; // Type 7
  }
}

bool Value::null_type( void ) const
{
  return ( this->type() == ValueType::Null );
}

bool Value::int_type( void ) const
{
  return ( this->type() == ValueType::SignedInteger );
}

bool Value::uint_type( void ) const
{
  return ( this->type() == ValueType::UnsignedInteger );
}

bool Value::double_type( void ) const
{
  return ( this->type() == ValueType::RealNumber );
}

bool Value::string_type( void ) const
{
  return ( this->type() == ValueType::String );
}

bool Value::bool_type( void ) const
{
  return ( this->type() == ValueType::Boolean );
}

bool Value::array_type( void ) const
{
  return ( this->type() == ValueType::ArrayValues );
}

bool Value::object_type( void ) const
{
  return ( this->type() == ValueType::ObjectValues );
}

const std::string Value::stringify( void ) const
{
  if( this->null_type() )
  {
    return "null";
  }
  else if( this->int_type() )
  {
    return std::to_string( this->get_int() );
  }
  else if( this->uint_type() )
  {
    return std::to_string( this->get_uint() );
  }
  else if( this->double_type() )
  {
    return std::to_string( this->get_double() );
  }
  else if( this->string_type() )
  {
    return this->get_string();
  }
  else if( this->bool_type() )
  {
    // Convert boolean value to a std::string
    if( this->get_bool() ) return "true";

    return "false";
  }
  else if( this->array_type() )
  {
    return "\0";
  }
  else if( this->object_type() )
  {
    return "\0";
  }
  else
  {
    return "\0";
  }
}

// TODO
// const std::string Value::key( void ) const
// {
//   return "\0";
// }

sint Value::get_int ( void ) const
{
  NOM_ASSERT( this->int_type() );

  if( this->int_type() ) return this->value_.int_;

  return 0; // Not found
}

uint Value::get_uint ( void ) const
{
  NOM_ASSERT( this->uint_type() );

  if( this->uint_type() ) return this->value_.uint_;

  return 0; // Not found
}

double Value::get_double ( void ) const
{
  NOM_ASSERT( this->double_type() );

  if( this->double_type() ) return this->value_.real_;

  return 0; // Not found
}

float Value::get_float ( void ) const
{
  NOM_ASSERT( this->double_type() );

  if( this->double_type() )
  {
    return static_cast<float> ( this->get_double() );
  }

  return 0; // Not found
}

const char* Value::get_cstring ( void ) const
{
  NOM_ASSERT( this->string_type() );

  if( this->string_type() )
  {
    uint size = strlen( this->value_.string_ );
    return priv::duplicate_string( this->value_.string_, size );
    // return this->value_.string_;
  }

  return nullptr; // If no string value is found
}

const std::string Value::get_string ( void ) const
{
  NOM_ASSERT( this->string_type() );

  if( this->string_type() )
  {
    // char* to std::string
    return std::string( this->get_cstring() );
  }

  return "\0"; // Null-terminated string if no string value is found
}

bool Value::get_bool ( void ) const
{
  NOM_ASSERT( this->bool_type() );

  if( this->bool_type() )
  {
    return this->value_.bool_;
  }

  return false;
}

bool Value::array_valid( void ) const
{
  if( this->array_type() )
  {
    if( this->value_.array_ != nullptr )
    {
      return true;
    }
  }

  return false;
}

bool Value::object_valid( void ) const
{
  if( this->object_type() )
  {
    if( this->value_.object_ != nullptr )
    {
      return true;
    }
  }

  return false;
}

const Array Value::array( void ) const
{
  NOM_ASSERT( this->array_type() );

  if( this->array_valid() )
  {
    return Array( *this->value_.array_ );
  }

  // Err; not initialized..!
  return Array();

  // return this->array_;
}

const Object Value::object( void ) const
{
  NOM_ASSERT( this->object_type() );

  if( this->object_valid() )
  {
    return Object( *this->value_.object_ );
  }

  // Err; not initialized..!
  return Object();

  // return this->object_;
}

uint Value::size( void ) const
{
  switch( this->type() )
  {
    default: return 1; break; // Non-NULL, non-array, non-object value type

    case ValueType::Null: return 0; break; // Empty

    case ValueType::ArrayValues:
    {
      if( this->array_valid() )
      {
        return this->value_.array_->size();
        // return this->array_.size();
      }
      return 0; // Not initialized

      break;
    }
    case ValueType::ObjectValues:
    {
      if( this->object_valid() )
      {
        return this->value_.object_->size();
        // return this->object_.size();
      }
      return 0; // Not initialized

      break;
    }
  }
}

void Value::clear( void )
{
  NOM_ASSERT( this->null_type() || this->array_type() || this->object_type() );

  switch( this->type() )
  {
    // Not an array or object type
    default: /* Do nothing */break;

    case ValueType::ArrayValues:
    {
      if( this->array_valid() )
      {
        this->value_.array_->clear();
        // this->array_.clear();
      }
      break;
    }
    case ValueType::ObjectValues:
    {
      if( this->object_valid() )
      {
        this->value_.object_->clear();
        // this->object_.clear();
      }
      break;
    }
  }
}

Value::ConstIterator Value::begin( void ) const
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::ConstIterator itr = this->value_.array_->begin();
    // Value::ConstIterator itr = this->array_.begin();
    // Value::ConstIterator itr = this->array_[0].begin();
    return Value::ConstIterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::ConstIterator itr = this->value_.object_->begin();
    // Value::ConstIterator itr = this->object_.begin();
    // ObjectConstIterator itr = this->object_.begin();
    return Value::ConstIterator( itr );
  }

  // Error; returning NullValue object
  return ValueConstIterator(); // Not an array or object type
}

Value::ConstIterator Value::end( void ) const
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::ConstIterator itr = this->value_.array_->end();
    // Value::ConstIterator itr = this->array_.end();
    return Value::ConstIterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::ConstIterator itr = this->value_.object_->end();
    // Value::ConstIterator itr = this->object_.end();
    return Value::ConstIterator( itr );
  }

  // Error; returning NullValue object
  return ValueConstIterator(); // Not an array or object type
}

Value::Iterator Value::begin( void )
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::Iterator itr = this->value_.array_->begin();
    // Value::Iterator itr = this->array_.begin();
    return Value::Iterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::Iterator itr = this->value_.object_->begin();
    // Value::Iterator itr = this->object_.begin();
    return Value::Iterator( itr );
  }

  // Error; returning NullValue object
  return ValueIterator(); // Not an array or object type
}

Value::Iterator Value::end( void )
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::Iterator itr = this->value_.array_->end();
    // Value::Iterator itr = this->array_.end();
    return Value::Iterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::Iterator itr = this->value_.object_->end();
    // Value::Iterator itr = this->object_.end();
    return Value::Iterator( itr );
  }

  // Error; returning NullValue object
  return ValueIterator(); // Not an array or object type
}

const std::string Value::dump_key( const Value& key ) const
{
  std::stringstream os; // Output buffer

  if( key.null_type() )
  {
    os << this->print_key( key.type_name(), 0 );
  }
  else if( key.int_type() )
  {
    os << this->print_key( key.type_name(), sizeof( key.get_int() ) );
  }
  else if( key.uint_type() )
  {
    os << this->print_key( key.type_name(), sizeof( key.get_uint() ) );
  }
  else if( key.double_type() )
  {
    os << this->print_key( key.type_name(), sizeof( key.get_double() ) );
  }
  else if( key.string_type() )
  {
    os << this->print_key( key.type_name(), sizeof( key.get_string() ) );
  }
  else if( key.bool_type() )
  {
    os << this->print_key( key.type_name(), sizeof( key.get_bool() ) );
  }
  else if( key.array_type() )
  {
    os << this->print_key( key.type_name(), key.size() );
  }
  else if( key.object_type() )
  {
    os << this->print_key( key.type_name(), key.size() );
  }
  else // Unknown type
  {
    os << this->print_key( key.type_name(), 0 );
  }

  return os.str();
}

const std::string Value::dump_value( const Value& val ) const
{
  std::stringstream os; // Output buffer

  if( val.null_type() ) // Type 0
  {
    os << this->print_value( "null" );
  }
  else if( val.int_type() ) // Type 1
  {
    os << this->print_value( val.stringify() );
  }
  else if( val.uint_type() ) // Type 2
  {
    os << this->print_value( val.stringify() );
  }
  else if( val.double_type() ) // Type 3
  {
    os << this->print_value( val.stringify() );
  }
  else if( val.string_type() ) // Type 4
  {
    os << this->print_value( val.stringify() );
  }
  else if( val.bool_type() ) // Type 5
  {
    os << this->print_value( val.stringify() );
  }
  else if( val.array_type() ) // Type 6
  {
    os << this->print_value( "array" );
  }
  else if( val.object_type() ) // Type 7
  {
    os << this->print_value( "object" );
  }
  else // Unknown type
  {
    os << "Unknown (" << val.type() << ")";
  }

  return os.str();
}

const std::string Value::dump( const Value& object, int depth ) const
{
  std::stringstream os; // Output buffer

  depth += 1;

  os << this->dump_key( object );

  // nom::Value object is a tree of key & value pairs.
  if( object.size() > 0 )
  {
    os << std::endl;

    for( Value::ConstIterator itr = object.begin(); itr != object.end(); ++itr )
    {
      Value::ConstIterator member( itr );

      // Show the current depth.
      for ( uint tab = 0 ; tab < depth; ++tab )
      {
        os << "-";
      }

      os << " subvalue (";

      // Dump the member key name of the key/value pair
      if( member.key() != "" )
      {
        os << member.key();
      }
      else
      {
        os << this->dump_value( member->ref() );
      }

      // Dump the individual values of each member key/value pair
      if( ! member->array_type() && ! member->object_type() )
      {
        os << ": ";
        os << this->dump_value( member->ref() );
        os << ")";
      }
      else
      {
        os << ") -";
      }

      // Iterate onwards to the next level of the tree
      os << this->dump( *itr, depth );
    }

    return os.str();
  }
  else
  {
    // os << " ";
    // os << this->dump_value( object );

    // Move onto the next element of the tree!
    os << std::endl;
  }

  return os.str();
}

const std::string Value::print_key( const std::string& type, uint size ) const
{
  std::stringstream os;

  os << " [type=" << type << ", " << "size=" << size << "]";

  return os.str();
}

const std::string Value::print_value( const std::string& val ) const
{
  std::stringstream os;

  os << val;

  return os.str();
}

std::ostream& operator <<( std::ostream& os, const Value& val )
{
  os << val.dump( val );

  return os;
}

/* Experimental
void Value::append( const Object& val )
{
  if( ! this->object_valid() )
  {
    this->type_ = ValueType::ObjectValues;  // Type 7
    this->value_.object_ = new Object( val );
  }

  // this->value_.object_->push_back( Value(val) );
  for( auto itr = val.begin(); itr != val.end(); ++itr )
  {
    this->value_.object_->push_back( Pair( itr->key, itr->value ) );
  }
}
*/

namespace priv {

char* duplicate_string( const char* val, uint length )
{
  // Buffer overflow protection
  if( length >= MAX_STRING_LENGTH )
  {
    length = priv::MAX_STRING_LENGTH - 1;
  }

  // Allocate memory for duplicating the C string
  char* duplicate_string = static_cast<char*> ( malloc( length + 1 ) );
  std::memcpy( duplicate_string, val, length );

  // Null-terminate
  duplicate_string[length] = 0;

  return duplicate_string;
}

} // namespace priv

} // namespace nom
