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

//bool FileReader::parse ( const std::string& filename, Value& value )
bool FileReader::load ( const std::string& filename, json_spirit::Value& value )
{
  std::ifstream fp; // input file stream

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    //json_spirit::read_stream ( fp, value.getValue() );
    if ( json_spirit::read_stream ( fp, value ) == false )
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON input file at: " + filename );
      fp.close();
      return false;
    }
    fp.close();
  }
  else
  {
    fp.close();
    return false;
  }

  return true;
}


  } // namespace JSON
} // namespace nom
