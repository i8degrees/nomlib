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
#include "nomlib/system/unix/UnixFile.hpp"

#include "nomlib/core/strings.hpp"
// Private headers (third-party libs)
#include <unistd.h>
//#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

#if defined( NOM_PLATFORM_OSX )
  #include <CoreServices/CoreServices.h>
  #include <CoreFoundation/CoreFoundation.h>
#endif

#if defined (NOM_PLATFORM_LINUX )
  //#include <cstudio>
#endif

// Private headers
#include "nomlib/system/Path.hpp"
#include "nomlib/system/filesystem.hpp"

namespace nom {

UnixFile::UnixFile()
{
  // NOM_LOG_TRACE( NOM );
}

UnixFile::~UnixFile()
{
  // NOM_LOG_TRACE( NOM );
}

const std::string UnixFile::extension(const std::string& file)
{
  filesystem::path p( file );
  std::string ext = p.extension().native();

  // nomlib's historical extension() returns the extension without the dot.
  if( ! ext.empty() && ext[0] == '.' )
  {
    ext.erase( 0, 1 );
  }

  return ext;
}

int32 UnixFile::size(const std::string& file_path)
{
  return static_cast<int32>( filesystem::file_size( file_path ) );
}

bool UnixFile::is_dir(const std::string& file_path)
{
  return filesystem::is_directory( file_path );
}

bool UnixFile::is_file(const std::string& file_path)
{
  return filesystem::is_regular_file( file_path );
}

bool UnixFile::exists(const std::string& file_path)
{
  return filesystem::exists( file_path );
}

const std::string UnixFile::path(const std::string& dir_path)
{
  filesystem::path p( dir_path );
  filesystem::path parent = p.parent_path();

  if( parent.empty() ) return ".";

  return parent.native();
}

std::string UnixFile::currentPath()
{
  return filesystem::current_path().native();
}

bool UnixFile::set_path(const std::string& path)
{
  if ( ::chdir( path.c_str() ) != 0 )
  {
    NOM_LOG_ERR( NOM, "Unknown error on attempt to change working directory to: " + path );
    return false;
  }

  return true;
}

const std::string UnixFile::basename(const std::string& filename)
{
  return filesystem::path( filename ).stem().native();
}

std::vector<std::string> UnixFile::read_dir(const std::string& dir_path)
{
  DIR *dp = nullptr;
  dirent *ep = nullptr;
  bool eof = false;               // Tracks the end of the directory
  std::string entry;              // A file entry within a directory
  std::vector<std::string> files; // Files found within directory

  dp = opendir( dir_path.c_str() );

  if( dp != nullptr )
  {
    while( eof == false )
    {
      ep = readdir( dp );

      if( ep != nullptr )
      {
        entry = ep->d_name;

        // TODO: Handle directory case
        // if( this->is_dir( dir_path + "/" + entry ) == true )
        // {
        // }

        // Sanitize the file entries
        if( entry != "." && entry != ".." )
        {
          files.push_back( entry );
        }
        // NOM_DUMP(entry);
      }
      else
      {
        eof = true;
      }
    }

    closedir( dp );
  }
  else
  {
    NOM_LOG_ERR( NOM, "Could not read directory path: " + dir_path );
  }

  return files;
}

const std::string UnixFile::resource_path(const std::string& identifier)
{
  NOM_ASSERT_INVALID_PATH("STUB: resource_path");
  return "\0";
}

const std::string UnixFile::user_documents_path()
{
  Path p;
  // missing
  auto HOME = this->env("XDG_DOCUMENTS_HOME");
  if(compare_string_insensitive(HOME, "") != 0) {
    HOME = this->env("HOME");
    return HOME + p.native() + "Documents";
  }

  return HOME;
}

const std::string UnixFile::user_app_support_path()
{
  Path p;
  auto HOME = this->env("XDG_STATE_HOME");
  if(nom::compare_string_insensitive(HOME, "") != 0) {
    HOME = this->env("HOME");
    return HOME + p.native() + ".config";
  }

  return HOME;
}

const std::string UnixFile::user_home_path()
{
  auto HOME = this->env("HOME");
  if(nom::compare_string_insensitive(HOME, "") != 0) {
    HOME = this->env("TMPDIR");
    // What more can we do here if this fails?
  }

  return HOME;
}

const std::string UnixFile::system_path()
{
  Path p;
  // missing
  auto SYSTEM = this->env("XDG_RUNTIME_DIR");
  if(nom::compare_string_insensitive(SYSTEM, "") != 0) {
    SYSTEM = p.native();
  }

  return SYSTEM;
}

const std::string UnixFile::system_temp_path()
{
  Path p;
  auto TMPDIR = this->env("TMPDIR");
  if(nom::compare_string_insensitive(TMPDIR, "") != 0) {
    TMPDIR = p.native() + "tmp";
  }

  return TMPDIR;
}

const std::string UnixFile::system_library_path()
{
  Path p("bin");
  return p.path();
}

const std::string UnixFile::system_fonts_path()
{
  Path p("/usr/share/fonts");
  return p.path();
}

bool UnixFile::mkdir(const std::string& path)
{
  return filesystem::create_directory( path );
}

bool UnixFile::recursive_mkdir(const std::string& path)
{
  return filesystem::create_directories( path );
}

bool UnixFile::rmdir(const std::string& path)
{
  if( ! filesystem::is_directory( path ) ) return false;
  return filesystem::remove( path );
}

bool UnixFile::mkfile(const std::string& path)
{
  std::ofstream fp;

  fp.open( path );

  if( fp.is_open() && fp.good() )
  {
    fp << "";

    // Success!
    return true;
  }

  // Err (not defined)
  return false;
}

std::string UnixFile::env(const std::string& path)
{
  char* value = nullptr;

  value = getenv( path.c_str() );

  if( value != nullptr )
  {
    return value;
  }

  // Err
  return "\0";
}

nom::size_type UnixFile::num_files(const std::string& path)
{
  std::vector<std::string> num_files_per_dir =
    this->read_dir(path);

  return num_files_per_dir.size();
}

} // namespace nom
