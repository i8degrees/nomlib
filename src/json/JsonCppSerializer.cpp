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
#include "nomlib/json/JsonCppSerializer.hpp"

namespace nom {

JsonCppSerializer::JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

JsonCppSerializer::~JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

bool JsonCppSerializer::serialize(  const JsonCppValue& source,
                                    const std::string& output ) const
{
  std::ofstream fp;
  Json::StyledStreamWriter writer( JSONCPP_INDENTION_LEVEL );

  fp.open( output );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Unable to save JSON output file: " + output );
    fp.close();
    return false;
  }

  writer.write( fp, source.get() );

  fp.close();

  return true;
}

bool JsonCppSerializer::unserialize(  const std::string& input,
                                      JsonCppValue& dest ) const
{
  std::ifstream fp;
  Json::Reader parser;

  fp.open( input );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Could not open given file: " + input );
    fp.close();
    return false;
  }

  if ( parser.parse( fp, dest.get() ) == false )
  {
    NOM_LOG_ERR ( NOM, "Unable to parse input JSON file: " + input );
    fp.close();
    return false;
  }

  fp.close();
  return true;
}

const std::string JsonCppSerializer::stringify( const JsonCppValue& obj ) const
{
  std::stringstream os;

  os << obj.get();

  return os.str();
}

} // namespace nom
