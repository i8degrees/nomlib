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
#ifndef NOMLIB_SYSTEM_IOBJECT_HPP
#define NOMLIB_SYSTEM_IOBJECT_HPP

#include <sstream>
#include <new>

#include "nomlib/config.hpp"
#include "nomlib/system/ObjectTypeInfo.hpp"

namespace nom {

/// \brief Root object hierarchy for object type identification (RTTI).
class IObject
{
  public:
    typedef IObject self_type;

    typedef IObject* raw_ptr;

    static uint64 total_alloc_bytes;
    static uint64 total_dealloc_bytes;

    /// \brief Default constructor.
    IObject( void );

    /// \brief Virtual destructor.
    virtual ~IObject( void );

    /// \brief Obtain the object's class type.
    ///
    /// \returns The object's uniquely identifying class type; the default
    /// implementation of this method will always return IObject's type.
    ///
    /// \remarks This method should be overridden anytime you need to do things
    /// like identify particular object type(s) -- i.e.: nom::Sprite objects
    /// within a larger group of objects, such as a the typical std::vector of
    /// nom::IDrawable objects.
    ///
    /// Generally, you can get away with a simple declaration, like so:
    ///
    /// \code
    ///
    /// ObjectTypeInfo ExampleClass::type( void ) const
    /// {
    ///   return NOM_OBJECT_TYPE( ExampleClassName );
    /// }
    ///
    /// \endcode
    ///
    /// \note See also: NOM_OBJECT_TYPE_INFO macro.
    virtual ObjectTypeInfo type( void ) const;

    /// \brief Compare this object type with another object's type.
    ///
    /// \returns The default implementation returns boolean TRUE if the
    /// comparison of nom::IObject object's type is the same (identical) as the
    /// other specified object, or boolean FALSE if the comparison of
    /// nom::IObject object's type against the other is not identical.
    ///
    /// \remarks Example re-implementation examples could include usage of
    /// static_cast or dynamic_cast in order to determine more complicated
    /// object type hierarchies, such as an object derived from a base class.
    virtual bool is_type( const ObjectTypeInfo& rhs ) const;

    /// \brief Compare two objects to determine if a derived class inherits from
    /// a base class object.
    ///
    /// \returns Boolean TRUE if the derived object inherits from the specified
    /// base class, boolean FALSE if the derived object does not inherit from
    /// the specified base class.
    ///
    /// \remarks Example uses of this include checking to see if an
    /// nom::IDrawable object of inherits nom::Transformable, in order to
    /// perform transformations on the object before its ready for rendering.
    template <typename Base, typename Derived>
    static bool is_a( const Derived object )
    {
      if( dynamic_cast<Base>( object ) != nullptr )
      {
        return true;
      }

      return false;
    }

    static void* operator new( size_t mem );
    static void operator delete( void* ptr );
};

/// \brief Equality comparison operator.
inline bool operator ==( const IObject& lhs, const IObject& rhs )
{
  return( &lhs == &rhs );
}

/// \brief In-equality comparison operator.
inline bool operator !=( const IObject& lhs, const IObject& rhs )
{
  return ! ( &lhs == &rhs );
}

} // namespace nom

#define NOM_ISA(type, expr) nom::IObject::is_a<type>( expr )

#endif // include guard defined

/// \class nom::IObject
/// \ingroup system
///
/// References (class design, ideas, inspiration):
///
/// http://stackoverflow.com/questions/5330835/why-is-stdtype-info-noncopyable-am-i-allowed-to-store-it-somewhere
/// http://docs.wxwidgets.org/trunk/classwx_object.html
/// http://docs.wxwidgets.org/trunk/classwx_class_info.html
/// http://msdn.microsoft.com/en-us/library/system.object(v=vs.110).aspx?cs-save-lang=1&cs-lang=cpp#code-snippet-2
///
