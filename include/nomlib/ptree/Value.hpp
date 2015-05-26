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

#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/ptree/ptree_config.hpp"
#include "nomlib/ptree/ptree_types.hpp"

namespace nom {

/// \brief Generic interface for opaque data containers
class Value
{
  public:
    typedef Value SelfType;
    typedef Value* RawPtr;
    typedef Value& Reference;

    // TODO: Rename to const_iterator?
    typedef ValueConstIterator ConstIterator;

    // TODO: Rename to iterator?
    typedef ValueIterator Iterator;

    typedef std::vector<std::string> Members;

    /// \brief Supported data types able to be held inside the object.
    ///
    /// \remarks ValueType::ObjectValue type is a singly-list of key / value
    /// pairs; this allows one to use this class to serialize and unserialize
    /// many common file formats, such as: JSON, XML, INI & plist.
    ///
    /// \internal
    /// \note Borrowed over from the wonderful JsonCpp API.
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
    static const Value& null;

    /// \brief Default constructor; constructs an object with NullValue data
    /// type.
    Value(); // type 0

    ~Value();

    /// \brief Construct an object using a signed integer value.
    ///
    /// \note Type 1
    Value(int val);

    /// \brief Construct an object using an unsigned (non-negative) integer
    /// value.
    ///
    /// \remarks It may be necessary to append the 'u' symbol after the number
    /// in order to have the compiler recognize the request and route to the
    /// correct constructor.
    ///
    /// \note Type 2
    Value(uint val);

    /// \brief Construct an object using a double-precision floating point
    /// "real" number value.
    ///
    /// \note Type 3
    Value(real64 val);

    /// \brief Construct an object from a C-style string value.
    ///
    /// \remarks A duplicate copy of the passed C string value is made upon
    /// construction.
    ///
    /// \internal
    /// \note This constructor makes construction using string literals
    /// possible:
    ///
    /// \code
    /// nom::Value( "string" );
    /// \endcode
    ///
    /// \note Without defining this constructor, string literals were being
    /// overloaded by the boolean constructor.
    /// \endinternal
    ///
    /// \note Type 4
    Value(const char* str);

    /// \brief Construct an object from a C++ string value (std::string).
    ///
    /// \note Type 4
    Value(const std::string& str);

    /// \brief Construct an object from a boolean value.
    ///
    /// \note Type 5
    Value(bool val);

    /// \brief Construct an object with either array or object node values.
    ///
    /// \note Type 6 or 7
    Value(const Object& obj);

    /// \brief Construct a Value container node of a specified type.
    Value(ValueType type);

    /// \brief Copy constructor.
    Value(const Value& rhs);

    /// \brief Copy assignment operator.
    Value::SelfType& operator =(const SelfType& rhs);

    /// \brief Exchange the contents of the container; copy & swap idiom.
    ///
    /// \remarks In particular, one must be careful to keep track of copying our
    /// char* strings as necessary.
    ///
    /// \note This method is used in the implementation of the copy assignment
    /// operator.
    void swap(Value& rhs);

    /// \brief Lesser than comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    ///
    /// \todo Unit testing of the operator overload logic for lesser than
    /// equality.
    bool operator <(const Value& rhs) const;

    /// \brief Lesser than or equal to comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator <=(const Value& rhs) const;

    /// \brief Greater than or equal to comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator >=(const Value& rhs) const;

    /// \brief Greater than or equal to comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator >(const Value& rhs) const;

    /// \brief Equality comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    ///
    /// \todo Unit testing of the operator overload logic for equality.
    bool operator ==(const Value& rhs) const;

    /// \brief Not equal comparison operator.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator !=(const Value& rhs) const;

    /// \brief Returns Value::null.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    bool operator!() const;

    /// \brief Internal helper method for comparing array & object node
    /// containers.
    ///
    /// \note Borrowed from JsonCpp library -- thanks!
    int compare(const Value& rhs) const;

    /// \brief Obtain a pointer to the object.
    ///
    /// \returns A raw pointer to the object. No transfer of ownership is made.
    Value::RawPtr get();

    /// \brief Obtain a reference to the object.
    ///
    /// \returns A reference to the object.
    Value::Reference ref();

    /// \brief Obtain the enumeration type of the object.
    ///
    /// \remarks The type is defined by the value type(s) set at construction.
    /// See also: Value::ValueType enumeration.
    enum Value::ValueType type() const;

    /// \brief Obtain the enumeration type of the object as a std::string.
    ///
    /// \remarks The type is defined by the value type(s) set at construction.
    /// See also: Value::ValueType enumeration.
    ///
    /// \todo Rename to stringify_type?
    const std::string type_name() const;

    /// \brief Query if the value type type stored in the object is NULL.
    bool null_type() const;

    /// \brief Query if the value type type stored in the object is a signed
    /// integer.
    bool int_type() const;

    /// \brief Query if the value type type stored in the object is an unsigned
    /// integer (non-negative.
    bool uint_type() const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool double_type() const;

    /// \brief Query if the value type type stored in the object is a double-
    /// precision floating point real number.
    bool float_type() const;

    /// \brief Query if the value type type stored in the object is a string
    /// value.
    bool string_type() const;

    /// \brief Query if the value type type stored in the object is a boolean
    /// value.
    bool bool_type() const;

    /// \brief Query if the value type type stored in the object are array
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool array_type() const;

    /// \brief Query if the value type type stored in the object are object
    /// values.
    ///
    /// \remarks nom::Value object may be either an array OR object type at any
    /// given time.
    bool object_type() const;

    const std::string stringify() const;

    /// \brief Obtain the signed integer value stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    int get_int() const;

    /// \brief Obtain the unsigned (non-negative) integer value stored within
    /// the container.
    ///
    /// \returns On err, zero (0) is returned.
    uint get_uint() const;

    /// \brief Obtain the double-precision floating point "real" number value
    /// stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    real64 get_double() const;

    /// \brief Obtain the double-precision floating point "real" number value
    /// stored within the container.
    ///
    /// \returns On err, zero (0) is returned.
    ///
    /// \remarks Conversion from the internally stored double variable to a
    /// float type is done.
    real32 get_float() const;

    /// \brief Obtain the C style string value stored within the container.
    ///
    /// \returns On err, nullptr is returned.
    ///
    /// ~~\remarks A copy of the stored C string is made, therefore no ownership
    /// transfers occur; you are responsible for freeing the returned C string.~~
    const char* get_cstring() const;

    /// \brief Obtain the string value stored within the container.
    ///
    /// \remarks Conversion from the internally stored char* variable to a
    /// std::string is done.
    ///
    /// \returns On err, a null-terminated std::string -- "\0" is returned.
    std::string get_string() const;

    /// \brief Obtain the boolean value stored within the container.
    ///
    /// \returns On err, boolean false is returned.
    bool get_bool() const;

    bool array_valid() const;
    bool object_valid() const;

    /// \brief Obtain the array values of the object.
    ///
    /// \returns ~~Return-by-value cloned copy of the nom::Array pointer held by
    /// this object.~~
    const Object array() const;

    /// \brief Obtain the object tree of the object.
    ///
    /// \returns ~~Return-by-value cloned copy of the nom::Object pointer held by
    /// this object.~~
    const Object object() const;

    /// \brief Obtain the size of the object's contained values.
    ///
    /// \returns Size of the array or object, or one (1) when not said type.
    /// On err -- when the object is ValueType::NullValue -- zero (0).
    nom::size_type size() const;

    /// \brief Obtain boolean response in regards to container's empty status.
    ///
    /// \remarks The object's type is not modified.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either: null, array or object node type.
    bool empty() const;

    /// \brief Remove all array and object members.
    ///
    /// \remarks The object's value type is unchanged.
    ///
    /// \note This method has no effect unless the object's container is one of
    /// two node types: array or object.
    void clear();

    /// \brief Obtain a stored element by index number.
    ///
    /// \param  val Unsigned integer of the element's index to return.
    ///
    /// \remarks It may be necessary to append the 'u' symbol after the number
    /// in order to have the compiler recognize the request and route to the
    /// correct constructor.
    ///
    /// \note In contrast to the standard STL method overloads for
    /// ::operator[](int), nom::Value should always perform bounds checking.
    Value& operator[](ArrayIndex index);

    /// \note In contrast to the standard STL method overloads for
    /// ::operator[](int), nom::Value should always perform bounds checking.
    Value& operator[](int index);

    /// \brief Obtain a stored element by index number.
    ///
    /// \param  val Unsigned integer of the element's index to return.
    ///
    /// \remarks It may be necessary to append the 'u' symbol after the number
    /// in order to have the compiler recognize your request properly. (Signed
    /// integers are the "default" literal integer type, at least on my
    /// development system).
    ///
    /// \todo Fix index methods -- VString, ValueBaseIterator & co; this method
    /// call relies on our half-finished implementation stemming from the
    /// VString class.
    ///
    /// \note In contrast to the standard STL method overloads for
    /// ::operator[](int), nom::Value should always perform bounds checking.
    const Value& operator[](ArrayIndex index) const;

    /// \note In contrast to the standard STL method overloads for
    /// ::operator[](int), nom::Value should always perform bounds checking.
    const Value& operator[](int index) const;

    /// \brief Access an object node's container.
    ///
    /// \remarks This method call requires an object node container type, and
    /// will immediately initialize one, if one is not found to be valid at the
    /// time of the call -- existing value(s) in the object will be lost.
    ///
    /// \note In contrast to the standard STL method overloads for
    /// ::operator[](int), nom::Value should always perform bounds checking.
    Value& operator[](const char* key);

    Value& operator[](const std::string& key);

    const Value& operator[](const char* key) const;
    const Value& operator[](const std::string& key) const;

    /// \brief Insert array elements.
    Value& push_back(const Value& val);

    /// \brief Search the object for an existing member.
    ///
    /// \returns The found member key upon success, or Value::null upon failure.
    ///
    /// \note The object's type is not modified.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either a null or object node type.
    const Value& find(const std::string& key) const;

    /// \brief Remove the named member.
    ///
    /// \returns Removed member upon success, or Value::null upon failure.
    ///
    /// \remarks The object is unchanged if the referenced key does not exist.
    ///
    /// \note The object's type is not modified.
    Value erase(const std::string& key);

    /// \brief Remove an array object.
    ///
    /// \returns The removed array object upon success, or Value::null on
    /// failure, such as when you specify an invalid position index.
    ///
    /// \remarks The object is unchanged if the referenced key does not exist.
    ///
    /// \note The object's type is not modified.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either a null or object node type.
    ///
    /// \todo Verify working state of method.
    // Value erase( int index );

    /// \brief Obtain the member names (keys) of each pair in this container.
    ///
    /// \returns A vector of strings upon success, or an empty vector upon
    /// failure.
    ///
    /// \note This method will fail with an assert if the container type is
    /// *not* either a null or object node type.
    Members member_names() const;

    Value::ConstIterator begin() const;

    Value::ConstIterator end() const;

    /// \brief Iterator access to the beginning of the object's tree.
    ///
    /// \returns nom::ValueIterator object pointing to the beginning of the
    /// array or object. On err, an invalid, empty nom::ValueIterator is
    /// returned.
    ///
    /// \remarks The nom::Value object must be initialized as a nom::ArrayValue
    /// or nom::ObjectValue type.
    Value::Iterator begin();

    /// \brief Iterator access to the end of the object's tree.
    ///
    /// \returns nom::ValueIterator object pointing to the beginning of the
    /// array or object. On err, an invalid, empty nom::ValueIterator is
    /// returned.
    ///
    /// \remarks The nom::Value object must be initialized as a nom::ArrayValue
    /// or nom::ObjectValue type.
    Value::Iterator end();

    /// \brief Dump the object's complete value tree.
    ///
    /// \remarks This is a useful method for debugging.
    ///
    /// \note This method is used by the << overload function for nom::Value
    /// objects.
    const std::string dump(const Value& object, int depth = 0) const;

  private:
    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_key(const Value& key) const;

    /// \brief Internal helper method for nom::Value::dump.
    const std::string dump_value(const Value& val) const;

    /// \brief Internal helper method for nom::Value::dump_key.
    const std::string print_key(const std::string& type, uint size) const;

    /// \brief Internal helper method for nom::Value::dump_value.
    const std::string print_value(const std::string& val) const;

    /// \brief Container for the data types able to be held.
    ///
    /// \internal
    ///
    /// \note Unionized structure is for conserving memory usage and
    /// conveniently so happens to fit our container's enforced one value type
    /// per object pattern.
    ///
    /// \endinternal
    union ValueHolder
    {
      int int_;             // Type 1
      uint uint_;           // Type 2
      real64 real_;         // Type 3
      bool bool_;           // Type 4
      // NOTE: Instance-owned pointer.
      const char* string_;  // Type 5
      // NOTE: Instance-owned pointer.
      Object* object_;      // Type 6 and 7
    };

    /// \brief The type of stored value in this instance.
    enum ValueType type_;

    /// \brief The stored value in this instance.
    ValueHolder value_;
};

/// \brief Pretty print the object
///
/// \todo Implement upper limit to value length dump; ideally within ~80
/// characters or less?
std::ostream& operator <<(std::ostream& os, const Value& val);

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
/// \todo Implement support for (un)-signed 64-bit integers
///
/// \todo Implement support for comments (XML & JSON style)
