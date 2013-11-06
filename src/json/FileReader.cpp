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

bool FileReader::load ( const std::string& filename, Json::Value& object )
{
  std::ifstream fp; // input file stream
  Json::Reader parser; // jsoncpp library

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    // disable comments parsing
    if ( parser.parse ( fp, object, false ) == false )
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


  } // namespace JSON
} // namespace nom
