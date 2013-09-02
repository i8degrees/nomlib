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
#ifndef NOM_JSON_FILEWRITER_HEADERS
#define NOM_JSON_FILEWRITER_HEADERS

#include <iostream>
#include <fstream>
#include <string>
#include <array>

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif

#include "json_spirit_writer_template.h"

#include "nomlib/config.hpp"
//#include "nomlib/json/Object.hpp"
//#include "nomlib/json/Array.hpp"

namespace nom {
  namespace JSON {

enum
{
  NoFormatting = 0, // no formatting applied
  PrettyPrint = 1, // json_spirit::pretty_print
  CompactArrays = 2 // json_spirit::single_line_arrays; implies PrettyPrint
};

class FileWriter
{
  public:
    FileWriter ( void );
    ~FileWriter ( void );
/*
    bool save (
                const std::string& filename, Object& root,
                enum options format = none
              );
*/
    bool save (
                const std::string& filename, const json_spirit::Array& root_object,
                uint32 format = NoFormatting
              );
};


  } // namespace JSON
} // namespace nom

#endif // include guard defined
