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
#include "nomlib/tests/common/UnitTest.hpp"

// Private headers (third-party)
#include "tclap/CmdLine.h"    // TCLAP command line parser

// Private headers
#include "nomlib/version.hpp"       // nom::revision
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

namespace priv {

// Static initializations
UnitTestFlags test_flags = UnitTestFlags();

} // namespace priv

// Static initializations
std::string test_output_directory = "\0";

void init_test( int argc, char** argv )
{
  Path p;
  File fp;

  // NOM_LOG_TRACE( NOM );

  // Initialize test state flags to their respective defaults; these values are
  // used for the defaults in the command line processing below.
  NOM_TEST_FLAG(interactive) = false;
  NOM_TEST_FLAG(disable_comparison) = false;
  NOM_TEST_FLAG(reference_screenshot) = false;
  NOM_TEST_FLAG(test_comment) = "\0";
  NOM_TEST_FLAG(comparison_dir) = "Reference";
  NOM_TEST_FLAG(no_html_output) = false;
  NOM_TEST_FLAG(force_overwrite) = false;

  if( argc < 0 )
  {
    return;
  }

  try
  {
    // Documentation: http://tclap.sourceforge.net/manual.html
    TCLAP::CmdLine cmd  (
                           // Program description
                          "nom::VisualUnitTest framework; a wrapper for Google Test integration",
                          ' ',
                          // Engine version number used
                          nom::revision()
                        );

    TCLAP::SwitchArg interactive  (
                                    // Short option form; -i
                                    "i",
                                    // Long option form; --interactive
                                    "interactive",
                                    // Option description
                                    "Run test interactively; do not auto-terminate main loop",
                                    cmd,
                                    // Option default
                                    NOM_TEST_FLAG(interactive)
                                  );

    TCLAP::SwitchArg reference_screenshot (
                                            "r",
                                            "reference",
                                            "Generate a reference image set",
                                            cmd,
                                            NOM_TEST_FLAG(reference_screenshot)
                                          );

    TCLAP::SwitchArg disable_comparison
        (
          // Option short form; -d
          "d",
          // Option long form; --disable-comparison
          "disable-comparison",
          // Option description
          "Supress image set comparison",
          cmd,
          // Option default
          NOM_TEST_FLAG(disable_comparison)
        );

    TCLAP::SwitchArg no_html_output (
                                      // Option short form is disabled
                                      "",
                                      // Option long form; --no-html
                                      "no-html",
                                      // Option description
                                      "Supress HTML test results output",
                                      cmd,
                                      // Option default
                                      NOM_TEST_FLAG(no_html_output)
                                    );

    TCLAP::ValueArg<std::string> comparison_dir (
                                                   // Option short form; -c
                                                  "c",
                                                  // Option long form; --compare
                                                  "compare",
                                                  // Option description
                                                  "Select which image set you wish to compare against this test run",
                                                  // Not required
                                                  false,
                                                  // Option default
                                                  NOM_TEST_FLAG(comparison_dir),
                                                  // Option example (part of description)
                                                  "Image set to compare against",
                                                  cmd
                                                );

    TCLAP::ValueArg<std::string> test_comment (
                                                // Option short form; -m
                                                "m",
                                                // Option long form; --message
                                                "message",
                                                // Option description
                                                "Add an optional comment to be associated with the generated image set",
                                                // Not required
                                                false,
                                                // Option default
                                                NOM_TEST_FLAG(test_comment),
                                                // Option example (part of description)
                                                "message",
                                                cmd
                                              );

    TCLAP::SwitchArg force_overwrite  (
                                        // Option short form; -f
                                        "f",
                                        // Option long form; --force
                                        "force",
                                        // Option description
                                        "Overwrite existing reference image set",
                                        cmd,
                                        // Option default
                                        NOM_TEST_FLAG(force_overwrite)
                                      );

    cmd.parse( argc, argv );

    if( interactive.getValue() == true )
    {
      NOM_TEST_FLAG( interactive ) = true;
    }

    if( reference_screenshot.getValue() == true )
    {
      NOM_TEST_FLAG( reference_screenshot ) = true;
    }

    if( test_comment.getValue() != "" )
    {
      NOM_TEST_FLAG( test_comment ) = test_comment.getValue();
    }

    if( comparison_dir.getValue() != "Reference" )
    {
      NOM_TEST_FLAG( comparison_dir ) = comparison_dir.getValue();

      NOM_LOG_ERR( NOM, "This option is not implemented." );
      exit( NOM_EXIT_FAILURE );
    }

    if( disable_comparison.getValue() == true )
    {
      NOM_TEST_FLAG( disable_comparison ) = true;
    }
    else
    {
      NOM_TEST_FLAG( disable_comparison ) = false;
    }

    if( no_html_output.getValue() == true )
    {
      NOM_TEST_FLAG( no_html_output ) = true;
    }
    else
    {
      NOM_TEST_FLAG( no_html_output ) = false;
    }

    if( force_overwrite.getValue() == true )
    {
      NOM_TEST_FLAG( force_overwrite ) = true;
    }
    else
    {
      NOM_TEST_FLAG( force_overwrite ) = false;
    }
  }
  catch( TCLAP::ArgException &e )
  {
    NOM_LOG_ERR( NOM, e.error(), " for arg ", e.argId() );
    exit( NOM_EXIT_FAILURE );
  }

  // if( this->output_directory() == "" )
  {
    Path user_docs_path = fp.user_documents_path();

    if( fp.exists( user_docs_path.path() ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not determine the user's documents directory path." );
      NOM_LOG_ERR( NOM, "Please fix this issue before running these tests." );
      exit( NOM_EXIT_FAILURE );
    }

    Path nomlib_tests_path = user_docs_path.path() + p.native() + "nomlib" + p.native() + "tests";

    if( fp.exists( nomlib_tests_path.path() ) == false )
    {
      if( fp.recursive_mkdir( nomlib_tests_path.path() ) == false )
      {
        NOM_LOG_ERR( NOM, "Could not create directory path for tests: ", nomlib_tests_path.path() );
        NOM_LOG_ERR( NOM, "Please create these directories before running these tests." );
        exit( NOM_EXIT_FAILURE );
      }
    }

    // OSX: /Users/jeff/Documents/nomlib/tests
    // Windows: C:\Users\jeff\Documents\nomlib\tests
    // POSIX: /home/jeff/Documents/nomlib/tests
    nom::test_output_directory = nomlib_tests_path.path();
  }
}

UnitTest::UnitTest( void ) :
  app_state_{ true }
{
  // NOM_LOG_TRACE( NOM );

  // Ensure that nom::init_test has been called before using nomlib's
  // integration framework. Otherwise, we crash if any of our code tries to
  // inspect or set any of our unit test state flags (generally specified via
  // command line).
  // if( priv::test_flags == nullptr )
  // {
    // Did you forget to call nom::init_test inside the main function before
    // running tests?
    // NOM_LOG_ERR( NOM, "This program must initialize with nom::init_test before calling any of its methods." );
    // exit( NOM_EXIT_FAILURE );
  // }

  ::testing::UnitTest* unit_test = ::testing::UnitTest::GetInstance();
  if( unit_test != nullptr )
  {
    const ::testing::TestCase* test_case = unit_test->current_test_case();
    const ::testing::TestInfo* test_info = unit_test->current_test_info();

    if( test_case != nullptr )
    {
      this->set_test_set( test_case->name() );
    }
    else
    {
    }

    if( test_info != nullptr )
    {
      this->set_test_name( test_info->name() );
    }
    else
    {
    }
  }
}

UnitTest::~UnitTest( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool UnitTest::app_state( void ) const
{
  return this->app_state_;
}

void UnitTest::set_app_state( bool state )
{
  this->app_state_ = state;
}

void UnitTest::quit( void )
{
  this->app_state_ = false;
}

const std::string& UnitTest::test_name( void )
{
  return this->test_name_;
}

const std::string& UnitTest::test_set( void )
{
  return this->test_set_;
}

const std::string& UnitTest::test_case()
{
  return this->test_set();
}

int UnitTest::total_test_count() const
{
  ::testing::UnitTest* unit_test = ::testing::UnitTest::GetInstance();

  NOM_ASSERT( unit_test != nullptr );

  if( unit_test != nullptr )
  {
    return unit_test->total_test_count();
  }

  return -1;
}

int UnitTest::failed_test_count() const
{
  ::testing::UnitTest* unit_test = ::testing::UnitTest::GetInstance();

  NOM_ASSERT( unit_test != nullptr );

  if( unit_test != nullptr )
  {
    return unit_test->failed_test_count();
  }

  return -1;
}

void UnitTest::set_test_name( const std::string& name )
{
  this->test_name_ = name;
}

void UnitTest::set_test_set( const std::string& name )
{
  this->test_set_ = name;
}

} // namespace nom
