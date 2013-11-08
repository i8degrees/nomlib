/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_JSON_VALUE_HPP
#define NOMLIB_JSON_VALUE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "json.h" // JSONCPP

#include "nomlib/config.hpp"
#include "nomlib/json/config_json.hpp"

namespace nom {
namespace JSON {

/// \brief Interface class for JSON data types
class Value
{
  public:
    Value ( void );
    ~Value ( void );

    /// Copy constructor
    Value ( const Value& copy );

    /// Copy assignment operator
    Value& operator = ( const ValueType& other );

    /// Obtain the underlying container of this object
    ValueType& get ( void ) const;

    /// JSON NULL
    ///
    /// \note Type 0 -- Json::Value::nullValue
    void insert ( const std::string& key, int index );

    /// JSON signed integer
    ///
    /// \note Type 1 -- Json::Value::intValue
    void insert ( const std::string& key, int value, int index );
    void insert ( const std::string& key, std::vector<int> values );

    /// JSON unsigned integer
    ///
    /// \note Type 2 -- Json::Value::intValue
    void insert ( const std::string& key, uint value, int index );
    void insert ( const std::string& key, const std::vector<uint>& values );

    /// JSON double
    ///
    /// \note Type 3 -- Json::Value::realNumber
    void insert ( const std::string& key, double value, int index );

    /// JSON float
    ///
    /// \note Type 3 -- Json::Value::realNumber
    void insert ( const std::string& key, float value, int index );

    /// JSON string (C string)
    ///
    /// \note Type 4 -- Json::Value::stringValue
    void insert ( const std::string& key, const char* value, int index );

    /// JSON string (C++ string)
    ///
    /// \note Type 4 -- Json::Value::stringValue
    void insert ( const std::string& key, const std::string& value, int index );
    void insert ( const std::string& key, const std::vector<std::string>& values );

    /// JSON boolean
    ///
    /// \note Type 5 -- Json::Value::booleanValue
    void insert ( const std::string& key, bool value, int index );

    /// JSON [...???...]
    ///
    /// \note Type [6||7] -- Json::Value::arrayValue || Json::Value::objectValue
    void insert ( const std::string& key, const std::vector<ValueType>& values );

    bool get_bool ( int index );
    /// Get bool array values by index
    std::vector<bool> get_bools ( int index );

    const char* c_str ( int index );

    std::string get_string ( int index );
    /// Get C++ string array values by index
    std::vector<std::string> get_strings ( int index );

    double get_double ( int index );

    int get_int ( int index );
    /// Get signed integer array values by index
    std::vector<int> get_ints ( int index );

    uint get_uint ( int index );
    /// Get unsigned integer array values by index
    std::vector<uint> get_uints ( int index );

    ValueType value ( int index );
    std::vector<ValueType> values ( int index );

    bool get_bool ( const std::string& key );

    int get_int ( const std::string& key, int index );
    uint get_uint ( const std::string& key );

    const char* c_str ( const std::string& key );
    std::string get_string ( const std::string& key, int index );

    ValueType value ( const std::string& key );

    uint32 size ( void ) const;

    JSONValueType type ( void ) const;
    JSONValueType type ( int index ) const;
    JSONValueType type ( const std::string& key, int index ) const;

  private:
    /// Contents of a key
    ///
    /// \fixme Mutable is a temporary workaround!
    mutable ValueType object_;

    /// Identifying access string for an object node
    std::string key_;
};

} // namespace JSON
} // namespace nom

#endif // include guard defined

/// \class nom::JSON::Value
/// \ingroup json
///
///   [TO BE WRITTEN]
///
/// See nom::SpriteSheet source for usage examples!
///
