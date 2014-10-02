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
#include "nomlib/ptree/Value.hpp"

// Forward declarations
#include "nomlib/ptree/ValueIterator.hpp"
#include "nomlib/ptree/ValueConstIterator.hpp"

namespace nom {

// Static initializations
const Value Value::null = Value();

Value::Value( void )  :
  type_ ( ValueType::Null )
{
  // NOM_LOG_TRACE(NOM);
}

// FIXME
Value::~Value( void )
{
  // NOM_LOG_TRACE(NOM);
/*
  if( this->array_valid() )
  {
    // this->value_.array_->clear();
    for( auto itr = this->value_.object_->begin(); itr != this->value_.object_->end(); ++itr )
    // for( auto itr = this->value_.array_->begin(); itr != this->value_.array_->end(); --itr )
    {
      if( itr->array_valid() )
      {
        // NOM_DUMP("a");
        // delete itr->value_.array_;
      }
      else if( itr->object_valid() )
      {
        // NOM_DUMP("o");
        // delete this->value_.object_;
      }
    }
    // delete this->value_.array_;
    // this->value_.array_ = nullptr;
  }
*/
  // else
  if( this->object_valid() )
  {
    // this->value_.object_->clear();
    // for( auto itr = this->value_.object_->end(); itr != this->value_.object_->begin(); --itr )
    for( auto itr = this->value_.object_->begin(); itr != this->value_.object_->end(); ++itr )
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
    // this->value_.object_->clear();
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
    {
      // Do nothing
      break;
    }
    case ValueType::SignedInteger:
    {
      this->value_.int_ = copy.value_.int_;
      break;
    }
    case ValueType::UnsignedInteger:
    {
      this->value_.uint_ = copy.value_.uint_;
      break;
    }
    case ValueType::RealNumber:
    {
      this->value_.real_ = copy.value_.real_;
      break;
    }

    case ValueType::Boolean:
    {
      this->value_.bool_ = copy.value_.bool_;
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
    case ValueType::ObjectValues:
    {
      this->value_.object_ = copy.value_.object_;
      break;
    }
  } // end switch type
}

Value::SelfType& Value::operator =( const SelfType& other )
{
  Value temp( other );

  this->swap( temp );

  return *this;
}

void Value::swap( Value& other )
{
   Value::ValueType temp = this->type();

   this->type_ = other.type();
   other.type_ = temp;
   std::swap( this->value_, other.value_ );

   int temp2 = this->string_allocated_;

   this->string_allocated_ = other.string_allocated_;
   other.string_allocated_ = temp2;
}

bool Value::operator <( const Value& other ) const
{
  int type_delta = this->type() - other.type();

  if( type_delta )
  {
    return type_delta < 0 ? true : false;
  }

  switch( this->type() )
  {
    default:
    case ValueType::Null:
    {
      return false;
      break;
    }

    case ValueType::SignedInteger:
    {
      return this->value_.int_ < other.value_.int_;
      break;
    }

    case ValueType::UnsignedInteger:
    {
      return this->value_.uint_ < other.value_.uint_;
      break;
    }

    case ValueType::RealNumber:
    {
      return this->value_.real_ < other.value_.real_;
      break;
    }

    case ValueType::Boolean:
    {
      return this->value_.bool_ < other.value_.bool_;
      break;
    }

    case ValueType::String:
    {
      return( this->value_.string_ == 0   && other.value_.string_ )
              || ( other.value_.string_   &&  this->value_.string_
                                          && strcmp
                ( this->value_.string_, other.value_.string_ ) < 0 );
      break;
    }

    case ValueType::ArrayValues:
    {
      int delta = int ( this->value_.object_->size() - other.value_.object_->size() );

      if( delta ) return delta < 0;

      break;

      return( *this->value_.object_ ) < ( *other.value_.object_ );

      break;
    }

    case ValueType::ObjectValues:
    {
      int delta = int ( this->value_.object_->size() - other.value_.object_->size() );

      if( delta ) return delta < 0;

      break;

      return( *this->value_.object_ ) < ( *other.value_.object_ );

      break;
    }
  } // end switch type

  return false; // Err
}

bool Value::operator <= ( const Value& other ) const
{
  return ! ( other < *this );
}

bool Value::operator >= ( const Value& other ) const
{
  return ! ( *this < other );
}

bool Value::operator > ( const Value& other ) const
{
  return ( other < *this );
}

bool Value::operator == ( const Value& other ) const
{
  //if ( type_ != other.type_ )
  // GCC 2.95.3 says:
  // attempt to take address of bit-field structure member `Json::Value::type_'
  // Beats me, but a temp solves the problem.
  int temp = other.type_;

  if ( this->type() != temp )
  {
    return false;
  }

  switch( type() )
  {
    default:
    case ValueType::Null:
    {
      return true;
      break;
    }

    case ValueType::SignedInteger:
    {
      return this->value_.int_ == other.value_.int_;
      break;
    }

    case ValueType::UnsignedInteger:
    {
      return this->value_.uint_ == other.value_.uint_;
      break;
    }

    case ValueType::RealNumber:
    {
      return this->value_.real_ == other.value_.real_;
      break;
    }

    case ValueType::Boolean:
    {
      return this->value_.bool_ == other.value_.bool_;
      break;
    }

    case ValueType::String:
    {
      return( this->value_.string_  == other.value_.string_ )
          || ( other.value_.string_ &&  this->value_.string_
          && strcmp ( this->value_.string_, other.value_.string_ ) == 0 );

      break;
    }

    case ValueType::ArrayValues:
    {
      return this->value_.object_->size() == other.value_.object_->size()
      && ( *this->value_.object_ ) == ( *other.value_.object_ );

      break;
    }

    case ValueType::ObjectValues:
    {
      return this->value_.object_->size() == other.value_.object_->size()
      && ( *this->value_.object_ ) == ( *other.value_.object_ );

      break;
    }
  } // end switch type

  return false; // Err
}

bool Value::operator != ( const Value& other ) const
{
  return ! ( *this == other );
}

bool Value::operator !( void ) const
{
  return this->null_type();
}

Value::Value( enum ValueType type ) :
  type_ ( type )
{
  // NOM_LOG_TRACE( NOM );

  switch( this->type() )
  {
    default:
    case ValueType::Null:
    case ValueType::SignedInteger:
    case ValueType::UnsignedInteger:
    case ValueType::RealNumber:
    case ValueType::Boolean:
    {
      // Do nothing extra
      break;
    }

    case ValueType::String:
    {
      // TODO: Allocate string (duplicate?)
      break;
    }

    case ValueType::ArrayValues:
    {
      this->value_.object_ = new Object();
      break;
    }

    case ValueType::ObjectValues:
    {
      this->value_.object_ = new Object();
      break;
    }
  }
}

Value::Value( sint val ) :
  type_ ( ValueType::SignedInteger )
{
  //NOM_LOG_TRACE(NOM);
  this->value_.int_ = val;
}

Value::Value( uint val ) :
  type_ ( ValueType::UnsignedInteger )
{
  //NOM_LOG_TRACE(NOM);
  this->value_.uint_ = val;
}

Value::Value( double val ) :
  type_ ( ValueType::RealNumber )
{
  //NOM_LOG_TRACE(NOM);
  this->value_.real_ = val;
}

Value::Value( const char* val ) :
  type_ ( ValueType::String )
{
  //NOM_LOG_TRACE(NOM);
  uint size = strlen( val );
  this->value_.string_ = priv::duplicate_string( val, size );
  this->string_allocated_ = true;
}

Value::Value( const std::string& val ) :
  type_ ( ValueType::String )
{
  //NOM_LOG_TRACE(NOM);
  uint size = val.length();
  this->value_.string_ = priv::duplicate_string( val.c_str(), size );
  this->string_allocated_ = true;
}

Value::Value( bool val ) :
  type_ ( ValueType::Boolean )
{
  //NOM_LOG_TRACE(NOM);
  this->value_.bool_ = val;
}

Value::Value( const Object& val ) :
  type_ ( ValueType::ObjectValues )
{
  //NOM_LOG_TRACE(NOM);
  this->value_.object_ =  new Object( val );
  // this->object_ = val;
}

// Derives from the JsonCpp library
sint Value::compare( const Value& other ) const
{
  if ( *this < other ) return -1;

  if ( *this > other ) return 1;

  return 0;
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
    default: return "\0"; break;

    case ValueType::Null: return "null"; break; // Type 0
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

sint Value::get_int ( void ) const
{
  // NOM_ASSERT( this->int_type() );

  if( this->int_type() ) return this->value_.int_;

  return 0; // Not found
}

uint Value::get_uint ( void ) const
{
  // NOM_ASSERT( this->uint_type() );

  if( this->uint_type() ) return this->value_.uint_;

  return 0; // Not found
}

double Value::get_double ( void ) const
{
  // NOM_ASSERT( this->double_type() );

  if( this->double_type() ) return this->value_.real_;

  return 0; // Not found
}

float Value::get_float ( void ) const
{
  // NOM_ASSERT( this->double_type() );

  if( this->double_type() )
  {
    return static_cast<float> ( this->get_double() );
  }

  return 0; // Not found
}

const char* Value::get_cstring ( void ) const
{
  // NOM_ASSERT( this->string_type() );

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
  // NOM_ASSERT( this->string_type() );

  if( this->string_type() )
  {
    // char* to std::string
    return std::string( this->get_cstring() );
  }

  return "\0"; // Null-terminated string if no string value is found
}

bool Value::get_bool ( void ) const
{
  // NOM_ASSERT( this->bool_type() );

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
    if( this->value_.object_ != nullptr )
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

const Object Value::array( void ) const
{
  // NOM_ASSERT( this->array_type() );

  if( this->array_valid() )
  {
    return Object( *this->value_.object_ );
  }

  // Err; not initialized..!
  return Object();
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
        return this->value_.object_->size();
      }
      return 0; // Not initialized

      break;
    }
    case ValueType::ObjectValues:
    {
      if( this->object_valid() )
      {
        return this->value_.object_->size();
      }
      return 0; // Not initialized

      break;
    }
  }
}

bool Value::empty() const
{
  NOM_ASSERT( this->null_type() || this->array_type() || this->object_type() );

  switch( this->type() )
  {
    // Not an array or object type
    default: /* Do nothing */ break;

    case ValueType::ArrayValues:
    {
      if( this->array_valid() )
      {
        return this->value_.object_->empty();
      }

      break;
    }

    case ValueType::ObjectValues:
    {
      if( this->object_valid() )
      {
        return this->value_.object_->empty();
      }

      break;
    }
  } // end switch type

  return true;
}

void Value::clear( void )
{
  NOM_ASSERT( this->null_type() || this->array_type() || this->object_type() );

  switch( this->type() )
  {
    // Not an array or object type
    default: /* Do nothing */ break;

    case ValueType::ArrayValues:
    {
      if( this->array_valid() )
      {
        this->value_.object_->clear();
      }

      break;
    }

    case ValueType::ObjectValues:
    {
      if( this->object_valid() )
      {
        this->value_.object_->clear();
      }

      break;
    }
  }
}

// Derives from JsonCpp implementation
Value& Value::operator[]( ArrayIndex index )
{
  NOM_ASSERT( this->null_type() || this->array_type() );

  if( this->null_type() )
  {
    *this = Value( ValueType::ArrayValues );
  }

  VString key( index );

  // Returns an iterator pointing to the first element in the container whose
  // key is not considered to go before k -- key_type -- (i.e., either it is
  // equivalent or goes after).
  ObjectIterator it = this->value_.object_->lower_bound( key );

  if( it != this->value_.object_->end() && (*it).first == key )
  {
    return (*it).second;
  }

  ObjectPair default_value( key, Value::null );

  it = this->value_.object_->insert( it, default_value );

  return (*it).second;
}

// Derives from JsonCpp implementation
Value& Value::operator[]( int index )
{
  NOM_ASSERT( index >= 0 );

  return (*this)[ ArrayIndex( index ) ];
}

// Derives from JsonCpp implementation
const Value& Value::operator[]( ArrayIndex index ) const
{
  NOM_ASSERT( this->null_type() || this->array_type() );

  if( this->null_type() )
  {
    return null;
  }

  VString key( index );

  ObjectConstIterator it = this->value_.object_->find( key );

  if( it == this->value_.object_->end() )
  {
    return null;
  }

  return (*it).second;
}

// Derives from JsonCpp implementation
const Value& Value::operator[]( int index ) const
{
  NOM_ASSERT( index >= 0 );

  return (*this)[ ArrayIndex( index ) ];
}

/*const*/ Value& Value::operator[]( const char* key ) /*const*/
{
  // An object node container is required for this method call.
  if( ! this->object_valid() )
  {
    this->type_ = ValueType::ObjectValues;
    this->value_.object_ = new Object();
  }

// TODO: TRY ME OUT
/*
  NOM_ASSERT( this->null_type() || this->object_type() );

  if( this->null_type() )
  {
    return Value::null;
  }
*/
  auto res = this->value_.object_->find( key );

  if( res == this->value_.object_->end() ) // No match found!
  {
    this->value_.object_->insert( std::pair<VString, Value>( key, Value() ) );

    auto res2 = this->value_.object_->find( key );

    if( res2 != this->value_.object_->end() ) // Match found!
    {
      return res2->second;
    }
    else // No match found
    {
      return *this;
    }
  }
  else // Match found!
  {
    this->value_.object_->insert( std::pair<VString, Value>( key, res->second ) );

    return res->second;
  }

  return res->second;
}

Value& Value::operator[]( const std::string& key )
{
  return (*this)[ key.c_str() ];
}

// const Value& Value::operator[]( const std::string& key ) const
// {
//   return (*this)[ key ];
// }

// Implementation derives from JsonCpp
Value& Value::push_back( const Value& val )
{
  return (*this)[ ArrayIndex( this->size() )] = val;
}

// Value& Value::at( int index )
// {
//   NOM_ASSERT( index >= 0 && index > this->size() );
//   if( index < 0 && index < this->size() )
//   {
//     // Valid position
//     return( *this )[ (index) ];
//   }

//   // An object node container is required for this method call.
//   if( ! this->object_valid() )
//   {
//     this->type_ = ValueType::ObjectValues;
//     this->value_.object_ = new Object();
//   }

//   // Err; invalid position
//   // return Value::null;
//   return *this;
// }

Value Value::find( const std::string& key ) const
{
  // Sanity check
  NOM_ASSERT( this->null_type() || this->object_type() );

  auto res = this->value_.object_->find( key );

  if( res == this->value_.object_->end() ) // No match found
  {
    return Value::null;
  }
  else // Success -- match found!
  {
    return res->second;
  }

  return Value::null;
}

Value Value::erase( const std::string& key )
{
  // Sanity check
  NOM_ASSERT( this->null_type() || this->object_type() );

  VString k( key );
  auto res = this->value_.object_->find( k );

  if( res == this->value_.object_->end() ) // No match found
  {
    return Value::null;
  }
  else // Success -- match found; erasing found key pair!
  {
    this->value_.object_->erase( res );

    return res->second;
  }

  return Value::null;
}

// Value Value::erase( int index )
// {
//   // Sanity check
//   NOM_ASSERT( this->null_type() || this->array_type() );

//   VString k( index );
//   auto res = this->value_.object_->find( k );

//   if( res == this->value_.object_->end() )
//   {
//     // No match found
//     return Value::null;
//   }
//   else
//   {
//     // Success -- match found; erasing found key pair!
//     this->value_.object_->erase( res );

//     return res->second;
//   }

//   return Value::null;
// }

Value::Members Value::member_names( void ) const
{
  Members keys;

  // Sanity check
  NOM_ASSERT( this->null_type() || this->object_type() );

  if( ! this->object_valid() ) return keys;

  for( auto itr = this->value_.object_->begin(); itr != this->value_.object_->end(); ++itr )
  {
    // VString object stores the member key

    keys.push_back( itr->first.c_str() );
  }

  return keys;
}

Value::ConstIterator Value::begin( void ) const
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::ConstIterator itr = this->value_.object_->begin();
    return Value::ConstIterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::ConstIterator itr = this->value_.object_->begin();
    return Value::ConstIterator( itr );
  }

  // Error; returning NullValue object
  return ValueConstIterator(); // Not an array or object type
}

Value::ConstIterator Value::end( void ) const
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::ConstIterator itr = this->value_.object_->end();
    return Value::ConstIterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::ConstIterator itr = this->value_.object_->end();
    return Value::ConstIterator( itr );
  }

  // Error; returning NullValue object
  return ValueConstIterator(); // Not an array or object type
}

Value::Iterator Value::begin( void )
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::Iterator itr = this->value_.object_->begin();
    return Value::Iterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::Iterator itr = this->value_.object_->begin();
    return Value::Iterator( itr );
  }

  return ValueIterator(); // Error: not an array or object type
}

Value::Iterator Value::end( void )
{
  if( this->array_valid() ) // ArrayIterator
  {
    Value::Iterator itr = this->value_.object_->end();
    return Value::Iterator( itr );
  }
  else if ( this->object_valid() ) // ObjectIterator
  {
    Value::Iterator itr = this->value_.object_->end();
    return Value::Iterator( itr );
  }

  return ValueIterator(); // Error: not an array or object type
}

const std::string Value::dump( const Value& object, int depth ) const
{
  std::string key;
  std::stringstream os; // Output buffer

  depth += 1; // Recursion

  os << this->dump_key( object );

  // Show the numeric depth level we are at
  os << " (";
  os << depth;
  os << ")";

  // nom::Value object is a tree of key & value pairs.
  if( object.size() > 0 )
  {
    os << std::endl;

    for( Value::ConstIterator itr = object.begin(); itr != object.end(); ++itr )
    {
      Value::ConstIterator member( itr );

      key = member.key();

      // Show the current depth.
      for ( uint tab = 0 ; tab < depth; ++tab )
      {
        os << "-";
      }

      os << " subvalue (";

      // Dump the member name portion of the pair.
      if( key != "" )
      {
        os << key;

        if( ! member->array_type() && ! member->object_type() )
        {
          os << ": ";
        }

        os << this->dump_value( member->ref() );
        os << ")";
      }
      else
      {
        os << this->dump_value( member->ref() );
        os << ")";
      }

      // Iterate onwards to the next level of the tree via recursion.
      os << this->dump( *itr, depth );
    }

    return os.str();
  }
  else
  {
    // Move onto the next element of the tree!
    os << std::endl;
  }

  return os.str();
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
  else // Unknown type???
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
    // Special case -- must be handled inside nom::Value::dump.
  }
  else if( val.object_type() ) // Type 7
  {
    // Special case -- must be handled inside nom::Value::dump.
  }
  else // Unknown type
  {
    os << "Unknown (" << val.type() << ")";
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

} // namespace nom
