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
#ifndef NOMLIB_JSON_JSONCPP_VALUE_HPP
#define NOMLIB_JSON_JSONCPP_VALUE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <fstream>

#include "nomlib/json/jsoncpp/json.h" // JsonCpp library

#include "nomlib/config.hpp"
#include "nomlib/json/JsonSerializer.hpp"
#include "nomlib/system/ptree/Value.hpp"

namespace nom {

/// \brief Low-level interfacing API wrapper for JsonCpp
class JsonCppValue
{
  public:
    typedef JsonCppValue SelfType;
    typedef Json::ValueType JsonValueType;
    typedef Json::Value::Members JsonMemberType;

    /// \brief Default constructor; initialize container to NullValue
    JsonCppValue( void );

    /// \brief Destructor.
    ~JsonCppValue( void );

    /// \brief Copy constructor
    JsonCppValue( const SelfType& copy );

    /// \brief Copy constructor for Json::Value
    JsonCppValue( const Json::Value& copy );

    /// \brief Copy assignment operator.
    SelfType& operator =( const SelfType& other );

    /// \brief Copy assignment operator for Json::Value.
    SelfType& operator =( const Json::Value& other );

    /// \brief Obtain the underlying container of this object
    Json::Value& get( void ) const;

    uint32 size( void ) const;
    JsonValueType type( void ) const;
    JsonValueType type( int index ) const;
    JsonValueType type( const std::string& key ) const;

    JsonMemberType members( int index ) const;

    /// \brief Obtain the member pairs of an unmapped JSON object (array with
    /// unmapped objects).
    JsonMemberType members( const std::string& key ) const;

    /// \brief Obtain the top-level member pairs of a JSON object.
    JsonMemberType members( void ) const;

    /// \brief Query if the value type type stored in the object is NULL.
    bool null_type( const std::string& key ) const;
    bool null_type( int index ) const;

    /// \brief Query if the value type type stored in the object is a signed
    /// integer.
    bool int_type( const std::string& key ) const;
    bool int_type( int index ) const;

    /// \brief Query if the value type type stored in the object is an unsigned
    /// integer (non-negative.
    bool uint_type( const std::string& key ) const;
    bool uint_type( int index ) const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool double_type( const std::string& key ) const;
    bool double_type( int index ) const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool float_type( const std::string& key ) const;
    bool float_type( int index ) const;

    /// \brief Query if the value type type stored in the object is a string
    /// value.
    bool string_type( const std::string& key ) const;
    bool string_type( int index ) const;

    /// \brief Query if the value type type stored in the object is a boolean
    /// value.
    bool bool_type( const std::string& key ) const;
    bool bool_type( int index ) const;

    /// \brief Query if the value type type stored in the object are array
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool array_type( const std::string& key ) const;
    bool array_type( int index ) const;

    /// \brief Query if the value type type stored in the object are object
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool object_type( const std::string& key ) const;
    bool object_type( int index ) const;

    /// \brief Query if the top-level JSON container is an object type.
    bool object_type( void ) const;

    int get_int( const std::string& key ) const;

    /// Get signed integer array values by key
    const std::vector<int> get_ints( const std::string& key ) const;

    uint get_uint( const std::string& key ) const;

    /// Get unsigned integer array values by key
    const std::vector<uint> get_uints( const std::string& key ) const;

    /// \brief Obtain double-precision floating point value by key
    double get_double( const std::string& key ) const;

    /// \brief Obtain double-precision floating point array values by key.
    const std::vector<double> get_doubles( const std::string& key ) const;

    /// \brief Obtain double-precision floating point value by key
    float get_float( const std::string& key ) const;

    /// \brief Obtain double-precision floating point array values by key.
    const std::vector<float> get_floats( const std::string& key ) const;

    /// Get C++ string value by key
    const std::string get_string( const std::string& key ) const;

    /// Get C++ string array values by key
    const std::vector<std::string> get_strings( const std::string& key ) const;

    /// Get bool value by key
    bool get_bool( const std::string& key );

    /// Get bool array values by key
    const std::vector<bool> get_bools( const std::string& key ) const;

    /// \brief Method for obtaining values from an array
    ///
    /// \remarks nom::Value
    const Array get_values( const std::string& key ) const;

    /// \brief Set the current value to NULL
    ///
    /// \note Json::nullValue (type 0).
    void insert( const std::string& key );

    /// JSON signed integer
    ///
    /// \note Type 1 -- Json::intValue
    void insert( const std::string& key, int value );
    void insert( const std::string& key, const std::vector<int> values );

    /// JSON unsigned integer
    ///
    /// \note Type 2 -- Json::uintValue
    void insert( const std::string& key, uint value );
    void insert( const std::string& key, const std::vector<uint>& values );

    /// JSON double
    ///
    /// \note Type 3 -- Json::realValue
    void insert( const std::string& key, double value );

    /// JSON float
    ///
    /// \note Type 3 -- Json::realValue
    void insert( const std::string& key, float value );

    void insert( int index, const char* val );
    /// JSON string (C++ string)
    ///
    /// \note Type 4 -- Json::stringValue
    void insert( const std::string& key, const std::string& value );
    void insert( const std::string& key, const std::vector<std::string>& values );

    void insert( const std::string& key, const char* val );
    void insert( const std::string& key, const std::vector<char*>& val );

    /// JSON boolean
    ///
    /// \note Type 5 -- Json::booleanValue
    void insert( const std::string& key, bool value );

    /// JSON array or object
    ///
    /// \note Type [6..7] -- Json::arrayValue || Json::objectValue
    void insert( const SelfType& values );

    /// JSON object
    ///
    /// \NOTE Json::objectValue -- Type 7
    ///
    /// \remarks { "key": { "key": "value" } }
    void insert( const std::string& key, const SelfType& values );

    /// \remarks nom::Value
    void insert( const std::string& key, const Array values );

    void endl( void );

    /// \brief Internal helper method for nom::JsonCppValue::dump.
    void dump_key ( const JsonCppValue& object ) const;

    /// \brief Internal helper method for nom::Value::dump.
    void dump_value ( const JsonCppValue& val ) const;

    /// \brief Dump the object's complete value tree.
    ///
    /// \remarks This is a useful debugging method.
    bool dump ( const JsonCppValue& object, int depth = 0 ) const;

    /// \brief Obtain a C++ string of the JSON object(s) stored.
    ///
    /// \returns Serialized object as a std::string.
    const std::string stringify( void ) const;

    /// \brief Save nom::JsonCppValue values to JSON objects.
    ///
    /// \param source nom::JsonCppValue container to serialize.
    /// \param output Absolute file path to save resulting data to.
    ///
    /// \deprecated This method exists only for the backwards compatibility with
    /// TTcards -- we have yet to upgrade its code-base to use
    /// nom::JsonSerializer and nom::Value.
    bool serialize( const JsonCppValue& source, const std::string& output ) const;

    /// \brief Load data from a JSON (.json) file to a nom::JsonCppValue object.
    ///
    /// \param input  Absolute file path file to un-serialize.
    /// \param dest   nom::JsonCppValue container to store values in.
    ///
    /// \deprecated This method exists only for the backwards compatibility with
    /// TTcards -- we have yet to upgrade its code-base to use
    /// nom::JsonSerializer and nom::Value.
    bool unserialize( const std::string& input, JsonCppValue& dest ) const;

  private:
    /// \brief Underlying container for values
    mutable Json::Value object_;

    /// \brief Tracks the position of Json::Value
    uint32 pos_;
};

/// \brief Pretty print the JSON object
std::ostream& operator <<( std::ostream& os, const JsonCppValue& obj );

} // namespace nom

#endif // include guard defined

/// \class nom::JsonCppValue
/// \ingroup json
///
/// \todo Rename insert methods to prevent method overloading; the overloading
/// makes it a nightmare in tracking which method is being called...
///
///   [TO BE WRITTEN]
///
/// See nom::SpriteSheet source for usage examples!
///
/// \deprecated This interface is likely to fade away as nom::Value and
/// nom::JsonSerializer interfaces slowly replace this wrapper class.
///
