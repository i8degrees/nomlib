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
#include "nomlib/json/FileWriter.hpp"

namespace nom {
  namespace JSON {

FileWriter::FileWriter ( void ) {}
FileWriter::~FileWriter ( void ) {}

bool FileWriter::save (
                        const std::string& filename,
                        const Json::Value& object,
                        uint32 format
                      )
{
  std::ofstream fp; // output file stream
  Json::StyledStreamWriter writer ( "  " ); // jsoncpp library with two spaces
                                            // indention

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    writer.write ( fp, object );
/* FIXME
    switch ( format )
    {
      default:
      case NoFormatting:
      {

      }
      break;

      case PrettyPrint:
      {
        json_spirit::write_stream (
                                    json_spirit::Value ( root_object ), fp,
                                    //json_spirit::Value ( root.values ), fp,
                                    json_spirit::pretty_print
                                  );
      }
      break;

      case CompactArrays:
      {
        json_spirit::write_stream (
                                    json_spirit::Value ( root_object ), fp,
                                    //json_spirit::Value ( root.values ), fp,
                                    json_spirit::single_line_arrays
                                  );
      }
      break;
    }
*/
  }
  else // file error
  {
NOM_LOG_ERR ( NOM, "Unable to save JSON output file at: " + filename );
    fp.close();
    return false;
  } // end if fp is good

  fp.close();
  return true;
}

  } // namespace JSON
} // namespace nom
