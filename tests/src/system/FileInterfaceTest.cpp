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

#include <fstream>

#include <nomlib/system.hpp>
#include "nomlib/system/filesystem.hpp"

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

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, pwd );

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );

  pwd = fp.path( "Resources" + p.native() + "fonts" + p.native() + "executable" );

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, pwd );

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );
}

TEST_F( FileInterfaceTest, ExecutableWorkingDirectoryPath )
{
  std::string cwd = fp.currentPath();
  std::string pwd = "\0";

  pwd = fp.path( cwd );

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, fp.currentPath() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, pwd );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, cwd );

  ASSERT_FALSE( pwd == "" );
  ASSERT_FALSE( pwd == "." );
}

TEST_F( FileInterfaceTest, FileExtensions )
{
  std::string file( "times.ttf" );
  std::string file_ext;

  file_ext = fp.extension( file );

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, p.path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, file );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, file_ext ); // ttf

  ASSERT_FALSE( file_ext == "" );
  ASSERT_TRUE( file_ext == "ttf" );

  p = nom::Path( "Resources/gui" );

  // Path should resolve to: Resources/gui/VIII.png
  file = p.prepend( "VIII.png" );
  file_ext = "\0";

  file_ext = fp.extension( file );

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, p.path() ); // Resources/gui/
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, file );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, file_ext ); // png

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

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "native_path_delimiter: ", p.native() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "user_home_path: ", fp.user_home_path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "user_library_path: ", u.path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "user_fonts_path: ", user.path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "system_path: ", fp.system_path() );
}

TEST_F( FileInterfaceTest, CreateRemoveDirectory )
{
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "system temp path: ", this->sys_temp.path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "nomlib temp path: ", this->nom_dir.path() );

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
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "system temp path: ", this->sys_temp.path() );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "nomlib recursive directory path: ", this->nom_rdir.path() );

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

TEST_F( FileInterfaceTest, FilesystemPathConstruction )
{
  using namespace nom::filesystem;

  path p("Resources/tests/audio/4s.wav");

  EXPECT_EQ( "Resources/tests/audio/4s.wav", p.native() );
  EXPECT_EQ( "Resources/tests/audio/4s.wav", p.string() );
  EXPECT_EQ( "Resources/tests/audio/4s.wav", p.generic_string() );
  EXPECT_EQ( "Resources/tests/audio/4s.wav", p.u8string() );
  EXPECT_EQ( "Resources/tests/audio/4s.wav", p.generic_u8string() );
  EXPECT_EQ( "4s.wav", p.filename().native() );
  EXPECT_EQ( ".wav", p.extension().native() );
  EXPECT_EQ( "4s", p.stem().native() );
  EXPECT_EQ( "Resources/tests/audio", p.parent_path().native() );
  EXPECT_TRUE( p.has_filename() );
  EXPECT_TRUE( p.has_extension() );
  EXPECT_TRUE( p.has_parent_path() );
  EXPECT_FALSE( p.is_absolute() );
}

TEST_F( FileInterfaceTest, FilesystemPathUtf8Content )
{
  using namespace nom::filesystem;

  std::string utf8 = "/tmp/nomlib_音乐_🎵_test.txt";
  path p(utf8);

  EXPECT_EQ( utf8, p.native() );
  EXPECT_EQ( utf8, p.u8string() );
  EXPECT_EQ( utf8, p.generic_u8string() );
  EXPECT_EQ( "nomlib_音乐_🎵_test.txt", p.filename().native() );
  EXPECT_EQ( ".txt", p.extension().native() );
  EXPECT_EQ( "nomlib_音乐_🎵_test", p.stem().native() );
}

TEST_F( FileInterfaceTest, FilesystemPathUtf16RoundTrip )
{
  using namespace nom::filesystem;

  std::string utf8 = "/tmp/nomlib_音乐_🎵_test";
  path p8(utf8);
  std::u16string u16 = p8.u16string();

  path p16(u16);
  EXPECT_EQ( utf8, p16.native() );
  EXPECT_EQ( u16, p16.u16string() );
  EXPECT_EQ( u16, p16.generic_u16string() );
}

TEST_F( FileInterfaceTest, FilesystemPathWstringRoundTrip )
{
  using namespace nom::filesystem;

  std::string utf8 = "/tmp/nomlib_音乐_🎵_test";
  path p8(utf8);
  std::wstring w = p8.wstring();

  path pw(w);
  EXPECT_EQ( utf8, pw.native() );
  EXPECT_EQ( w, pw.wstring() );
  EXPECT_EQ( w, pw.generic_wstring() );
}

TEST_F( FileInterfaceTest, FilesystemPathAssign )
{
  using namespace nom::filesystem;

  path p;
  p.assign( std::u16string(u"音乐/file.txt") );
  EXPECT_EQ( "音乐/file.txt", p.native() );

  p = std::wstring(L"another/路径.txt");
  EXPECT_EQ( "another/路径.txt", p.native() );

  p.assign( "/tmp/final.txt" );
  EXPECT_EQ( "/tmp/final.txt", p.native() );
}

TEST_F( FileInterfaceTest, FilesystemPathAppend )
{
  using namespace nom::filesystem;

  std::string sep( 1, path::preferred_separator );
  path base("/tmp");
  path result = base / "音乐" / "test.txt";

  EXPECT_EQ( std::string("/tmp") + sep + "音乐" + sep + "test.txt",
             result.native() );
  EXPECT_EQ( "test.txt", result.filename().native() );
  EXPECT_EQ( ".txt", result.extension().native() );

  path concat("Resources");
  concat += "/gui.png";
  EXPECT_EQ( "Resources/gui.png", concat.native() );
}

TEST_F( FileInterfaceTest, FilesystemCurrentPath )
{
  using namespace nom::filesystem;

  path cwd = current_path();
  EXPECT_FALSE( cwd.empty() );
  EXPECT_TRUE( exists(cwd) );
  EXPECT_TRUE( is_directory(cwd) );
}

TEST_F( FileInterfaceTest, FilesystemUtf8DirectoryOps )
{
  using namespace nom::filesystem;

  path p("/tmp/nomlib_音乐");
  ASSERT_FALSE( exists(p) );

  EXPECT_TRUE( create_directory(p) );
  EXPECT_TRUE( exists(p) );
  EXPECT_TRUE( is_directory(p) );
  EXPECT_FALSE( is_regular_file(p) );

  EXPECT_TRUE( remove(p) );
  EXPECT_FALSE( exists(p) );
}

TEST_F( FileInterfaceTest, FilesystemUtf8RecursiveDirectoryOps )
{
  using namespace nom::filesystem;

  path leaf("/tmp/nomlib_音乐_parents_🎵_leaf");
  ASSERT_FALSE( exists(leaf) );

  EXPECT_TRUE( create_directories(leaf) );
  EXPECT_TRUE( exists(leaf) );
  EXPECT_TRUE( is_directory(leaf) );

  EXPECT_TRUE( remove(leaf) );
  EXPECT_FALSE( exists(leaf) );

  // Clean up the intermediate parent directory created by create_directories.
  path parent = leaf.parent_path();
  if( !parent.empty() && exists(parent) )
  {
    remove(parent);
  }
}

TEST_F( FileInterfaceTest, FilesystemFileSize )
{
  using namespace nom::filesystem;

  std::string content = "Hello, nomlib filesystem! 音乐 🎵";
  path p("/tmp/nomlib_filesystem_size_test.txt");

  {
    std::ofstream out( p.c_str(), std::ios::binary );
    ASSERT_TRUE( out.is_open() );
    out.write( content.data(), content.size() );
  }

  EXPECT_EQ( content.size(), file_size(p) );
  EXPECT_TRUE( is_regular_file(p) );
  EXPECT_FALSE( is_directory(p) );

  EXPECT_TRUE( remove(p) );
  EXPECT_FALSE( exists(p) );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
