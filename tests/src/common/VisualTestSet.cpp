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
#include "nomlib/tests/common/VisualTestSet.hpp"

// Forward declarations
#include "nomlib/ptree/Value.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/serializers/IValueSerializer.hpp"

// Private headers
#include "nomlib/version.hpp"       // nom::GIT_REVISION
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

VisualTestSet::VisualTestSet( void )
{
  // NOM_LOG_TRACE( NOM );

  this->set_version( nom::revision() );

  // Should not be used; see documentation notes for why
  // this->set_timestamp( nom::timestamp() );
}

VisualTestSet::~VisualTestSet( void )
{
  // NOM_LOG_TRACE( NOM );
}

VisualTestSet::VisualTestSet  (
                                const std::string& set,
                                const std::string& name,
                                const std::string& ts,
                                const Size2i& res,
                                const std::string& dir_path
                              )
{
  // NOM_LOG_TRACE( NOM );

  this->set_test_set( set );
  this->set_test_name( name );
  this->set_timestamp( ts );
  this->set_resolution( res );
  this->set_directory( dir_path );

  // Auto-initialized variables
  this->set_version( nom::revision() );
}

bool VisualTestSet::operator ==( const self_type& rhs )
{
  if( this->resolution() != rhs.resolution() )
  {
    return false;
  }

  for( auto idx = 0; idx != this->images_.size(); ++idx )
  {
    if( this->images_[idx] != rhs.images_[idx] )
    {
      return false;
    }
  }

  return true;
}

bool VisualTestSet::operator !=( const self_type& rhs )
{
  return ! ( *this == rhs );
}

bool VisualTestSet::operator >( const self_type& rhs )
{
  return( this->timestamp() > rhs.timestamp() );
}

bool VisualTestSet::operator <( const self_type& rhs )
{
  return( rhs.timestamp() > this->timestamp() );
}

const std::vector<std::string>& VisualTestSet::images( void ) const
{
  return this->images_;
}

std::string VisualTestSet::image( int pos ) const
{
  NOM_ASSERT( pos < this->images().size() );

  return this->images()[ pos ];
}

const std::string& VisualTestSet::test_set( void ) const
{
  return this->test_set_;
}

const std::string& VisualTestSet::test_name( void ) const
{
  return this->test_name_;
}

const std::string& VisualTestSet::version( void ) const
{
  return this->version_;
}

const std::string& VisualTestSet::timestamp( void ) const
{
  return this->timestamp_;
}

const std::string& VisualTestSet::comment( void ) const
{
  return this->comment_;
}

const Size2i& VisualTestSet::resolution( void ) const
{
  return this->resolution_;
}

const std::string& VisualTestSet::directory( void ) const
{
  return this->directory_;
}

void VisualTestSet::set_test_set( const std::string& set )
{
  this->test_set_ = set;
}

void VisualTestSet::set_test_name( const std::string& name )
{
  this->test_name_ = name;
}

void VisualTestSet::set_version( const std::string& ver )
{
  this->version_ = ver;
}

void VisualTestSet::set_timestamp( const std::string& ts )
{
  this->timestamp_ = ts;
}

void VisualTestSet::set_comment( const std::string& comment )
{
  this->comment_ = comment;
}

void VisualTestSet::set_resolution( const Size2i& res )
{
  this->resolution_ = res;
}

void VisualTestSet::set_directory( const std::string& path )
{
  this->directory_ = path;
}

void VisualTestSet::append_image( const std::string& filename )
{
  this->images_.push_back( filename );
}

bool VisualTestSet::save_file( IValueSerializer* cfg )
{
  Path p;
  Value os;

  os["batch"]["test_set"] = this->test_set();
  os["batch"]["test_name"] = this->test_name();
  os["batch"]["version"] = this->version();
  os["batch"]["timestamp"] = this->timestamp();
  os["batch"]["comment"] = this->comment();
  os["batch"]["resolution"]["width"] = this->resolution().w;
  os["batch"]["resolution"]["height"] = this->resolution().h;

  for( auto idx = 0; idx != this->images().size(); ++idx )
  {
    os["batch"]["images"][idx] = this->image( idx );
  }

  return cfg->save( os, this->directory() + p.native() + "info.json" );
}

bool VisualTestSet::load_file( IValueDeserializer* cfg )
{
  Path p;
  Value info;

  if( cfg->load( this->directory() + p.native() + "info.json", info ) == false )
  {
    return false;
  }

  this->set_test_set( info["batch"]["test_set"].get_string() );
  this->set_test_name( info["batch"]["test_name"].get_string() );
  this->set_version( info["batch"]["version"].get_string() );
  this->set_timestamp( info["batch"]["timestamp"].get_string() );
  this->set_comment( info["batch"]["comment"].get_string() );
  this->set_resolution( Size2i( info["batch"]["resolution"]["width"].get_int(), info["batch"]["resolution"]["height"].get_int() ) );

  for( auto idx = 0; idx != info["batch"]["images"].size(); ++idx )
  {
    this->append_image( info["batch"]["images"][idx].get_string() );
  }

  return true;
}

} // namespace nom
