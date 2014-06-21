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
#include "gtest/gtest.h"

#include <nomlib/system.hpp>

#define NOM_DEBUG_FILE_TEST_OUTPUT

namespace nom {

class FileInterfaceTest: public ::testing::Test
{
  public:
    FileInterfaceTest( void )
    {
      //
    }

    ~FileInterfaceTest( void )
    {
      //
    }

    virtual void SetUp( void )
    {
      //
    }

    virtual void TearDown( void )
    {
      //
    }

  protected:
    File fp;
};

TEST_F( FileInterfaceTest, PathDelimiter )
{
  Path p;

  #if defined( NOM_PLATFORM_POSIX )
    EXPECT_EQ( "/", p.native() );
  #elif defined ( NOM_PLATFORM_WINDOWS )
    EXPECT_EQ( "\/", p.native() );
  #else // Unsupported platform?
    FAIL() << std::endl << "Unknown (unsupported) platform??" << std::endl;
  #endif
}

TEST_F( FileInterfaceTest, DirPath )
{
  std::string pwd = "\0";

  pwd = fp.path( "Resources" + p.native() + "fonts" + p.native() + "executable.exe" );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    NOM_DUMP(pwd);
  #endif

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );

  pwd = fp.path( "Resources" + p.native() + "fonts" + p.native() + "executable" );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    NOM_DUMP(pwd);
  #endif

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );
}

TEST_F( FileInterfaceTest, ExecutableWorkingDirectoryPath )
{
  std::string cwd = fp.currentPath();
  std::string pwd = "\0";

  pwd = fp.path( cwd );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    NOM_DUMP( fp.currentPath() );
    NOM_DUMP( pwd );
    NOM_DUMP( cwd );
  #endif

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );
}

TEST_F( FileInterfaceTest, FileExtensions )
{
  Path p;
  std::string file( "times.ttf" );
  std::string file_ext;

  file_ext = fp.extension( file );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    // NOM_DUMP( p.path() );
    NOM_DUMP( file );
    NOM_DUMP( file_ext ); // ttf
  #endif

  ASSERT_FALSE( file_ext == "" );
  ASSERT_TRUE( file_ext == "ttf" );

  p = nom::Path( "Resources/gui" );

  // Path should resolve to: Resources/gui/VIII.png
  file = p.prepend( "VIII.png" );
  file_ext = "\0";

  file_ext = fp.extension( file );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    NOM_DUMP( p.path() ); // Resources/gui/
    NOM_DUMP( file );
    NOM_DUMP( file_ext ); // png
  #endif

  ASSERT_FALSE( file_ext == "" );
  ASSERT_TRUE( file_ext == "png" );
}

TEST_F( FileInterfaceTest, PlatformPaths )
{
  Path p;

  Path u = fp.user_home_path() + p.native() + "Library";
  Path user( u.prepend( "Fonts" ) );

  ASSERT_FALSE( u.path() == "" );
  ASSERT_FALSE( fp.user_home_path() == "" );
  ASSERT_FALSE( u.path() == "" );
  ASSERT_FALSE( user.path() == "" );
  ASSERT_FALSE( fp.system_path() == "" );

  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    NOM_DUMP_VAR( "native_path_delimiter: ", p.native() );
    NOM_DUMP_VAR( "user_home_path: ", fp.user_home_path() );
    NOM_DUMP_VAR( "user_library_path: ", u.path() );
    NOM_DUMP_VAR( "user_fonts_path: ", user.path() );
    NOM_DUMP_VAR( "system_path: ", fp.system_path() );
  #endif
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
