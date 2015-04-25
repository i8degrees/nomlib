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

// Private headers
#include "nomlib/core/helpers.hpp"
#include <cassert>

// Forward declarations
#include "nomlib/ptree/ValueIterator.hpp"
#include "nomlib/ptree/ValueConstIterator.hpp"

// Use standard assert facilities, so we do not need to link to SDL2
#if defined(NOM_DEBUG_ASSERT) && defined(NOM_USE_SDL2_ASSERT)
  #undef NOM_ASSERT
  #define NOM_ASSERT(expression) ( assert (expression) )
#endif

namespace nom {

// Static initializations
const Value& Value::null = Value();

Value::Value() :
  type_(ValueType::Null)
{
  // NOM_LOG_TRACE(NOM);
}

Value::~Value()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  switch(this->type_) {

    default: /* Nothing to free for most primitive types */ break;

    case ValueType::String:
    {
      if( this->value_.string_ != nullptr ) {
        nom::free_string(this->value_.string_);
        this->value_.string_ = nullptr;
      }
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      NOM_DELETE_PTR(this->value_.object_);
    } break;
  }
}

Value::Value(int val) :
  type_(ValueType::SignedInteger)
{
  //NOM_LOG_TRACE(NOM);

  this->value_.int_ = val;
}

Value::Value(uint val) :
  type_(ValueType::UnsignedInteger)
{
  //NOM_LOG_TRACE(NOM);

  this->value_.uint_ = val;
}

Value::Value(real64 val) :
  type_(ValueType::RealNumber)
{
  //NOM_LOG_TRACE(NOM);

  this->value_.real_ = val;
}

Value::Value(const char* str) :
  type_(ValueType::String)
{
  //NOM_LOG_TRACE(NOM);

  nom::size_type str_len = nom::string_length(str);
  this->value_.string_ = nom::duplicate_string(str, str_len);
}

Value::Value(const std::string& str) :
  type_(ValueType::String)
{
  //NOM_LOG_TRACE(NOM);

  nom::size_type str_len = nom::string_length(str);
  this->value_.string_ = nom::duplicate_string(str, str_len);
}

Value::Value(bool val) :
  type_(ValueType::Boolean)
{
  //NOM_LOG_TRACE(NOM);

  this->value_.bool_ = val;
}

Value::Value(const Object& obj) :
  type_(ValueType::ObjectValues)
{
  // NOM_LOG_TRACE(NOM);

  this->value_.object_ = new Object(obj);
}

Value::Value(ValueType type) :
  type_(type)
{
  // NOM_LOG_TRACE( NOM );

  switch(this->type_)
  {
    default:
    case ValueType::Null:
    break;

    case ValueType::SignedInteger:
    {
      this->value_.int_ = 0;
    } break;

    case ValueType::UnsignedInteger:
    {
      this->value_.uint_ = 0;
    } break;

    case ValueType::RealNumber:
    {
      this->value_.real_ = 0.0f;
    } break;

    case ValueType::Boolean:
    {
      this->value_.bool_ = false;
    } break;

    case ValueType::String:
    {
      this->value_.string_ = nullptr;
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      this->value_.object_ = new Object();
      NOM_ASSERT(this->value_.object_ != nullptr);
    } break;
  }
}

Value::Value(const Value& rhs)
{
  // NOM_LOG_TRACE(NOM);

  this->type_ = rhs.type_;

  switch(this->type_)
  {
    default:
    case ValueType::Null:
    {
      // Do nothing
      break;
    }
    case ValueType::SignedInteger:
    {
      this->value_.int_ = rhs.value_.int_;
      break;
    }
    case ValueType::UnsignedInteger:
    {
      this->value_.uint_ = rhs.value_.uint_;
      break;
    }
    case ValueType::RealNumber:
    {
      this->value_.real_ = rhs.value_.real_;
      break;
    }

    case ValueType::Boolean:
    {
      this->value_.bool_ = rhs.value_.bool_;
      break;
    }

    case ValueType::String:
    {
      const char* rhs_string_value = rhs.value_.string_;
      if( rhs_string_value != nullptr ) {
        nom::size_type str_len = nom::string_length(rhs_string_value);

        this->value_.string_ =
          nom::duplicate_string(rhs_string_value, str_len);
      }
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      this->value_.object_ = new Object(*rhs.value_.object_);
    } break;
  } // end switch type
}

Value::SelfType& Value::operator =(const SelfType& rhs)
{
  if( this != &rhs ) {
    Value copy(rhs);
    this->swap(copy);
  }

  return *this;
}

void Value::swap(Value& rhs)
{
  Value::ValueType temp = this->type_;

  this->type_ = rhs.type_;
  rhs.type_ = temp;
  std::swap(this->value_, rhs.value_);
}

bool Value::operator <(const Value& rhs) const
{
  bool comp_result = false;

  int type_delta = this->type_ - rhs.type_;
  if( type_delta > 0 ) {
    comp_result = (type_delta < 0) ? true : false;
  }

  switch(this->type_)
  {
    default:
    case ValueType::Null:
    {
      comp_result = false;
    } break;

    case ValueType::SignedInteger:
    {
      comp_result = this->value_.int_ < rhs.value_.int_;
    } break;

    case ValueType::UnsignedInteger:
    {
      comp_result = this->value_.uint_ < rhs.value_.uint_;
    } break;

    case ValueType::RealNumber:
    {
      comp_result = this->value_.real_ < rhs.value_.real_;
    } break;

    case ValueType::Boolean:
    {
      comp_result = this->value_.bool_ < rhs.value_.bool_;
    } break;

    case ValueType::String:
    {
      if( this->value_.string_ == nullptr || rhs.value_.string_ == nullptr ) {

        if( rhs.value_.string_ != nullptr ) {
          comp_result = true;
        } else {
          comp_result = false;
        }
      } else {
        comp_result =
          nom::compare_cstr_sensitive(this->value_.string_, rhs.value_.string_) < 0;
      }
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      int delta =
        this->value_.object_->size() - rhs.value_.object_->size();
      if( delta > 0 ) {
        comp_result = delta < 0;
      } else {
        comp_result = *this->value_.object_ < *rhs.value_.object_;
      }
    } break;
  } // end switch type

  return comp_result;
}

bool Value::operator <=(const Value& rhs) const
{
  return !( rhs < *this );
}

bool Value::operator >=(const Value& rhs) const
{
  return !( *this < rhs );
}

bool Value::operator >(const Value& rhs) const
{
  return( rhs < *this );
}

bool Value::operator ==(const Value& rhs) const
{
  bool comp_result = false;

  if( this->type_ != rhs.type_ ) {
    comp_result = false;
    return comp_result;
  }

  switch(this->type_)
  {
    default:
    case ValueType::Null:
    {
      comp_result = true;
    } break;

    case ValueType::SignedInteger:
    {
      comp_result = this->value_.int_ == rhs.value_.int_;
    } break;

    case ValueType::UnsignedInteger:
    {
      comp_result = this->value_.uint_ == rhs.value_.uint_;
    } break;

    case ValueType::RealNumber:
    {
      comp_result = this->value_.real_ == rhs.value_.real_;
    } break;

    case ValueType::Boolean:
    {
      comp_result = this->value_.bool_ == rhs.value_.bool_;
    } break;

    case ValueType::String:
    {
      if( this->value_.string_ == nullptr || rhs.value_.string_ == nullptr ) {
        comp_result = this->value_.string_ == rhs.value_.string_;
      } else {
        comp_result =
          nom::compare_cstr_sensitive(this->value_.string_, rhs.value_.string_) == 0;
      }
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      comp_result =
        ( this->value_.object_->size() == rhs.value_.object_->size() ) &&
        ( *this->value_.object_ ) == ( *rhs.value_.object_ );
    } break;
  }

  return comp_result;
}

bool Value::operator != (const Value& rhs) const
{
  return !(*this == rhs);
}

bool Value::operator !() const
{
  return this->null_type();
}

// Derives from the JsonCpp library
int Value::compare(const Value& rhs) const
{
  int comp_result = 0;
  if( *this < rhs ) {
    comp_result = -1;
  } else if( *this > rhs ) {
    comp_result = 1;
  } else {
    comp_result = 0;
  }

  return comp_result;
}

Value::RawPtr Value::get()
{
  return this;
}

const Value::Reference Value::ref()
{
  return *this;
}

enum Value::ValueType Value::type() const
{
  return this->type_;
}

const std::string Value::type_name() const
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

bool Value::null_type() const
{
  return ( this->type() == ValueType::Null );
}

bool Value::int_type() const
{
  return ( this->type() == ValueType::SignedInteger );
}

bool Value::uint_type() const
{
  return ( this->type() == ValueType::UnsignedInteger );
}

bool Value::double_type() const
{
  return ( this->type() == ValueType::RealNumber );
}

bool Value::string_type() const
{
  return ( this->type() == ValueType::String );
}

bool Value::bool_type() const
{
  return ( this->type() == ValueType::Boolean );
}

bool Value::array_type() const
{
  return ( this->type() == ValueType::ArrayValues );
}

bool Value::object_type() const
{
  return ( this->type() == ValueType::ObjectValues );
}

const std::string Value::stringify() const
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

int Value::get_int() const
{
  if( this->int_type() ) return this->value_.int_;

  return 0; // Not found
}

uint Value::get_uint() const
{
  if( this->uint_type() ) return this->value_.uint_;

  return 0; // Not found
}

real64 Value::get_double() const
{
  if( this->double_type() ) return this->value_.real_;

  return 0; // Not found
}

real32 Value::get_float() const
{
  if( this->double_type() )
  {
    return static_cast<float> ( this->get_double() );
  }

  return 0; // Not found
}

const char* Value::get_cstring() const
{
  if( this->string_type() ) {
    return this->value_.string_;
  }

  return nullptr;
}

std::string Value::get_string() const
{
  if( this->string_type() ) {
    return std::string(this->value_.string_);
  }

  return "\0";
}

bool Value::get_bool() const
{
  if( this->bool_type() ) {
    return this->value_.bool_;
  }

  return false;
}

bool Value::array_valid() const
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

bool Value::object_valid() const
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

const Object Value::array() const
{
  if( this->array_valid() )
  {
    return Object( *this->value_.object_ );
  }

  // Err; not initialized..!
  return Object();
}

const Object Value::object() const
{
  if( this->object_valid() )
  {
    return Object( *this->value_.object_ );
  }

  // Err; not initialized..!
  return Object();
}

nom::size_type Value::size() const
{
  nom::size_type result = 0;

  switch(this->type_)
  {
    default:
    {
      result = 1;
    } break;

    case ValueType::Null:
    {
      result = 0;
    } break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      if( this->array_valid() == true || this->object_valid() == true ) {
        result = this->value_.object_->size();
      }
    } break;
  }

  return result;
}

bool Value::empty() const
{
  // NOM_ASSERT( this->null_type() || this->array_type() || this->object_type() );

  bool result = true;

  switch(this->type_)
  {
    default: break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      if( this->array_valid() == true || this->object_valid() == true ) {
        result = this->value_.object_->empty();
      }
    } break;
  }

  return result;
}

void Value::clear()
{
  // NOM_ASSERT( this->null_type() || this->array_type() || this->object_type() );

  switch(this->type_)
  {
    default: break;

    case ValueType::ArrayValues:
    case ValueType::ObjectValues:
    {
      if( this->array_valid() == true || this->object_valid() == true ) {
        this->value_.object_->clear();
      }
    } break;
  }
}

// Derives from JsonCpp implementation
Value& Value::operator[](ArrayIndex index)
{
  NOM_ASSERT( this->null_type() || this->array_type() );

  if( this->null_type() ) {
    *this = Value(ValueType::ArrayValues);
  }

  VString key(index);

  // Returns an iterator pointing to the first element in the container whose
  // key is not considered to go before k -- key_type -- (i.e., either it is
  // equivalent or goes after).
  ObjectIterator it = this->value_.object_->lower_bound(key);

  if( it != this->value_.object_->end() && (*it).first == key ) {
    return (*it).second;
  }

  ObjectPair default_value(key, Value::null);

  it = this->value_.object_->insert(it, default_value);

  return (*it).second;
}

Value& Value::operator[](int index)
{
  NOM_ASSERT(index >= 0);

  return (*this)[ ArrayIndex(index) ];
}

const Value& Value::operator[](ArrayIndex index) const
{
  NOM_ASSERT( this->null_type() || this->array_type() );

  if( this->null_type() ) {
    return Value::null;
  }

  VString key(index);

  ObjectConstIterator it = this->value_.object_->find(key);

  if( it == this->value_.object_->end() ) {
    return Value::null;
  }

  return (*it).second;
}

const Value& Value::operator[](int index) const
{
  NOM_ASSERT(index >= 0);

  return (*this)[ ArrayIndex(index) ];
}

// Derives from JsonCpp implementation
Value& Value::operator[](const char* key)
{
  NOM_ASSERT( this->null_type() || this->object_type() );
  if( this->null_type() ) {
    *this = Value(ValueType::ObjectValues);
  }

  auto it = this->value_.object_->lower_bound(key);

  if( it != this->value_.object_->end() && (*it).first == key ) {
    return (*it).second;
  }

  auto default_value =
    std::pair<VString, Value>(key, Value::null);
  auto res = this->value_.object_->insert(it, default_value);

  Value& value = (*res).second;
  return value;
}

Value& Value::operator[](const std::string& key)
{
  return (*this)[ key.c_str() ];
}

const Value& Value::operator[](const char* key) const
{
  const Value& found = this->find(key);

  if( found == Value::null ) {
    return Value::null;
  }

  return found;
}

const Value& Value::operator[](const std::string& key) const
{
  const Value& found = this->find( key.c_str() );

  if( found == Value::null ) {
    return Value::null;
  }

  return found;
}

// Implementation derives from JsonCpp
Value& Value::push_back(const Value& val)
{
  return (*this)[ ArrayIndex( this->size() )] = val;
}

const Value& Value::find(const std::string& key) const
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

Value Value::erase(const std::string& key)
{
  VString k(key);
  Value ret = Value::null;

  // Invalid object state!
  NOM_ASSERT( this->null_type() || this->object_type() );

  auto res = this->value_.object_->find(k);

  if( res == this->value_.object_->end() ) {
    // Failure; no match found
    return ret;
  } else {
    // Success; matching key value found
    ret = res->second;

    this->value_.object_->erase(res);
  }

  return ret;
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

Value::Members Value::member_names() const
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

Value::ConstIterator Value::begin() const
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

Value::ConstIterator Value::end() const
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

Value::Iterator Value::begin()
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

Value::Iterator Value::end()
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

const std::string Value::dump(const Value& object, int depth) const
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

const std::string Value::dump_key(const Value& key) const
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

const std::string Value::dump_value(const Value& val) const
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

const std::string Value::print_key(const std::string& type, uint size) const
{
  std::stringstream os;

  os << " [type=" << type << ", " << "size=" << size << "]";

  return os.str();
}

const std::string Value::print_value(const std::string& val) const
{
  std::stringstream os;

  os << val;

  return os.str();
}

std::ostream& operator <<(std::ostream& os, const Value& val)
{
  os << val.dump( val );

  return os;
}

} // namespace nom
