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
#ifndef NOMLIB_SYSTEM_RESOURCE_FILE_HPP
#define NOMLIB_SYSTEM_RESOURCE_FILE_HPP

#include <string>
#include <vector>
#include <memory>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Descriptor of a file-based resource
///
/// \see nom::ResourceCache.
class ResourceFile
{
  public:
    typedef ResourceFile self_type;

    static const ResourceFile null;

    enum Type
    {
      Invalid = 0,
      Graphic,
      Movie,
      Audio,
      TrueTypeFont,
      BitmapFont
    };

    /// \brief Default constructor; initialize resource to an invalid state.
    ResourceFile( void );

    /// \brief Destructor.
    virtual ~ResourceFile( void );

    /// \brief Construct an invalid resource.
    ///
    /// \remarks This constructor is intended for use with internal queries made
    /// to the resource cache.
    ResourceFile( const std::string& name );

    /// \brief Construct a minimal, but valid resource.
    ResourceFile( const std::string& name, const std::string& file_path );

    /// \brief Construct a complete resource.
    ResourceFile( const std::string& name, const std::string& file_path, ResourceFile::Type type );

    /// \brief Lesser-than comparison method overload.
    ///
    /// \remarks The resource name is used in the comparison.
    ///
    /// \note This operator method overload is a required implementation for
    /// use within a std::map.
    bool operator <( const self_type& rhs ) const;

    /// \brief Equality comparison method overload.
    ///
    /// \note This operator method overload is a required implementation for
    /// use within a std::map.
    bool operator ==( const self_type& rhs ) const;

    /// \brief Get the status of resource existence at the file-system level.
    bool exists( void ) const;

    /// \brief Get the name of the resource.
    const std::string& name( void ) const;

    /// \brief Get the absolute file path to the resource.
    const std::string& path( void ) const;

    /// \brief Get the resource's type.
    ///
    /// \see nom::RESOURCE_TYPE enumeration.
    ResourceFile::Type type( void ) const;

    bool loaded( void ) const;

    /// \brief Set the resource's name.
    void set_name( const std::string& name );

    /// \brief Set the resource's absolute file path.
    void set_path( const std::string& file_path );

    /// \brief Set the resource's type.
    void set_type( ResourceFile::Type type );

    void set_loaded( bool state );

  private:
    /// \brief The resource's referenced name (think: key).
    std::string name_;

    /// \brief The absolute file path to the resource.
    std::string path_;

    /// \brief The description of the resource.
    ResourceFile::Type type_;

  public:
    /// \brief Initialized resource object state.
    ///
    /// \fixme No idea why I have to declare this mutable and access it
    /// directly, but ::ResourceCache::load_resource refuses to compile
    /// otherwise, complaining about const issues with ::set_loaded.
    mutable bool loaded_;
};

} // namespace nom

#endif // include guard defined
