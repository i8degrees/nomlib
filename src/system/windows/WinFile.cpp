/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/system/windows/WinFile.hpp"

namespace nom {

WinFile::WinFile ( void ) {}

WinFile::~WinFile ( void ) {}

const std::string WinFile::extension ( const std::string& file )
{
  std::string extension = "\0";

  if ( file.find_last_of ( "." ) != std::string::npos ) // match found
  {
    extension = file.substr ( file.find_last_of (".") + 1 );
  }

  // if we reach this point, return false (a null terminated string)
  return extension;
}

int32 WinFile::size ( const std::string& file_path )
{
  // Stub
    return 0;
}

bool WinFile::exists ( const std::string& file_path )
{
  // Stub
    return false;
}

const std::string WinFile::path ( const std::string& dir_path )
{
  Path p; // Just to be safe, we'll let nom::Path determine our path separator!

  uint32 pos = dir_path.find_last_of ( p.native(), PATH_MAX );

  // If no matches are found, this means the file path given is actually a base
  // file name path, without any directory path at all. 
  // See also 'man 3 basename'
  if ( pos == std::string::npos ) return ".";
  
  // A match was found -- return only the directory path leading up to the 
  // file name path, without a a trailing path separator.
  return dir_path.substr ( 0, pos );
}

const std::string WinFile::currentPath ( void )
{
  // Stub
  return "";
}

bool WinFile::set_path ( const std::string& path )
{
  if ( _chdir ( path.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Unknown error on attempt to change working directory to: " + path );
    return false;
  }

  return true;
}

const std::string WinFile::basename ( const std::string& filename )
{
  size_t pos = 0;
  pos = filename.find_last_of ( ".", PATH_MAX );

  // If no matches are found, return the input filename back to the end-user.
  if ( pos == std::string::npos ) return filename; // Failure

  // Bingo! A match was found -- return everything leading up to --
  // the last '.' character found (excluding the dot).
  return filename.substr ( 0, pos );
}

} // namespace nom
