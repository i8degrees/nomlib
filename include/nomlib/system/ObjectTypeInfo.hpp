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
#ifndef NOMLIB_SYSTEM_OBJECT_TYPE_INFO_HPP
#define NOMLIB_SYSTEM_OBJECT_TYPE_INFO_HPP

#include <typeinfo>   // std::type_info (typeid)
#include <typeindex>  // std::type_index

#include "nomlib/config.hpp"

namespace nom {

/// \brief Run-time type information (RTTI) container class.
class ObjectTypeInfo
{
  public:
    typedef ObjectTypeInfo self_type;
    typedef std::type_index value_type;

    /// \brief Default constructor; initializes the object's type info as
    /// itself (this should be regarded as an invalid object state).
    ObjectTypeInfo( void );

    /// \brief Destructor.
    ~ObjectTypeInfo( void );

    /// \brief Copy constructor.
    ObjectTypeInfo( const self_type& copy );

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& other );

    /// \brief Constructor for a complete object initialization.
    ///
    /// \param std::type_info object; pass through the typeid method.
    ObjectTypeInfo( const value_type& object );

    /// \brief Get the object type's name.
    ///
    /// \returns A null-terminated character sequence that may identify the
    /// object's type.
    ///
    /// \note The particular representation pointed by the returned value is
    /// implementation-defined, and may or may not be different for different
    /// types.
    const std::string name( void ) const;

    /// \brief Get the object's type hash code.
    ///
    /// \returns A hash code value that identifies the type.
    ///
    /// \remarks This function returns the same value for any two type_info
    /// objects that compare equal.
    ///
    /// \note The particular values returned are implementation-defined and may
    /// vary between executions of the same program.
    size_t hash_code( void ) const;

    /// \brief Setter for the object's type.
    void set_type( const ObjectTypeInfo::value_type& object );

    /// \brief Equality comparison operator.
    ///
    /// \remarks This operator overload is provided so that we are able to
    /// conveniently compare two nom::ObjectTypeInfo objects with minimal
    /// effort -- note that we could not have ::type declared under private
    /// scope if not for this provision.
    bool operator ==( const self_type& rhs ) const;

    /// \brief In-equality comparison operator.
    bool operator !=( const self_type& rhs ) const;

  private:
    /// \brief Getter for the internal container type used.
    const value_type& type( void ) const;

    /// \brief Type information container.
    value_type type_;
};

/// \class nom::ObjectTypeInfo
/// \ingroup system
///
/// This utility class implementation is done primarily to help encapsulate
/// implementation details that are subject to availability -- not all platforms
/// support RTTI (typeid, std::type_index), nor are the supported platforms
/// implemented the same (i.e.: name strings).
///
/// See also: nom::IObject.
///
/// References (class design & ideas):
///
/// http://stackoverflow.com/questions/5330835/why-is-stdtype-info-noncopyable-am-i-allowed-to-store-it-somewhere
/// http://docs.wxwidgets.org/trunk/classwx_class_info.html
///

/// \brief Helper method for our factory API.
///
/// \remarks This wrapper structure should always be used (instead of typeid),
/// in order to encapsulate functionality that is subject to availability.
template<typename T>
struct RTTIObject
{
  typedef T type;
};

/// \brief Factory API for creation of an ObjectTypeInfo object.
///
/// \remarks You should *always* use this API call for the creation of
/// ObjectTypeInfo objects.
///
/// \note The idea behind implementing this factory interface was to
/// encapsulate implementation details that are subject to availability -- not
/// all platforms support RTII, nor do the supported platforms implement
/// specifics the same (i.e.: name strings).
template<class T>
static ObjectTypeInfo ObjectType()
{
  return ObjectTypeInfo( typeid( RTTIObject<T> ) );
}

/// \brief Convenience macro for wrapping the slightly ugly factory API syntax.
#define NOM_OBJECT_TYPE_INFO( type ) \
  ( ObjectType<type>() )

} // namespace nom

#endif // include guard defined
