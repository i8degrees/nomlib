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
#include "nomlib/json/FileReader.hpp"

namespace nom {
  namespace JSON {

FileReader::FileReader ( void ) {}
FileReader::~FileReader ( void ) {}

bool FileReader::load (
                        const std::string& filename,
                        Json::Value& object,
                        bool parse_comments
                      )
{
  std::ifstream fp; // input file stream
  Json::Reader parser; // jsoncpp library

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    // disable comments parsing
    if ( parser.parse ( fp, object, parse_comments ) == false )
    {
NOM_LOG_ERR ( NOM, "Unable to parse input JSON file at: " + filename );
      fp.close();
      return false;
    }
  }
  else
  {
NOM_LOG_ERR ( NOM, "Could not open given file: " + filename );
    fp.close();
    return false;
  }

  fp.close();
  return true;
}

void FileReader::dump_key ( const Json::Value& key ) const
{
  switch ( key.type() )
  {
    // Unkonwn type; let jsoncpp try figuring it out
    default: printf ( " [type=%d, size=%d]", key.type(), key.size() ); break;

    case 0: // NULL
    {
      printf ( " [type=nullValue, size=%d]", key.size() );
      break;
    }

    case 1: // JSON signed integer
    {
      printf ( " [type=intValue, size=%lu]", sizeof ( key.asInt() ) );
      break;
    }

    case 2: // JSON unsigned integer
    {
      printf ( " [type=uintValue, size=%lu]", sizeof ( key.asUInt() ) );
      break;
    }

    case 3: // JSON double
    {
      printf ( " [type=realValue, size=%lu]", sizeof ( key.asDouble() ) );
      break;
    }

    case 4: // JSON string
    {
      printf ( " [type=stringValue, size=%lu]", key.asString().length() );
      break;
    }

    case 5: // JSON boolean
    {
      printf ( " [type=booleanValue, size=%lu]", sizeof ( key.asBool() ) );
      break;
    }

    case 6: // JSON Array
    {
      printf ( " [type=arrayValue, size=%d]", key.size() );
      break;
    }

    case 7:
    {
      printf ( " [type=objectValue, size=%d]", key.size() );
      break;
    }
  } // switch key.type()
}

void FileReader::dump_value ( const Json::Value& value ) const
{
  if ( value.isString() )
  {
    printf ( "string (%s)", value.asString().c_str() );
  }
  else if ( value.isBool() )
  {
    printf ( "bool (%d)", value.asBool() );
  }
  else if ( value.isInt() )
  {
    printf ( "int (%d)", value.asInt() );
  }
  else if ( value.isUInt() )
  {
    printf ( "uint (%u)", value.asUInt() );
  }
  else if ( value.isDouble() )
  {
    printf ( "double (%f)", value.asDouble() );
  }
  else // Unkonwn type; let jsoncpp try figuring it out
  {
    printf ( "unknown type=%d", value.type() );
  }
}

bool FileReader::dump ( const Json::Value& root, int depth ) const
{
  depth += 1;

  this->dump_key ( root );

  if ( root.size() > 0 )
  {
    std::cout << std::endl;
    for ( auto itr = root.begin() ; itr != root.end() ; ++itr )
    {
      // Pretty print depth.
      for ( auto tab = 0 ; tab < depth; tab++ )
      {
        std::cout << "-";
      }

      std::cout << " subvalue (";
      this->dump_value ( itr.key() );
      std::cout << ") -";
      this->dump ( *itr, depth );
    }

    return true;
  }
  else
  {
    std::cout << " ";
    this->dump_value ( root );
    std::cout << std::endl;
  }

  return true;
}

  } // namespace JSON
} // namespace nom
