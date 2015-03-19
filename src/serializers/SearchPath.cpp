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
#include "nomlib/serializers/SearchPath.hpp"

#include "nomlib/system/File.hpp"
#include "nomlib/ptree/Value.hpp"
#include "nomlib/serializers/JsonCppDeserializer.hpp"

// Forward declarations
#include "nomlib/serializers/IValueDeserializer.hpp"

namespace nom {

SearchPath::SearchPath() :
  fp_{ nullptr }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_SYSTEM, nom::NOM_LOG_PRIORITY_VERBOSE );
}

SearchPath::~SearchPath()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_SYSTEM, nom::NOM_LOG_PRIORITY_VERBOSE );
}

const std::string& SearchPath::path()
{
  return this->path_;
}

bool SearchPath::load_file( const std::string& filename, const std::string& node )
{
  File dir;

  Value obj;
  Value prefixes;
  std::string path;

  // Use JSON parser as the default
  if( this->fp_ == nullptr )
  {
    this->fp_.reset( new JsonCppDeserializer() );
  }

  if( this->fp_->load( filename, obj ) == false )
  {
    return false;
  }

  // Sanity check for well-formed input

  if( obj[node].null_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Top-level object is not defined for the search path." );
    return false;
  }

  if( ! obj[node].object_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Top-level node must be an object type." );
    return false;
  }

  if( obj[node]["path"].null_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Path is not defined for", node );
    return false;
  }

  if( ! obj[node]["path"].string_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Path must be a string type for", node );
    return false;
  }

  if( obj[node]["search_prefix"].null_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Search prefix is not defined for", node );
    return false;
  }

  if( ! obj[node]["search_prefix"].array_type() )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Search prefix must be an array type for", node );
    return false;
  }

  // Commence the formation of the path

  path = obj[node]["path"].get_string();
  prefixes = obj[node]["search_prefix"];
  // NOM_DUMP( obj[node]["path"].get_string() );

  // Begin by forming the list of possible locations that the directory path
  // may be at
  for( auto itr = prefixes.begin(); itr != prefixes.end(); ++itr )
  {
    // NOM_DUMP( (*itr).get_string() );
    this->search_prefix_.push_back( (*itr).get_string() + path );
  }

  // Check each of the locations until we find one that exists
  for( auto itr = this->search_prefix_.begin(); itr != this->search_prefix_.end(); ++itr )
  {
    if( dir.exists( *itr ) )
    {
      this->path_ = *itr;
      break;
    }
    else
    {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_SYSTEM, "Not using non-existent search path:", *itr );
    }
  }

  // Check the final path and handle non-existent case
  if( ! dir.exists( this->path_ ) )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not find test resources at any of the defined search paths:" );

    // Debug aid
    for( auto itr = this->search_prefix_.begin(); itr != this->search_prefix_.end(); ++itr )
    {
      NOM_LOG_INFO( NOM_LOG_CATEGORY_SYSTEM, *itr );
    }

    return false;
  }
  else
  {
    // Success!
    NOM_LOG_INFO( NOM_LOG_CATEGORY_SYSTEM, "Using resources from:", this->path_ );
  }

  return true;
}

void SearchPath::set_deserializer( std::unique_ptr<IValueDeserializer> fp )
{
  this->fp_ = std::move( fp );
}

} // namespace nom
