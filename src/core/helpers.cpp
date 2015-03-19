
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
#include "nomlib/core/helpers.hpp"

namespace nom {

namespace priv {

nom::size_type string_length(const char* str)
{
  return strlen(str);
}

nom::size_type string_length(const std::string& str)
{
  return str.length();
}

int compare_string(const char* str1, const char* str2)
{
  return strcmp(str1, str2);
}

int compare_string(const std::string& str1, const std::string& str2)
{
  return str1.compare(str2);
}

const char* duplicate_string(const char* str, nom::size_type length)
{
  // Buffer overflow protection
  if( length >= priv::MAX_STRING_LENGTH )
  {
    length = priv::MAX_STRING_LENGTH - 1;
  }

  // Allocate memory for duplicating the C string
  char* duplicate_string = static_cast<char*> ( malloc( length + 1 ) );
  std::memcpy(duplicate_string, str, length);

  // Null-terminate
  duplicate_string[length] = 0;

  return duplicate_string;
}

const char* duplicate_string(const std::string& str, nom::size_type length)
{
  return priv::duplicate_string(str.c_str(), length);
}

void free_string(const char* ptr)
{
  std::free( NOM_CCAST(char*, ptr) );
}

} // namespace priv

} // namespace nom
