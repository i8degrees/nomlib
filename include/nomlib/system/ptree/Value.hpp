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
#ifndef NOMLIB_SYSTEM_PTREE_VALUE_HPP
#define NOMLIB_SYSTEM_PTREE_VALUE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/ptree/ptree_config.hpp"
#include "nomlib/system/ptree/Object.hpp" // nom::Object
#include "nomlib/system/ptree/Array.hpp" // nom::Array

namespace nom {

/// \brief Generic interface for opaque data containers
class Value
{
  public:
    typedef Value SelfType;
    typedef Value* RawPtr;
    typedef Value& Reference;

    typedef ValueConstIterator ConstIterator;
    typedef ValueIterator Iterator;

    /// \brief Supported data types able to be held inside the object.
    ///
    /// \remarks ValueType::ObjectValue type is a singly-list of key / value
    /// pairs; this allows one to use this class to serialize and unserialize
    /// many common file formats, such as: JSON, XML, INI & plist.
    ///
    /// \internal
    /// \NOTE Borrowed over from the wonderful JsonCpp API.
    /// \endinternal
    enum ValueType
    {
      Null = 0,         // Type 0 (default)
      SignedInteger,    // Type 1
      UnsignedInteger,  // Type 2
      RealNumber,       // Type 3
      String,           // Type 4
      Boolean,          // Type 5
      ArrayValues,      // Type 6
      ObjectValues      // Type 7
    };

    /// \brief Declared value of Null for this object.
    ///
    /// \remarks Value::null is the default constructor's type.
    static const Value null;

    /// \brief Default constructor; constructs an object with NullValue data
    /// type.
    Value( void ); // type 0

    /// \brief Destructor.
    ///
    /// \TODO Memory management of our pointers.
    ~Value( void );

    /// \brief Copy constructor.
    Value( const Value& copy );

    /// \brief Copy assignment overload.
    ///
    /// \TODO Conditional if like we do in copy constructor for value_.
    Value::SelfType& operator =( const SelfType& other );

    /// \brief Construct an object using a signed integer value.
    Value( sint val ); // type 1

    /// \brief Construct an object using an unsigned (non-negative) integer
    /// value.
    ///
    /// \FIXME The above constructor for signed integers seems to be overloading
    /// this one, therefore breaking native support for this type.
    Value( uint val ); // type 2

    /// \brief Construct an object using a double-precision floating point
    /// "real" number value.
    Value( double val ); // type 3

    /// \brief Construct an object with a C-style string value.
    ///
    /// \remarks A duplicate copy of the passed C string value is made upon
    /// construction.
    ///
    /// \internal
    /// \NOTE This constructor makes construction using string literals possible:
    ///
    /// \code
    /// nom::Value( "string" );
    /// \endcode
    ///
    /// Without this constructor, string literals were being overloaded by the
    /// boolean constructor.
    /// \endinternal
    Value( const char* val ); // type 4

    /// \brief Construct an object with a std::string value.
    Value( const std::string& val ); // type 4

    /// \brief Construct an object from a boolean value.
    Value( bool val ); // type 5

    /// \brief Construct an object with nom::Array values.
    ///
    /// \remarks An array is a simplified version of the nom::Object data
    /// container; the container for an array holds references to nom::Value
    /// objects.
    ///
    /// \note The constructed object may only contain a single nom::Array object
    /// at a time.
    Value( const std::vector<SelfType>& val ); // type 6

    /// \brief Construct an object with nom::Object values.
    ///
    /// \remarks Each object holds a pointer to a nom::Value, or in other words,
    /// a singly-list, and has no restrictions on size other than the free
    /// memory required to store the values.
    ///
    /// \note The constructed object may only contain a single nom::Object
    /// object at a time.
    Value( const Object& val ); // type 7

    /// \brief Obtain a pointer to the object.
    ///
    /// \returns A raw pointer to the object. No transfer of ownership is made.
    Value::RawPtr get( void );

    /// \brief Obtain a reference to the object.
    ///
    /// \returns A reference to the object.
    const Value::Reference ref( void );

    /// \brief Obtain the enumeration type of the object.
    ///
    /// \remarks The type is defined by the value type(s) set at construction.
    /// See also: Value::ValueType enumeration.
    enum Value::ValueType type( void ) const;

    /// \brief Obtain the enumeration type of the object as a std::string.
    ///
    /// \remarks The type is defined by the value type(s) set at construction.
    /// See also: Value::ValueType enumeration.
    ///
    /// \TODO Rename to stringify_type?
    const std::string type_name( void ) const;

    /// \brief Query if the value type type stored in the object is NULL.
    bool null_type( void ) const;

    /// \brief Query if the value type type stored in the object is a signed
    /// integer.
    bool int_type( void ) const;

    /// \brief Query if the value type type stored in the object is an unsigned
    /// integer (non-negative.
    bool uint_type( void ) const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool double_type( void ) const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool float_type( void ) const;

    /// \brief Query if the value type type stored in the object is a string
    /// value.
    bool string_type( void ) const;

    /// \brief Query if the value type type stored in the object is a boolean
    /// value.
    bool bool_type( void ) const;

    /// \brief Query if the value type type stored in the object are array
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool array_type( void ) const;

    /// \brief Query if the value type type stored in the object are object
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool object_type( void ) const;

    const std::string stringify( void ) const;

    // TODO: Implement
    // const std::string key( void );

    /// \brief Obtain the signed integer value stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    sint get_int( void ) const;

    /// \brief Obtain the unsigned (non-negative) integer value stored within
    /// the container.
    ///
    /// \returns On err, zero (0) is returned.
    uint get_uint( void ) const;

    /// \brief Obtain the double-precision floating point "real" number value
    /// stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    double get_double( void ) const;

    /// \brief Obtain the double-precision floating point "real" number value
    /// stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    ///
    /// \remarks Conversion from the internally stored double variable to a
    /// float type is done.
    float get_float( void ) const;

    /// \brief Obtain the C style string value stored within the container.
    ///
    /// \returns On err, nullptr is returned.
    ///
    /// ~~\remarks A copy of the stored C string is made, therefore no ownership
    /// transfers occur; you are responsible for freeing the returned C string.~~
    const char* get_cstring( void ) const;

    /// \brief Obtain the string value stored within the container.
    ///
    /// \remarks Conversion from the internally stored char* variable to a
    /// std::string is done.
    ///
    /// \returns On err, a null-terminated std::string -- "\0" is returned.
    const std::string get_string( void ) const;

    /// \brief Obtain the boolean value stored within the container.
    ///
    /// \returns On err, boolean false is returned.
    bool get_bool( void ) const;

    /// \brief Obtain the value stored within the container.
    ///
    /// \returns If NULL, zero (0) is returned. On error (an unknown value),
    /// negative one (-1) is returned.
    template <typename T>
    inline T get_value( void ) const
    {
      if( this->null_type() )
      {
        return 0;
      }
      else if( this->int_type() )
      {
        return this->get_int();
      }
      else if( this->uint_type() )
      {
        return this->get_uint();
      }
      else if( this->double_type() )
      {
        return this->get_double();
      }
      else if( this->string_type() )
      {
        return this->get_string();
      }
      else if( this->bool_type() )
      {
        return this->get_bool();
      }
      else if( this->array_type() )
      {
        return this->array();
      }
      else if( this->object_type() )
      {
        return this->object();
      }
      else // Handle unknown cases
      {
        return -1;
      }
    }

    bool array_valid( void ) const;
    bool object_valid( void ) const;

    /// \brief Obtain the array values of the object.
    ///
    /// \returns ~~Return-by-value cloned copy of the nom::Array pointer held by
    /// this object.~~
    const Array array( void ) const;

    /// \brief Obtain the object tree of the object.
    ///
    /// \returns ~~Return-by-value cloned copy of the nom::Object pointer held by
    /// this object.~~
    const Object object( void ) const;

    /// \brief Obtain the size of the object's contained values.
    ///
    /// \returns Size of the array or object, or one (1) when not said type.
    /// On err -- when the object is ValueType::NullValue -- zero (0).
    uint size( void ) const;

    /// \brief Remove all array and object members
    ///
    /// \remarks Value type is not modified.
    void clear( void );

    Value& append( const Value& val );

    /// \FIXME
    Value::ConstIterator begin( void ) const;

    /// \FIXME
    Value::ConstIterator end( void ) const;

    /// \brief Iterator access to the beginning of the object's tree.
    ///
    /// \returns nom::ValueIterator object pointing to the beginning of the
    /// array or object. On err, an invalid, empty nom::ValueIterator is
    /// returned.
    ///
    /// \remarks The nom::Value object must be initialized as a nom::ArrayValue
    /// or nom::ObjectValue type.
    Value::Iterator begin( void );

    /// \brief Iterator access to the end of the object's tree.
    ///
    /// \returns nom::ValueIterator object pointing to the beginning of the
    /// array or object. On err, an invalid, empty nom::ValueIterator is
    /// returned.
    ///
    /// \remarks The nom::Value object must be initialized as a nom::ArrayValue
    /// or nom::ObjectValue type.
    Value::Iterator end( void );

    /// \brief Dump the object's complete value tree.
    ///
    /// \remarks This is a useful method for debugging.
    ///
    /// \note This method is used by the << overload function for nom::Value
    /// objects.
    const std::string dump( const Value& object, int depth = 0 ) const;

  private:
    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_key( const Value& key ) const;

    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_value( const Value& val ) const;

    /// \brief Internal helper method for nom::Value::dump_key.
    const std::string print_key( const std::string& type, uint size ) const;

    /// \brief Internal helper method for nom::Value::dump_value.
    const std::string print_value( const std::string& val ) const;

    /// \brief Container for the data types able to be held.
    ///
    /// \internal
    ///
    /// \NOTE Unionized structure is for conserving memory usage and
    /// conveniently so happens to fit our container's enforced one value type
    /// per object pattern.
    ///
    /// \endinternal
    union ValueHolder
    {
      sint int_;            // Type 1
      uint uint_;           // Type 2
      double real_;         // Type 3
      bool bool_;           // Type 4

      // FIXME: Memory leak is happening here.
      const char* string_;  // Type 5

      // FIXME: I haven't figured out how to resolve the memory leaks that we
      // get when these two variables below are pointers.
      Array* array_;        // Type 6
      Object* object_;      // Type 7
    } value_;

    // ValueConstIterator breaks if we try using these
    // Array array_;           // Type 6
    // Object object_;         // Type 7

    /// The type of value held in object container.
    enum ValueType type_;

    bool string_allocated_ = false;
};

/// \brief Pretty print the object
///
/// \TODO Implement upper limit to value length dump; ideally within ~80
/// characters or less?
std::ostream& operator <<( std::ostream& os, const Value& val );

namespace priv {

/// \brief Clone a C style string value.
///
/// \param length Size of the string to copy.
///
/// \returns Null-terminated string up to MAX_STRING_LENGTH.
///
/// \TODO Find a better home for this function?
char* duplicate_string( const char* val, uint length );

} // namespace priv

} // namespace nom

#endif // include guard defined

/// \class nom::Value
/// \ingroup ptree
///
/// Inspired by Apple's PropertyList API & made possible by JSONCPP, ...
///
///   [STUB]
///
/// References (class design & inspiration):
///
///   JSON RFC
///   json_spirit
///   jsoncpp
///   cijson (of the Cinder engine)
///   Boost::PropertyTree
///   Apple's PropertyList (.plist)
///
/// \TODO Implement support for (un)-signed 64-bit integers
///
/// \TODO Implement support for comments (XML & JSON style)
