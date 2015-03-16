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
#include "nomlib/system/File.hpp"

// Forward declarations
#include "nomlib/system/IFile.hpp"

#if defined(NOM_PLATFORM_OSX) || defined(NOM_PLATFORM_LINUX)
  #include "nomlib/system/unix/UnixFile.hpp"
#elif defined(NOM_PLATFORM_WINDOWS)
  #include "nomlib/system/windows/WinFile.hpp"
#else
  #pragma message("The file interface must be implemented for this platform")
#endif

namespace nom {

File::File()
{
#if defined (NOM_PLATFORM_WINDOWS) // Use Windows APIs
  this->file = std::unique_ptr<IFile> ( new WinFile() );
#else // Assume POSIX compatibility; use POSIX / BSD & GNU APIs
  this->file = std::unique_ptr<IFile> ( new UnixFile() );
#endif
}

File::~File()
{
  //
}

std::string File::extension(const std::string& file) const
{
  return this->file->extension(file);
}

int32 File::size(const std::string& file_path) const
{
  return this->file->size(file_path);
}

bool File::is_dir(const std::string& file_path) const
{
  return this->file->is_dir(file_path);
}

bool File::is_file(const std::string& file_path) const
{
  return this->file->is_file(file_path);
}

bool File::exists(const std::string& file_path) const
{
  return this->file->exists(file_path);
}

std::string File::path(const std::string& dir_path) const
{
  return this->file->path(dir_path);
}

std::string File::currentPath() const
{
  return this->file->currentPath();
}

bool File::set_path (const std::string& path) const
{
  return this->file->set_path(path);
}

std::string File::basename(const std::string& filename) const
{
  return this->file->basename(filename);
}

std::vector<std::string> File::read_dir(const std::string& dir_path) const
{
  return this->file->read_dir(dir_path);
}

std::string File::resource_path(const std::string& identifier) const
{
  return this->file->resource_path(identifier);
}

std::string File::user_documents_path() const
{
  return this->file->user_documents_path();
}

std::string File::user_app_support_path() const
{
  return this->file->user_app_support_path();
}

std::string File::user_home_path() const
{
  return this->file->user_home_path();
}

std::string File::system_path() const
{
  return this->file->system_path();
}

bool File::mkdir(const std::string& path) const
{
  return this->file->mkdir(path);
}

bool File::recursive_mkdir(const std::string& path) const
{
  return this->file->recursive_mkdir(path);
}

bool File::rmdir(const std::string& path) const
{
  return this->file->rmdir(path);
}

bool File::mkfile(const std::string& path) const
{
  return this->file->mkfile(path);
}

std::string File::env(const std::string& path) const
{
  return this->file->env(path);
}

nom::size_type File::num_files(const std::string& path) const
{
  return this->file->num_files(path);
}

namespace priv {

// Static initializations
std::string file_root_ = "\0";

} // namespace priv

std::string file_root()
{
  return priv::file_root_;
}

void set_file_root(const std::string& root)
{
  priv::file_root_ = root;
}

} // namespace nom
