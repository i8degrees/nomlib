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
#include "nomlib/system/helpers.hpp"
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

    typedef std::vector<std::string> Members;

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

    /// \brief Exchange the contents of the container.
    ///
    /// \remarks In particular, one must be careful to keep track of copying our
    /// char* strings as necessary.
    ///
    /// \note This method is used in the implementation of the copy assignment
    /// operator.
    void swap( Value& other );

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator <( const Value& other ) const;

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator <=( const Value& other ) const;

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator >=( const Value& other ) const;

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator >( const Value& other ) const;

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator ==( const Value& other ) const;

    /// \brief ...
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator !=( const Value& other ) const;

    /// \brief Returns ::null_type.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator!( void ) const;

    /// \brief Construct a Value container node of a specified type.
    Value( enum ValueType type );

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

    /// \brief Internal helper method for comparing array & object node
    /// containers.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    int compare( const Value& other ) const;

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

    /// \brief Obtain boolean response in regards to container's empty status.
    ///
    /// \remarks The object's type is not modified.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either: null, array or object node type.
    bool empty( void );

    /// \brief Remove all array and object members.
    ///
    /// \remarks The object's value type is unchanged.
    ///
    /// \note This method has no effect unless the object's container is one of
    /// two node types: array or object.
    void clear( void );

    /// \brief Obtain a stored element by index number.
    ///
    /// \param  val Unsigned integer of the element's index to return.
    ///
    /// \remarks It may be necessary to append the 'u' symbol after the number
    /// in order to have the compiler recognize your request properly. (Signed
    /// integers are the "default" literal integer type, at least on my
    /// development system).
    ///
    /// \TODO Fix index methods -- VString, ValueBaseIterator & co; this method
    /// call relies on our half-finished implementation stemming from the
    /// VString class.
    Value& operator[]( ArrayIndex index );

    Value& operator[]( int index );

    /// \brief Obtain a stored element by index number.
    ///
    /// \param  val Unsigned integer of the element's index to return.
    ///
    /// \remarks It may be necessary to append the 'u' symbol after the number
    /// in order to have the compiler recognize your request properly. (Signed
    /// integers are the "default" literal integer type, at least on my
    /// development system).
    ///
    /// \TODO Fix index methods -- VString, ValueBaseIterator & co; this method
    /// call relies on our half-finished implementation stemming from the
    /// VString class.
    const Value& operator[]( ArrayIndex index ) const;

    const Value& operator[]( int index ) const;

    /// \brief Access an object node's container.
    ///
    /// \remarks This method call requires an object node container type, and
    /// will immediately initialize one, if one is not found to be valid at the
    /// time of the call -- existing value(s) in the object will be lost.
    Value& operator[]( const char* key );

    Value& operator[]( const std::string& key );

    /// \brief Insert array elements.
    void push_back( const Value& val );

    /// \brief Remove the named member.
    ///
    /// \returns Removed member upon success, or Value::null upon failure.
    ///
    /// \remarks The object is unchanged if the referenced key does not exist.
    ///
    /// \note The object's type is not modified.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either a null or object node type.
    Value erase( const std::string& key );

    /// \brief Obtain the member names (keys) of each pair in this container.
    ///
    /// \returns A vector of strings upon success, or an empty vector upon
    /// failure.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either a null or object node type.
    Members member_names( void ) const;

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
