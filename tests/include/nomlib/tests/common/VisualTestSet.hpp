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
#ifndef NOMLIB_TESTS_COMMON_VISUAL_TEST_SET_HPP
#define NOMLIB_TESTS_COMMON_VISUAL_TEST_SET_HPP

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"

namespace nom {

// Forward declarations
class IValueDeserializer;
class IValueSerializer;

/// \brief An object representing screen-shots from a test case
class VisualTestSet
{
  public:
    typedef VisualTestSet self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    /// \brief Default constructor; the version and timestamp are initialized
    /// to sane defaults.
    VisualTestSet( void );

    /// \brief Destructor.
    ~VisualTestSet( void );

    /// \brief Construct a complete object.
    ///
    /// \param set      The test set name to write to the output file; this is
    ///                 the test case description in Google Test.
    /// \param name     The test name to write to the output file.
    /// \param ts       The timestamp string to write to the output file.
    /// \param dir_path The absolute directory path to the test set.
    ///
    /// \note The test name is not currently used.
    ///
    /// \note The timestamp used should be synchronized with the directory's
    /// timestamp string, in the case of the test running in comparison image
    /// mode. The timestamp string is used in forming the image hyperlinks when
    /// HTML test results are created.
    ///
    /// \remarks The engine version used is automatically initialized.
    VisualTestSet (
                    const std::string& set,
                    const std::string& name,
                    const std::string& ts,
                    const Size2i& res,
                    const std::string& dir_path
                  );

    /// \brief Equality comparison operator overload.
    bool operator ==( const self_type& rhs );

    /// \brief In-equality comparison operator overload.
    bool operator !=( const self_type& rhs );

    /// \brief Greater than equality comparison operator overload.
    ///
    /// \remarks The timestamp string is used for the comparison; this is always
    /// accurate because lexicographical ordering is also chronological.
    bool operator >( const self_type& rhs );

    /// \brief Lesser than equality comparison.
    ///
    /// \remarks The timestamp string is used for the comparison; this is always
    /// accurate because lexicographical ordering is also chronological.
    bool operator <( const self_type& rhs );

    /// \brief Get the image list for this test set.
    const std::vector<std::string>& images( void ) const;

    /// \brief Get the image file path for an element in the stored test set.
    std::string image( int pos ) const;

    /// \brief Get the name of the test set.
    ///
    /// \remarks This should match the name for the test case when used within
    // the Google Test framework.
    const std::string& test_set( void ) const;

    /// \brief Get the name of the test.
    ///
    /// \remarks This should match the name for the test description when used
    /// within the Google Test framework.
    const std::string& test_name( void ) const;

    /// \brief Get the recorded version.
    ///
    /// \remarks This should match the version of nomlib used.
    const std::string& version( void ) const;

    /// \brief Get the recorded timestamp.
    const std::string& timestamp( void ) const;

    /// \brief Get the recorded test comment.
    const std::string& comment( void ) const;

    /// \brief Get the resolution used for the test set.
    const Size2i& resolution( void ) const;

    /// \brief Get the directory path prefix.
    const std::string& directory( void ) const;

    /// \brief Set the name of the test set.
    void set_test_set( const std::string& set );

    /// \brief Set the test name for the test set.
    void set_test_name( const std::string& name );

    /// \brief Set the version of the test set.
    void set_version( const std::string& ver );

    /// \brief Set the recording time of the test set.
    ///
    /// \note The timestamp used should be synchronized with the directory's
    /// timestamp string, in the case of the test running in comparison image
    /// mode. The timestamp string is used in forming the image hyperlinks when
    /// HTML test results are created.
    void set_timestamp( const std::string& ts );

    /// \brief Set an optional comment for the test set.
    void set_comment( const std::string& comment );

    /// \brief Set the width and height (in pixels) used for the image files.
    void set_resolution( const Size2i& res );

    /// \brief Set the directory path.
    ///
    /// \remarks This path is a prefix that is used for saving and loading
    /// configuration files and its associated image files from.
    void set_directory( const std::string& path );

    /// \brief Insert an image into the batch collection.
    ///
    /// \param filename The relative path to the image file; this includes only
    /// the name of the file and its extension.
    ///
    /// \remarks The directory path set for the object should be the prefix
    /// used for locating the image file; the directory path prefixed onto the
    /// image file path should form a valid absolute path.
    void append_image( const std::string& filename );

    /// \brief Output the object properties to a configuration file.
    ///
    /// \remarks The directory for the object should be set before calling this
    /// method.
    bool save_file( IValueSerializer* cfg );

    /// \brief Initialize the object with an existing configuration file.
    ///
    /// \remarks The directory for the object should be set before calling this
    /// method.
    bool load_file( IValueDeserializer* cfg );

  private:
    /// \brief The image files from this batch.
    std::vector<std::string> images_;

    /// \brief The test set; this is the same as Google Test's test case name
    /// field.
    std::string test_set_;

    /// \brief The test name; this is the same as Google Test's test description
    /// field.
    std::string test_name_;

    /// \brief The version of nomlib used.
    std::string version_;

    /// \brief The recorded time of the test run.
    std::string timestamp_;

    /// \brief The user's comment made at the time of the run.
    std::string comment_;

    /// \brief The width and height of the test run.
    Size2i resolution_;

    /// \brief The absolute directory path used for saving and loading the
    /// configuration file from.
    std::string directory_;
};

typedef std::vector<VisualTestSet> VisualTestSetBatch;

} // namespace nom

#endif // include guard defined

/// \class nom::VisualTestSet
/// \ingroup tests
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
