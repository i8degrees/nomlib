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
#include "nomlib/serializers/IConfigFile.hpp"

namespace nom {

IConfigFile::IConfigFile( void )
{
  NOM_LOG_TRACE( NOM );
}

IConfigFile::~IConfigFile( void )
{
  NOM_LOG_TRACE( NOM );
}

const std::string& IConfigFile::filename( void ) const
{
  return this->filename_;
}

bool IConfigFile::exists( const std::string& key ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return true;
  }

  return false;
}

int IConfigFile::get_int( const std::string& key, int default_value ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return res->second.get_int();
  }

  return default_value;
}

uint IConfigFile::get_uint( const std::string& key, uint default_value ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return res->second.get_uint();
  }

  return default_value;
}

double IConfigFile::get_double( const std::string& key, double default_value ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return res->second.get_double();
  }

  return default_value;
}

const std::string IConfigFile::get_string( const std::string& key, const std::string& default_value ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return res->second.get_string();
  }

  return default_value;
}

bool IConfigFile::get_bool( const std::string& key, bool default_value ) const
{
  auto res = this->config_.find( key );

  if( res != this->config_.end() )
  {
    return res->second.get_bool();
  }

  return default_value;
}

void IConfigFile::set_property( const std::string& key, const Value& value )
{
  if( this->exists( key ) )
  {
    this->erase( key );
  }

  Pair p( key, value );

  this->config_.insert( p );

  #if defined( NOM_DEBUG_SERIALIZERS_ICONFIG_FILE )
    if( value.int_type() )
    {
      NOM_LOG_INFO( NOM, "IConfigFile: " + key + ": " + std::to_string( value.get_int() ) + " has been added to the cache." );
    }
    else if( value.uint_type() )
    {
      NOM_LOG_INFO( NOM, "IConfigFile: " + key + ": " + std::to_string( value.get_uint() ) + " has been added to the cache." );
    }
    else if( value.double_type() )
    {
      NOM_LOG_INFO( NOM, "IConfigFile: " + key + ": " + std::to_string( value.get_double() ) + " has been added to the cache." );
    }
    else if( value.string_type() )
    {
      NOM_LOG_INFO( NOM, "IConfigFile: " + key + ": " + "\"" + value.get_string() + "\"" + " has been added to the cache." );
    }
    else if( value.bool_type() )
    {
      NOM_LOG_INFO( NOM, "IConfigFile: " + key + ": " + "\"" + std::to_string( value.get_bool() ) + "\"" + " has been added to the cache." );
    }
    else
    {
      // Either not implemented yet (array, object types), or unknown value
      // type.
      NOM_LOG_ERR( NOM, "IConfigFile: " + key + " could not be added to the cache." );
    }
  #endif
}

bool IConfigFile::erase( const std::string& key )
{
  this->config_.erase( key );

  return true;
}

void IConfigFile::set_filename( const std::string& filename )
{
  this->filename_ = filename;

  this->load();
}

// Private scope

const IConfigFile::value_type& IConfigFile::get( void ) const
{
  return this->config_;
}

} // namespace nom
