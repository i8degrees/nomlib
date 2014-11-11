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
#ifndef NOMLIB_TESTS_UNIT_TEST_HPP
#define NOMLIB_TESTS_UNIT_TEST_HPP

#include <iostream>
#include <string>

#include "gtest/gtest.h"      // Google Test framework

#include "nomlib/config.hpp"

/// \brief Set or get a global state control flag.
#define NOM_TEST_FLAG(var) nom::priv::test_flags.var

namespace nom {

namespace priv {

/// \brief Available global state control flags for the nom::UnitTest framework
///
/// \see nom::init_test.
struct UnitTestFlags
{
  /// \brief Whether or not to run unit tests within a game loop.
  ///
  /// \see VisualUnitTest.
  bool interactive;

  /// \brief Disable image set comparison
  ///
  /// \remarks This also disables reference image sets from being generating.
  bool disable_comparison;

  /// \brief Generate a reference image set.
  ///
  /// \remarks The reference image set is used for comparison results.
  ///
  /// \see VisualUnitTest.
  bool reference_screenshot;

  /// \brief Optional test message for info.json meta-data file.
  ///
  /// \remarks This string is used in HTML results output.
  ///
  /// \see ImageTestSet
  std::string test_comment;

  /// \brief The image set used for comparison.
  ///
  /// \remarks This is not implemented; reserved for future use.
  ///
  /// \see ImageTestSet
  std::string comparison_dir;

  /// \brief Whether or not to output HTML test results.
  ///
  /// \see UnitTestResultsWriter
  bool no_html_output;

  /// \brief Toggle the control logic of overwriting existing reference image
  /// sets.
  ///
  /// \see UnitTestResultsWriter
  bool force_overwrite;
};

/// \brief Global state control flags
///
/// \remarks This should be initialized only once during the program execution,
/// before anything else from the nom::UnitTest framework is constructed.
extern UnitTestFlags test_flags;

} // namespace priv

/// \brief The base directory path used for test output, i.e.: results,
/// screen-shots dumps, etc.
///
/// \remarks The default prefix is nomlib/tests under the user's documents
/// directory.
///
/// \see nom::init_test.
extern std::string test_output_directory;

/// \brief Initialize nomlib's unit testing framework.
///
/// \remarks This method parses the command line for state control options
/// supported by the framework.
///
/// \note This method should be called **after** Google Test's initialization
/// and before the use of any other feature of the nom::UnitTest framework,
/// generally within the main execution function.
///
/// \see nom::UnitTest, nom::UnitTestFlags, nom::VisualUnitTest
void init_test( int argc, char** argv );

/// \brief Base class interface for unit testing within Google Test
class UnitTest: public ::testing::Test
{
  public:
    /// \brief Default constructor; initialize the test name and test set with
    /// the information received from the underlying testing framework --
    /// Google Test.
    ///
    /// \note nom::init_test should be called before construction of this object.
    ///
    /// \see nom::init_test, nom::UnitTestFlags, nom::VisualUnitTest.
    UnitTest( void );

    /// \brief Destructor.
    ///
    /// \remarks This class interface is intended to be used via inheritance.
    virtual ~UnitTest( void );

    /// \brief Get the running state of the test.
    bool app_state( void ) const;

    /// \brief Toggle the running state flag of the test.
    void set_app_state( bool state );

    /// \brief Request termination of the test.
    ///
    /// \remarks The default implementation of this method is to toggle the
    /// app state variable to false. It is the responsibility of the
    /// implementation to honor this request.
    virtual void quit( void );

    /// \brief Get the name of the current test.
    const std::string& test_name( void );

    /// \brief Get the name of the current test case.
    ///
    /// \deprecated This method will be renamed to *test_case* at some point in
    /// the future.
    ///
    /// \see nom::UnitTest::test_case
    const std::string& test_set( void );

    /// \brief Get the name of the current test case.
    const std::string& test_case();

    /// \brief Gets the number of all tests in this test case.
    ///
    /// \returns Non-negative integer on success, or negative one (-1) on
    /// failure, such as the underlying unit testing framework being
    /// invalid / NULL.
    int total_test_count() const;

    /// \brief Gets the number of failed tests in this test case.
    ///
    /// \returns Non-negative integer on success, or negative one (-1) on
    /// failure, such as the underlying unit testing framework being
    /// invalid / NULL.
    int failed_test_count() const;

  private:
    FRIEND_TEST( UnitTestFrameworkTest, SetterMethods );

    /// \brief Set the current test name.
    ///
    /// \remarks The test name correlates to Google Test's test description
    /// field.
    void set_test_name( const std::string& name );

    /// \brief Set the current test set name.
    ///
    /// \remarks The test set name correlates to Google Test's test case field.
    void set_test_set( const std::string& name );

    /// \brief The name of the current test.
    std::string test_name_;

    /// \brief The name of the current test case.
    std::string test_set_;

    /// \brief The program state; defaults to true (running).
    bool app_state_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::UnitTest
/// \ingroup tests
///
/// A wrapper for integration of nomlib into the Google Test framework.
///
/// \see nom::VisualUnitTest
///
/// \see https://code.google.com/p/googletest/
