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
#ifndef NOMLIB_JSON_VALUE_HPP
#define NOMLIB_JSON_VALUE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/json/config_json.hpp"

namespace nom {
namespace JSON {

/// \brief Interface class for JSON data types
class Value
{
  public:
    /// Default constructor
    Value ( void );

    /// Destructor
    ~Value ( void );

    /// Copy constructor for self
    Value ( const Value& copy );

    /// Copy constructor for ValueType
    Value ( const ValueType& copy );

    /// Copy assignment operator
    Value& operator = ( const Value& other );

    /// Copy assignment operator
    Value& operator = ( const ValueType& other );

    /// Obtain the underlying container of this object
    ValueType& get ( void ) const;

    uint32 size ( void ) const;
    JSONValueType type ( void ) const;
    JSONValueType type ( int index ) const;
    JSONValueType type ( const std::string& key ) const;
    JSONMemberType members ( int index ) const;

    /// Get signed integer array values by key
    std::vector<int> get_ints ( const std::string& key );
    int get_int ( const std::string& key );

    /// Get unsigned integer array values by key
    std::vector<uint> get_uints ( const std::string& key );
    uint get_uint ( const std::string& key );

    /// Get C++ string value by key
    std::string get_string ( const std::string& key );

    /// Get C++ string array values by key
    std::vector<std::string> get_strings ( const std::string& key );

    /// Get bool array values by key
    std::vector<bool> get_bools ( const std::string& key );

    /// Get bool value by key
    bool get_bool ( const std::string& key );

    /// JSON NULL
    ///
    /// \note Type 0 -- Json::nullValue
    void insert ( const std::string& key );

    /// JSON signed integer
    ///
    /// \note Type 1 -- Json::intValue
    void insert ( const std::string& key, int value );
    void insert ( const std::string& key, std::vector<int> values );

    /// JSON unsigned integer
    ///
    /// \note Type 2 -- Json::uintValue
    void insert ( const std::string& key, uint value );
    void insert ( const std::string& key, const std::vector<uint>& values );

    /// JSON double
    ///
    /// \note Type 3 -- Json::realValue
    void insert ( const std::string& key, double value );

    /// JSON float
    ///
    /// \note Type 3 -- Json::realValue
    void insert ( const std::string& key, float value );

    /// JSON string (C++ string)
    ///
    /// \note Type 4 -- Json::stringValue
    void insert ( const std::string& key, const std::string& value );
    void insert ( const std::string& key, const std::vector<std::string>& values );

    /// JSON boolean
    ///
    /// \note Type 5 -- Json::booleanValue
    void insert ( const std::string& key, bool value );

    void endl ( void );

    /// JSON array or object
    ///
    /// \note Type [6..7] -- Json::arrayValue || Json::objectValue
    void insert ( const Value& values );

  private:
    /// Contents of a key
    ///
    /// \fixme Mutable is a temporary workaround!
    mutable ValueType object_;

    /// Tracks the position of ValueType
    uint32 pos_;
};

} // namespace JSON
} // namespace nom

#endif // include guard defined

/// \class nom::JSON::Value
/// \ingroup json
///
/// \todo Rename insert methods to prevent method overloading; the overloading
/// makes it a nightmare in tracking which method is being called...
///
///   [TO BE WRITTEN]
///
/// See nom::SpriteSheet source for usage examples!
///
