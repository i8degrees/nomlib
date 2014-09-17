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
#ifndef NOMLIB_SYSTEM_SEARCH_PATH_HPP
#define NOMLIB_SYSTEM_SEARCH_PATH_HPP

#include <memory>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/File.hpp"

#include "nomlib/ptree.hpp"
#include "nomlib/serializers.hpp"

namespace nom {

/// \brief Determine the directory location to use based on search prefixes
///
/// \todo Add an interface for using this class without a file.
class SearchPath
{
  public:
    typedef SearchPath self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor; initialize the parser to NULL.
    SearchPath();

    /// \brief Destructor.
    ~SearchPath();

    /// \brief Get the resolved path.
    const std::string& path();

    /// \brief Set a custom parser.
    ///
    /// \remarks The default parser is JSON.
    void set_deserializer( std::unique_ptr<IValueDeserializer> fp );

    /// \brief Parse a file into memory.
    ///
    /// \param filename The absolute file path.
    /// \param node     The top-level object to use in formation of the path.
    ///
    /// \remarks The default parser is JSON, and can be changed using the
    /// ::set_deserializer method.
    bool load_file( const std::string& filename, const std::string& node );

  private:
    /// \brief Parser object to use.
    ///
    /// \see nom::IValueDeserializer
    std::unique_ptr<IValueDeserializer> fp_;

    /// \brief List of path prefixes to scan through with the given path.
    std::vector<std::string> search_prefix_;

    /// \brief The resolved path.
    std::string path_;
};

} // namespace nom

#endif // include guard defined

/// Usage example:
///
/// nom::SearchPath resources;
/// resources.load_file( "resources.json" );
///
/// ```resources.json```
/// (should generally be in the same directory as the executable)
///
/// \code
/// {
///   // Top-level object 'node'
///   "resources":
///   {
///     // List of paths to search
///     "search_prefix":
///     [
///       "./",
///       "../../",
///       "../../../"
///     ],
///     // The 'suffix' that is appended to each search prefix path during the
///     // scan; the first full path that exists will be what is returned on a
///     // call to ::path.
///     "path": "Resources/tests/gui/librocket/nomlibTest/"
///   }
/// }
/// \endcode
