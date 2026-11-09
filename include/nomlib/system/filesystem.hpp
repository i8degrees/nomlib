/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

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
#ifndef NOMLIB_FILESYSTEM_HEADERS
#define NOMLIB_FILESYSTEM_HEADERS

#include <cstdint>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/export.hpp"

namespace nom {
namespace filesystem {

/// \brief A C++14 reimplementation of the path abstraction described in
/// ISO/IEC TS 18822:2015 (N4100) / Boost.Filesystem.
///
/// Objects of this class represent a path syntactically; they do not require
/// the path to exist on the filesystem. Operations are modeled on the generic
/// pathname grammar, with platform-specific adjustments for Windows
/// root-names and preferred separators.
class NOM_EXPORT path
{
  public:
    using value_type = char;
    using string_type = std::string;

    #if defined( NOM_PLATFORM_WINDOWS )
    static const value_type preferred_separator;
    #else
    static const value_type preferred_separator;
    #endif

    path();
    path( const path& other );
    path( path&& other ) noexcept;
    path( const string_type& source );
    path( const char* source );
    ~path();

    path& operator=( const path& other );
    path& operator=( path&& other ) noexcept;
    path& operator=( const string_type& source );
    path& operator=( const char* source );

    path& assign( const string_type& source );

    path& operator/=( const path& other );
    path& operator/=( const string_type& source );
    path& operator/=( const char* source );

    path& operator+=( const path& other );
    path& operator+=( const string_type& source );
    path& operator+=( const char* source );

    void clear() noexcept;

    /// Return the path in the platform's native format.
    const string_type& native() const;

    /// Return a NUL-terminated C-string in native format.
    const value_type* c_str() const;

    /// Convenience alias for native().
    string_type string() const;

    /// For nomlib, generic and native strings are interchangeable.
    string_type generic_string() const;

    /// \name Decomposition
    /// \{
    path root_name() const;
    path root_directory() const;
    path root_path() const;
    path relative_path() const;
    path parent_path() const;
    path filename() const;
    path stem() const;
    path extension() const;
    /// \}

    /// \name Queries
    /// \{
    bool has_root_name() const;
    bool has_root_directory() const;
    bool has_root_path() const;
    bool has_relative_path() const;
    bool has_parent_path() const;
    bool has_filename() const;
    bool has_stem() const;
    bool has_extension() const;

    bool is_absolute() const;
    bool is_relative() const;

    bool empty() const;
    /// \}

  private:
    void parse( const string_type& source );
    void invalidate_cache() const;
    void rebuild_cache() const;

    mutable string_type native_pathname_;
    mutable bool dirty_;

    string_type root_name_;
    bool has_root_directory_;
    bool absolute_;
    std::vector<string_type> components_;
    bool has_trailing_separator_;
};

/// \brief Append two paths using the filesystem path-append rules.
NOM_EXPORT path operator/( const path& lhs, const path& rhs );
NOM_EXPORT path operator/( const path& lhs, const std::string& rhs );
NOM_EXPORT path operator/( const path& lhs, const char* rhs );
NOM_EXPORT path operator/( const std::string& lhs, const path& rhs );

/// \brief Concatenate two paths without inserting a separator.
NOM_EXPORT path operator+( const path& lhs, const path& rhs );
NOM_EXPORT path operator+( const path& lhs, const std::string& rhs );
NOM_EXPORT path operator+( const path& lhs, const char* rhs );

/// \name Operational functions
/// \{

/// \brief Test if the path resolves to an existing file system object.
NOM_EXPORT bool exists( const path& p );

/// \brief Test if the path resolves to an existing directory.
NOM_EXPORT bool is_directory( const path& p );

/// \brief Test if the path resolves to an existing regular file.
NOM_EXPORT bool is_regular_file( const path& p );

/// \brief Create a single directory.
///
/// \returns true if the directory was newly created; false if it already
/// exists or could not be created.
NOM_EXPORT bool create_directory( const path& p );

/// \brief Create a directory and all missing parent directories.
///
/// \returns true if the leaf directory was newly created; false if it already
/// exists or could not be created.
NOM_EXPORT bool create_directories( const path& p );

/// \brief Remove a file or empty directory.
///
/// \returns true if an object was removed.
NOM_EXPORT bool remove( const path& p );

/// \brief Return the size (in bytes) of a regular file.
NOM_EXPORT std::uintmax_t file_size( const path& p );

/// \brief Return the current working directory.
NOM_EXPORT path current_path();

/// \brief Change the current working directory.
NOM_EXPORT void current_path( const path& p );

/// \}

} // namespace filesystem
} // namespace nom

#endif // include guard

/// \class nom::filesystem::path
/// \ingroup system
