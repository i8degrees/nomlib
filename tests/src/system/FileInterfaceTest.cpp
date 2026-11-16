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

#include <algorithm>
#include <cerrno>
#include <fstream>
#include <list>
#include <string>
#include <system_error>
#include <vector>

#include <nomlib/system.hpp>
#include "nomlib/system/filesystem.hpp"

namespace nom {

namespace {

/// \brief Open an std::ofstream using the platform-native path encoding.
///
/// On Windows, MSVC's standard library provides a wchar_t constructor for
/// fstreams, so we convert the UTF-8 path to UTF-16 via wstring(). On POSIX
/// systems, the narrow UTF-8 representation is opened directly.
std::ofstream open_output_stream( const filesystem::path& p,
                                  std::ios::openmode mode = std::ios::out )
{
#if defined( NOM_PLATFORM_WINDOWS )
  return std::ofstream( p.wstring().c_str(), mode );
#else
  return std::ofstream( p.c_str(), mode );
#endif
}

} // namespace

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
      if( filesystem::exists( "nom_file" ) ) {
        EXPECT_TRUE( filesystem::remove( "nom_file" ) );
      }
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

// res:// paths, i.e.: Apple Application Bundle -- .app layout
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

  auto native = []( std::string s ) {
    std::replace( s.begin(), s.end(), '/', path::preferred_separator );
    return s;
  };

  path p("Resources/tests/audio/4s.wav");

  EXPECT_EQ( native("Resources/tests/audio/4s.wav"), p.native() );
  EXPECT_EQ( native("Resources/tests/audio/4s.wav"), p.string() );
  EXPECT_EQ( native("Resources/tests/audio/4s.wav"), p.generic_string() );
  EXPECT_EQ( native("Resources/tests/audio/4s.wav"), p.u8string() );
  EXPECT_EQ( native("Resources/tests/audio/4s.wav"), p.generic_u8string() );
  EXPECT_EQ( "4s.wav", p.filename().native() );
  EXPECT_EQ( ".wav", p.extension().native() );
  EXPECT_EQ( "4s", p.stem().native() );
  EXPECT_EQ( native("Resources/tests/audio"), p.parent_path().native() );
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

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(p) )
  {
    remove(p);
  }

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

  // Use an intermediate root directory so that parent_path() points at a path
  // created by this test, not /tmp itself.
  path root("/tmp/nomlib_音乐_parents_🎵_root");
  path leaf( root / "leaf" );

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(leaf) )
  {
    remove(leaf);
  }
  if( exists(root) )
  {
    remove(root);
  }

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

  if( exists(root) )
  {
    remove(root);
  }
}

TEST_F( FileInterfaceTest, FilesystemFileSize )
{
  using namespace nom::filesystem;

  std::string content = "Hello, nomlib filesystem! 音乐 🎵";
  path p("/tmp/nomlib_filesystem_size_test.txt");

  // Defensive cleanup in case a prior run left the file behind.
  if( exists(p) )
  {
    remove(p);
  }

  {
    std::ofstream out = open_output_stream( p, std::ios::binary );
    ASSERT_TRUE( out.is_open() );
    out.write( content.data(), content.size() );
  }

  EXPECT_EQ( content.size(), file_size(p) );
  EXPECT_TRUE( is_regular_file(p) );
  EXPECT_FALSE( is_directory(p) );

  EXPECT_TRUE( remove(p) );
  EXPECT_FALSE( exists(p) );
}

TEST_F( FileInterfaceTest, FilesystemStatusQueries )
{
  using namespace nom::filesystem;

  // This test mirrors the Boost.Filesystem tut2.cpp exercise:
  //   - existing regular file  -> "<path> size is <size>"
  //   - existing directory     -> "<path> is a directory"
  //   - non-existent path      -> "<path> does not exist"

  const path dir("/tmp/nomlib_status_queries_test");
  const path file( dir / "data.txt" );
  const path missing( dir / "missing.txt" );

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(file) )
  {
    remove(file);
  }
  if( exists(dir) )
  {
    remove(dir);
  }

  ASSERT_FALSE( exists(dir) );

  EXPECT_TRUE( create_directories(dir) );

  const std::string content = "Hello, nomlib filesystem status queries!";
  {
    std::ofstream out = open_output_stream( file, std::ios::binary );
    ASSERT_TRUE( out.is_open() );
    out.write( content.data(), content.size() );
  }

  ASSERT_TRUE( exists(file) );
  ASSERT_TRUE( is_regular_file(file) );
  ASSERT_FALSE( is_directory(file) );
  EXPECT_EQ( content.size(), file_size(file) );

  ASSERT_TRUE( exists(dir) );
  ASSERT_TRUE( is_directory(dir) );
  ASSERT_FALSE( is_regular_file(dir) );

  ASSERT_FALSE( exists(missing) );
  ASSERT_FALSE( is_regular_file(missing) );
  ASSERT_FALSE( is_directory(missing) );

  EXPECT_TRUE( remove(file) );
  EXPECT_TRUE( remove(dir) );
  EXPECT_FALSE( exists(dir) );
}

TEST_F( FileInterfaceTest, FilesystemPathDecomposition )
{
  using namespace nom::filesystem;

  // This test mirrors the Boost.Filesystem path_info.cpp exercise. We compose
  // a relative path from three elements using operator/= and verify observers,
  // decomposition and query functions.

  auto native = []( std::string s ) {
    std::replace( s.begin(), s.end(), '/', path::preferred_separator );
    return s;
  };

  path p;
  p /= "foo";
  p /= "bar";
  p /= "baz.txt";

  EXPECT_EQ( native("foo/bar/baz.txt"), p.native() );
  EXPECT_EQ( native("foo/bar/baz.txt"), p.string() );
  EXPECT_EQ( native("foo/bar/baz.txt"), p.generic_string() );
  EXPECT_EQ( native("foo/bar/baz.txt"), p.u8string() );
  EXPECT_EQ( native("foo/bar/baz.txt"), p.generic_u8string() );

  // Iteration yields each path element.
  std::vector<std::string> parts;
  for( const auto& element : p )
  {
    parts.push_back( element.native() );
  }
  ASSERT_EQ( 3u, parts.size() );
  EXPECT_EQ( "foo", parts[0] );
  EXPECT_EQ( "bar", parts[1] );
  EXPECT_EQ( "baz.txt", parts[2] );

  // make_preferred() normalizes separators to the platform preferred form.
  EXPECT_EQ( native("foo/bar/baz.txt"), p.make_preferred().native() );
  EXPECT_TRUE( p.make_preferred().native() == path("foo/bar/baz.txt").native() );

  // Decomposition.
  EXPECT_EQ( "", p.root_name().native() );
  EXPECT_EQ( "", p.root_directory().native() );
  EXPECT_EQ( "", p.root_path().native() );
  EXPECT_EQ( native("foo/bar/baz.txt"), p.relative_path().native() );
  EXPECT_EQ( native("foo/bar"), p.parent_path().native() );
  EXPECT_EQ( "baz.txt", p.filename().native() );
  EXPECT_EQ( "baz", p.stem().native() );
  EXPECT_EQ( ".txt", p.extension().native() );

  // Queries.
  EXPECT_FALSE( p.empty() );
  EXPECT_FALSE( p.is_absolute() );
  EXPECT_TRUE( p.is_relative() );
  EXPECT_FALSE( p.has_root_name() );
  EXPECT_FALSE( p.has_root_directory() );
  EXPECT_FALSE( p.has_root_path() );
  EXPECT_TRUE( p.has_relative_path() );
  EXPECT_TRUE( p.has_parent_path() );
  EXPECT_TRUE( p.has_filename() );
  EXPECT_TRUE( p.has_stem() );
  EXPECT_TRUE( p.has_extension() );
}

TEST_F( FileInterfaceTest, FilesystemAbsolutePathDecomposition )
{
  using namespace nom::filesystem;

  // Mirrors the Boost.Filesystem path_info.cpp output for an absolute path:
  //   elements: "/", "foo", "bar", "baa.txt"
  //   root_name: ""
  //   root_directory: "/"
  //   root_path: "/"
  //   relative_path: "foo/bar/baa.txt"
  //   parent_path: "/foo/bar"
  //   filename: "baa.txt"
  //   stem: "baa"
  //   extension: ".txt"

  auto native = []( std::string s ) {
    std::replace( s.begin(), s.end(), '/', path::preferred_separator );
    return s;
  };

  path p;
  p /= "/foo";
  p /= "bar";
  p /= "baa.txt";

  // Parsing after appending "/foo" should have produced an absolute path.
  EXPECT_EQ( native("/foo/bar/baa.txt"), p.native() );
  EXPECT_TRUE( p.is_absolute() );
  EXPECT_FALSE( p.is_relative() );

  std::vector<std::string> parts;
  for( const auto& element : p )
  {
    parts.push_back( element.native() );
  }
  ASSERT_EQ( 4u, parts.size() );
  EXPECT_EQ( "/", parts[0] );
  EXPECT_EQ( "foo", parts[1] );
  EXPECT_EQ( "bar", parts[2] );
  EXPECT_EQ( "baa.txt", parts[3] );

  EXPECT_EQ( "", p.root_name().native() );
  EXPECT_EQ( "/", p.root_directory().native() );
  EXPECT_EQ( "/", p.root_path().native() );
  EXPECT_EQ( native("foo/bar/baa.txt"), p.relative_path().native() );
  EXPECT_EQ( native("/foo/bar"), p.parent_path().native() );
  EXPECT_EQ( "baa.txt", p.filename().native() );
  EXPECT_EQ( "baa", p.stem().native() );
  EXPECT_EQ( ".txt", p.extension().native() );

  EXPECT_TRUE( p.has_root_directory() );
  EXPECT_TRUE( p.has_root_path() );
  EXPECT_TRUE( p.has_relative_path() );
  EXPECT_TRUE( p.has_parent_path() );
}

TEST_F( FileInterfaceTest, FilesystemPathNormalization )
{
  using namespace nom::filesystem;

  // ".." components must be collapsed. Relative paths preserve leading ".."
  // segments; absolute paths cannot escape the root.

  EXPECT_EQ( "foo", ( path("foo/bar") / ".." ).native() );
  EXPECT_EQ( "foo", path("foo/bar/..").filename().native() );
  EXPECT_EQ( "", path("foo/bar/..").parent_path().native() );
  EXPECT_EQ( "../baz", ( path("../bar") / ".." / "baz" ).native() );

  EXPECT_EQ( "/foo", ( path("/foo/bar") / ".." ).native() );
  EXPECT_EQ( "/", ( path("/foo") / ".." ).native() );
  EXPECT_EQ( "/", ( path("/") / ".." ).native() );
  EXPECT_EQ( "/data", ( path("/etc") / ".." / ".." / "data" ).native() );

  // "." components should be discarded.
  EXPECT_EQ( "foo/bar", path("foo/././bar").native() );
  EXPECT_EQ( "foo/bar", ( path("foo") / "." / "bar" ).native() );
}

TEST_F( FileInterfaceTest, FilesystemDirectoryIteration )
{
  using namespace nom::filesystem;

  // This test mirrors the Boost.Filesystem tut3.cpp directory-iteration
  // exercise: open a directory, collect its entries, sort them, and verify
  // each entry's filename.

  const path dir("/tmp/nomlib_dir_iter_test");
  const path file1( dir / "alpha.txt" );
  const path file2( dir / "beta.txt" );

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(file1) ) remove(file1);
  if( exists(file2) ) remove(file2);
  if( exists(dir) ) remove(dir);

  ASSERT_FALSE( exists(dir) );

  // Prepare a clean directory with two known files.
  EXPECT_TRUE( create_directories(dir) );
  ASSERT_TRUE( exists(dir) );
  ASSERT_TRUE( is_directory(dir) );

  {
    std::ofstream out1 = open_output_stream( file1 );
    ASSERT_TRUE( out1.is_open() );
    std::ofstream out2 = open_output_stream( file2 );
    ASSERT_TRUE( out2.is_open() );
  }

  EXPECT_TRUE( is_regular_file(file1) );
  EXPECT_TRUE( is_regular_file(file2) );

  std::vector<path> entries;
  for( const auto& entry : directory_iterator(dir) )
  {
    entries.push_back( entry.path() );
  }

  std::sort( entries.begin(), entries.end(), []( const path& a, const path& b ) {
    return a.filename().native() < b.filename().native();
  });

  ASSERT_EQ( 2u, entries.size() );
  EXPECT_EQ( "alpha.txt", entries[0].filename().native() );
  EXPECT_EQ( "beta.txt", entries[1].filename().native() );

  EXPECT_TRUE( entries[0].has_filename() );
  EXPECT_TRUE( entries[1].has_filename() );
  EXPECT_TRUE( exists(entries[0]) );
  EXPECT_TRUE( exists(entries[1]) );

  EXPECT_TRUE( remove(file1) );
  EXPECT_TRUE( remove(file2) );
  EXPECT_TRUE( remove(dir) );
  EXPECT_FALSE( exists(dir) );
}

TEST_F( FileInterfaceTest, FilesystemDirectoryIterationSortedByFilename )
{
  using namespace nom::filesystem;

  // This test mirrors the Boost.Filesystem tut4.cpp exercise: collect entries
  // from a directory iterator, sort them, and inspect only the filename part.

  const path dir("/tmp/nomlib_dir_iter_tut4_test");
  const path file1( dir / "zebra.txt" );
  const path file2( dir / "apple.txt" );
  const path file3( dir / "mango.txt" );

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(file1) ) remove(file1);
  if( exists(file2) ) remove(file2);
  if( exists(file3) ) remove(file3);
  if( exists(dir) ) remove(dir);

  ASSERT_FALSE( exists(dir) );

  EXPECT_TRUE( create_directories(dir) );
  ASSERT_TRUE( exists(dir) );

  {
    std::ofstream out1 = open_output_stream( file1 );
    ASSERT_TRUE( out1.is_open() );
    std::ofstream out2 = open_output_stream( file2 );
    ASSERT_TRUE( out2.is_open() );
    std::ofstream out3 = open_output_stream( file3 );
    ASSERT_TRUE( out3.is_open() );
  }

  std::vector<path> entries;
  for( const auto& entry : directory_iterator(dir) )
  {
    entries.push_back( entry.path().filename() );
  }

  std::sort( entries.begin(), entries.end(), []( const path& a, const path& b ) {
    return a.native() < b.native();
  });

  ASSERT_EQ( 3u, entries.size() );
  EXPECT_EQ( "apple.txt", entries[0].native() );
  EXPECT_EQ( "mango.txt", entries[1].native() );
  EXPECT_EQ( "zebra.txt", entries[2].native() );

  EXPECT_TRUE( remove(file1) );
  EXPECT_TRUE( remove(file2) );
  EXPECT_TRUE( remove(file3) );
  EXPECT_TRUE( remove(dir) );
  EXPECT_FALSE( exists(dir) );
}

TEST_F( FileInterfaceTest, FilesystemDirectoryIteratorEmptyDirectory )
{
  using namespace nom::filesystem;

  const path dir("/tmp/nomlib_empty_dir_iter_test");

  // Defensive cleanup in case a prior run left the path behind.
  if( exists(dir) )
  {
    remove(dir);
  }

  ASSERT_FALSE( exists(dir) );

  EXPECT_TRUE( create_directory(dir) );
  ASSERT_TRUE( exists(dir) );

  std::size_t count = 0;
  for( const auto& entry : directory_iterator(dir) )
  {
    ++count;
    (void)entry;
  }

  EXPECT_EQ( 0u, count );
  EXPECT_TRUE( remove(dir) );
}

TEST_F( FileInterfaceTest, FilesystemDirectoryIteratorMissingDirectory )
{
  using namespace nom::filesystem;

  const path dir("/tmp/nomlib_missing_dir_iter_test");
  if( exists(dir) )
  {
    EXPECT_TRUE( remove(dir) );
  }

  std::size_t count = 0;
  for( const auto& entry : directory_iterator(dir) )
  {
    ++count;
    (void)entry;
  }

  EXPECT_EQ( 0u, count );
}

TEST_F( FileInterfaceTest, FilesystemPathUnicodeConstructors )
{
  using namespace nom::filesystem;

  // This test mirrors the Boost.Filesystem tut5.cpp exercise: path should
  // accept narrow C-strings, narrow strings, wide C-strings, wide strings,
  // and character ranges in either encoding. The expected filesystem result
  // (on POSIX) is a set of files named:
  //   smile smile☺ smile2 smile2☺ smile3 smile3☺ smile4 smile4☺

  const char narrow_cstr[] = "smile";
  const wchar_t wide_cstr[] = L"smile\u263A";
  std::string narrow_string("smile2");
  std::wstring wide_string(L"smile2\u263A");

  std::list<char> narrow_list { 's', 'm', 'i', 'l', 'e', '3' };
  std::list<wchar_t> wide_list { L's', L'm', L'i', L'l', L'e', L'3', L'\u263A' };

  // Build paths from every supported source type.
  path p1(narrow_cstr);
  path p2(wide_cstr);
  path p3(narrow_string);
  path p4(wide_string);
  path p5(std::string(narrow_list.begin(), narrow_list.end()));
  path p6(std::wstring(wide_list.begin(), wide_list.end()));

  // Mutate the ranges to produce the fourth pair of files.
  narrow_list.pop_back();
  narrow_list.push_back('4');
  wide_list.pop_back();
  wide_list.pop_back();
  wide_list.push_back(L'4');
  wide_list.push_back(L'\u263A');

  path p7(std::string(narrow_list.begin(), narrow_list.end()));
  path p8(std::wstring(wide_list.begin(), wide_list.end()));

  // Make sure we are starting clean.
  for( const auto& p : { p1, p2, p3, p4, p5, p6, p7, p8 } )
  {
    if( exists(p) )
    {
      EXPECT_TRUE( remove(p) )
      << "Pre-existing file should be removable: " << p.native();
    }
  }

  // Create the files using the platform-native path encoding.
  {
    std::ofstream f = open_output_stream( p1 );
    ASSERT_TRUE( f.is_open() ) << p1.native();
  }
  {
    std::ofstream f = open_output_stream( p2 );
    ASSERT_TRUE( f.is_open() ) << p2.native();
  }
  {
    std::ofstream f = open_output_stream( p3 );
    ASSERT_TRUE( f.is_open() ) << p3.native();
  }
  {
    std::ofstream f = open_output_stream( p4 );
    ASSERT_TRUE( f.is_open() ) << p4.native();
  }
  {
    std::ofstream f = open_output_stream( p5 );
    ASSERT_TRUE( f.is_open() ) << p5.native();
  }
  {
    std::ofstream f = open_output_stream( p6 );
    ASSERT_TRUE( f.is_open() ) << p6.native();
  }
  {
    std::ofstream f = open_output_stream( p7 );
    ASSERT_TRUE( f.is_open() ) << p7.native();
  }
  {
    std::ofstream f = open_output_stream( p8 );
    ASSERT_TRUE( f.is_open() ) << p8.native();
  }

  // Verify every path represents a regular file on disk.
  EXPECT_TRUE( exists(p1) );
  EXPECT_TRUE( is_regular_file(p1) );
  EXPECT_TRUE( exists(p2) );
  EXPECT_TRUE( is_regular_file(p2) );
  EXPECT_TRUE( exists(p3) );
  EXPECT_TRUE( is_regular_file(p3) );
  EXPECT_TRUE( exists(p4) );
  EXPECT_TRUE( is_regular_file(p4) );
  EXPECT_TRUE( exists(p5) );
  EXPECT_TRUE( is_regular_file(p5) );
  EXPECT_TRUE( exists(p6) );
  EXPECT_TRUE( is_regular_file(p6) );
  EXPECT_TRUE( exists(p7) );
  EXPECT_TRUE( is_regular_file(p7) );
  EXPECT_TRUE( exists(p8) );
  EXPECT_TRUE( is_regular_file(p8) );

  // Spot-check preservation of the UTF-8 filename components.
  EXPECT_EQ( "smile", p1.filename().native() );
  EXPECT_EQ( "smile2", p3.filename().native() );
  EXPECT_EQ( "smile3", p5.filename().native() );
  EXPECT_EQ( "smile4", p7.filename().native() );

  EXPECT_EQ( std::wstring(L"smile\u263A"), p2.wstring() );
  EXPECT_EQ( std::wstring(L"smile2\u263A"), p4.wstring() );
  EXPECT_EQ( std::wstring(L"smile3\u263A"), p6.wstring() );
  EXPECT_EQ( std::wstring(L"smile4\u263A"), p8.wstring() );

  // Clean up.
  for( const auto& p : { p1, p2, p3, p4, p5, p6, p7, p8 } )
  {
    EXPECT_TRUE( remove(p) )
    << "Could not remove test file: " << p.native();
    EXPECT_FALSE( exists(p) );
  }
}

TEST_F( FileInterfaceTest, FilesystemErrorSinglePath )
{
  using filesystem::path;

  path p1( this->sys_temp.prepend( "missing_file.txt" ) );
  std::error_code ec( ENOENT, std::system_category() );

  filesystem::filesystem_error err( "could not access path", p1, ec );

  EXPECT_EQ( ec, err.code() );
  EXPECT_EQ( p1.native(), err.path1().native() );
  EXPECT_TRUE( err.path2().empty() );

  std::string msg( err.what() );
  EXPECT_NE( std::string::npos, msg.find( "could not access path" ) );
  EXPECT_NE( std::string::npos, msg.find( p1.native() ) );
  EXPECT_NE( std::string::npos, msg.find( ec.message() ) );

  // Must be usable through the standard interface.
  const std::system_error& base = err;
  EXPECT_EQ( ec, base.code() );
}

TEST_F( FileInterfaceTest, FilesystemErrorTwoPaths )
{
  using filesystem::path;

  path src( this->sys_temp.prepend( "source.txt" ) );
  path dst( this->sys_temp.prepend( "destination.txt" ) );
  std::error_code ec( EXDEV, std::system_category() );

  filesystem::filesystem_error err( "could not rename across devices", src, dst, ec );

  EXPECT_EQ( ec, err.code() );
  EXPECT_EQ( src.native(), err.path1().native() );
  EXPECT_EQ( dst.native(), err.path2().native() );

  std::string msg( err.what() );
  EXPECT_NE( std::string::npos, msg.find( "could not rename across devices" ) );
  EXPECT_NE( std::string::npos, msg.find( src.native() ) );
  EXPECT_NE( std::string::npos, msg.find( dst.native() ) );
  EXPECT_NE( std::string::npos, msg.find( ec.message() ) );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
