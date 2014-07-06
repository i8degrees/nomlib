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
    /// \brief Default constructor; initialize the directory paths that can be
    /// used for tests.
    ///
    /// \remarks This method is called at the start of each unit test.
    FileInterfaceTest( void )
    {
      // TODO: this->fp.system_temp_path();

      #if defined( NOM_PLATFORM_POSIX )
        this->sys_temp = Path( "/tmp" );
      #elif defined( NOM_PLATFORM_WINDOWS )
        this->sys_temp = Path( "C:\\Windows\\Temp" );
      #endif

      this->nom_dir = Path( this->sys_temp.prepend( "nomlib" ) );
      this->nom_rdir = Path( this->nom_dir.prepend( "nom_rdir" ) );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~FileInterfaceTest( void )
    {
      // Nothing to be done...
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp( void )
    {
      // Nothing to be done...
    }

    /// \brief Sanitize (remove) the directory paths that could have been used
    /// during the tests.
    ///
    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    ///
    /// \note The directory creation & removal tests depend on proper clean up
    /// of the directory paths used for running the tests multiple times.
    virtual void TearDown( void )
    {
      // Note that the order of removal is reverse order (of creation).
      if( this->fp.exists( this->nom_rdir.path() ) == true )
      {
        EXPECT_EQ( true, this->fp.rmdir( this->nom_rdir.path() ) )
        << "Could not sanitize directory paths in between tests: " << std::endl
        << this->nom_rdir.path() << std::endl
        << "Please try deleting the directory path before re-running the tests.";

        EXPECT_EQ( false, this->fp.exists( this->nom_rdir.path() ) )
        << "Directory path still exists after deletion: " << this->nom_rdir.path()
        << std::endl << "Please delete the directory path before re-running tests.";
      }

      if( this->fp.exists( this->nom_dir.path() ) == true )
      {
        EXPECT_EQ( true, this->fp.rmdir( this->nom_dir.path() ) )
        << "Could not sanitize directory paths in between tests: " << std::endl
        << this->nom_dir.path() << std::endl
        << "Please try deleting the directory path before re-running the tests.";

        EXPECT_EQ( false, this->fp.exists( this->nom_dir.path() ) )
        << "Directory path still exists after deletion: " << this->nom_dir.path()
        << std::endl << "Please delete the directory path before re-running tests.";
      }
    }

  protected:
    Path p;
    File fp;

    /// \brief The platform-defined system temporary files directory path.
    Path sys_temp;

    /// \brief The directory path to try creating in the appropriate tests.
    Path nom_dir;

    /// \brief The recursive directory path to try creating in the appropriate
    /// tests.
    Path nom_rdir;
};

TEST_F( FileInterfaceTest, PathDelimiter )
{
  #if defined( NOM_PLATFORM_POSIX )
    EXPECT_EQ( "/", p.native() );
  #elif defined ( NOM_PLATFORM_WINDOWS )
    EXPECT_EQ( "\\", p.native() );
  #else // Err
    FAIL() << std::endl << "Unknown / possibly unsupported platform" << std::endl;
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

/// \fixme Look into why "." is not valid under Windows for pwd;
/// see also: ::path, ::currentPath?
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

  // FIXME: Why does this fail only under Windows? My guess is that perhaps "."
  // isn't recognized / valid under Windows...
  ASSERT_FALSE( pwd == "." );
}

TEST_F( FileInterfaceTest, FileExtensions )
{
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

TEST_F( FileInterfaceTest, CreateRemoveDirectory )
{
  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    // NOM_DUMP_VAR("system temp path: ", this->sys_temp.path() );
    NOM_DUMP_VAR("nomlib temp path: ", this->nom_dir.path() );
  #endif

  EXPECT_EQ( true, this->fp.mkdir( this->nom_dir.path() ) )
  << "Could not create directory path: " << this->nom_dir.path();

  EXPECT_EQ( true, this->fp.is_dir( this->nom_dir.path() ) )
  << "Path created should be a directory entry: " << this->nom_dir.path();

  EXPECT_EQ( false, this->fp.is_file( this->nom_dir.path() ) )
  << "Path created should NOT be be a file entry: " << this->nom_dir.path();

  // Perhaps file security access issue, or flawed internal logic of our
  // functions occurred if this assertion fails?
  EXPECT_EQ( true, this->fp.exists( this->nom_dir.path() ) )
  << "Path created does not exist: " << this->nom_dir.path();
}

TEST_F( FileInterfaceTest, CreateRemoveRecursiveDirectory )
{
  #if defined( NOM_DEBUG_FILE_TEST_OUTPUT )
    // NOM_DUMP_VAR("system temp path: ", this->sys_temp.path() );
    NOM_DUMP_VAR("nomlib recursive directory path: ", this->nom_rdir.path() );
  #endif

  EXPECT_EQ( true, this->fp.recursive_mkdir( this->nom_rdir.path() ) )
  << "Could not create directory path: " << this->nom_rdir.path();

  EXPECT_EQ( true, this->fp.is_dir( this->nom_rdir.path() ) )
  << "Path created should be a directory entry: " << this->nom_rdir.path();

  EXPECT_EQ( false, this->fp.is_file( this->nom_rdir.path() ) )
  << "Path created should NOT be be a file entry: " << this->nom_rdir.path();

  // Perhaps file security access issue, or flawed internal logic of our
  // functions occurred if this assertion fails?
  EXPECT_EQ( true, this->fp.exists( this->nom_rdir.path() ) )
  << "Path created does not exist: " << this->nom_rdir.path();
}

TEST_F( FileInterfaceTest, MakeFile )
{
  File fp;

  EXPECT_TRUE( fp.mkfile( "nom_file" ) );

  EXPECT_TRUE( fp.is_file( "nom_file" ) );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
