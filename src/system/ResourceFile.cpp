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
#include "nomlib/system/ResourceFile.hpp"

// Private headers
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

const ResourceFile ResourceFile::null;

ResourceFile::ResourceFile( void )
{
  // NOM_LOG_TRACE( NOM );

  this->set_type( ResourceFile::Type::Invalid );
  this->set_loaded( false );
}

ResourceFile::~ResourceFile( void )
{
  // NOM_LOG_TRACE( NOM );
}

ResourceFile::ResourceFile( const std::string& name )
{
  Path p;

  this->set_name( name );
  this->set_type( ResourceFile::Type::Invalid );
  this->set_loaded( false );
}

ResourceFile::ResourceFile( const std::string& name, const std::string& file_path )
{
  this->set_name( name );
  this->set_path( file_path );
  this->set_type( ResourceFile::Type::Invalid );
  this->set_loaded( false );
}

ResourceFile::ResourceFile( const std::string& name, const std::string& file_path, ResourceFile::Type type )
{
  this->set_name( name );
  this->set_path( file_path );
  this->set_type( type );
  this->set_loaded( false );
}

bool ResourceFile::operator <( const self_type& rhs ) const
{
  return( this->name() < rhs.name() );
}

bool ResourceFile::operator ==( const self_type& rhs ) const
{
  return(
          this->name() == rhs.name()  && this->path() == rhs.path()
                                      &&
          this->type() == rhs.type()  && this->loaded() == rhs.loaded()
        );
}

bool ResourceFile::exists( void ) const
{
  File fp;

  if( fp.exists( this->path() ) == true )
  {
    return true;
  }

  return false;
}

const std::string& ResourceFile::name( void ) const
{
  return this->name_;
}

const std::string& ResourceFile::path( void ) const
{
  return this->path_;
}

ResourceFile::Type ResourceFile::type( void ) const
{
  return this->type_;
}

bool ResourceFile::loaded( void ) const
{
  return this->loaded_;
}

void ResourceFile::set_name( const std::string& name )
{
  this->name_ = name;
}

void ResourceFile::set_path( const std::string& file_path )
{
  this->path_ = file_path;
}

void ResourceFile::set_type( ResourceFile::Type type )
{
  this->type_ = type;
}

void ResourceFile::set_loaded( bool state )
{
  this->loaded_ = state;
}

} // namespace nom
