/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/core/strings.hpp"

// Private headers (third-party)
#include <cstring>
#include <ctype.h>
#include <inttypes.h>

// Private headers (internal)
#include "nomlib/core/err.hpp"
#include "nomlib/math/math_helpers.hpp"

namespace nom {

const char*
create_string(const char* str, nom::size_type num_bytes)
{
  auto str_length = num_bytes;

  // NOTE(jeff): Make sure we have room for null-termination
  if(str_length >= MAX_STRING_LENGTH) {
    str_length = (MAX_STRING_LENGTH - 1);
  }

  char* result = NOM_SCAST(char*, malloc(str_length + 1));

  if(result == nullptr) {
    nom::set_error("Failed to allocate memory for string; NULL result");
    return result;
  }

  if(str != nullptr) {
    std::memcpy(result, str, str_length);
  }

  // NOTE(jeff): Null terminate the resulting string
  result[str_length] = '\0';

  return result;
}

const char*
create_string(const char* str)
{
  nom::size_type str_length = 0;
  const char* result = nullptr;

  if(str == nullptr) {
    nom::set_error("Failed to allocate memory for string; passed NULL string");
    return result;
  }

  str_length = nom::string_length(str);

  result = nom::create_string(str, str_length);

  return result;
}

int string_to_int(const char* str, int input_base)
{
  int result = 0;

  if(str != nullptr) {
    result = strtoimax(str, nullptr, input_base);
  } else {
    result = 0;
  }

  return result;
}

int string_to_int(const char* str)
{
  int result = 0;
  int input_base = 0;

  if(str != nullptr) {
    result = strtoimax(str, nullptr, input_base);
  } else {
    result = 0;
  }

  return result;
}

uint string_to_uint(const char* str, int input_base)
{
  int result = 0;

  if(str != nullptr) {
    result = strtoumax(str, nullptr, input_base);
  } else {
    result = 0;
  }

  return result;
}

uint string_to_uint(const char* str)
{
  int result = 0;
  int input_base = 0;

  if(str != nullptr) {
    result = strtoumax(str, nullptr, input_base);
  } else {
    result = 0;
  }

  return result;
}

const char* integer_to_cstr(int number)
{
  char* result = nullptr;

  sprintf(result, "%d", number);

  return result;
}

std::string integer_to_string(int number)
{
  auto result = std::to_string(number);

  return result;
}

nom::size_type string_length(const char* str)
{
  return strlen(str);
}

nom::size_type string_length(const std::string& str)
{
  return str.length();
}

int
compare_cstr_insensitive(const char* str1, const char* str2,
                         nom::size_type len)
{
  auto str1_tolower = nom::to_lowercase_cstr(str1);
  auto str2_tolower = nom::to_lowercase_cstr(str2);

  auto result = strncmp(str1_tolower, str2_tolower, len);

  nom::free_string(str1_tolower);
  nom::free_string(str2_tolower);

  return result;
}

int compare_cstr_insensitive(const char* str1, const char* str2)
{
  auto str1_len = nom::string_length(str1);
  auto str2_len = nom::string_length(str2);
  const auto STR_LENGTH = nom::clamp_min(str1_len, str2_len);

  auto result = nom::compare_cstr_insensitive(str1, str2, STR_LENGTH);

  return result;
}

int
compare_cstr_sensitive(const char* str1, const char* str2,
                       nom::size_type len)
{
  auto result = strncmp(str1, str2, len);

  return result;
}

int compare_cstr_sensitive(const char* str1, const char* str2)
{
  auto str1_len = nom::string_length(str1);
  auto str2_len = nom::string_length(str2);
  const auto STR_LENGTH = nom::clamp_min(str1_len, str2_len);

  auto result = nom::compare_cstr_sensitive(str1, str2, STR_LENGTH);

  return result;
}

int
compare_string_insensitive(const std::string& str1, const std::string& str2,
                           nom::size_type len)
{
  auto str1_cstr = str1.c_str();
  auto str2_cstr = str2.c_str();

  auto result =
    nom::compare_cstr_insensitive(str1_cstr, str2_cstr, len);

  return result;
}

int compare_string_insensitive(const std::string& str1, const std::string& str2)
{
  auto str1_len = str1.length();
  auto str2_len = str2.length();
  const auto STR_LENGTH = nom::clamp_min(str1_len, str2_len);

  auto result =
    nom::compare_string_insensitive(str1, str2, STR_LENGTH);

  return result;
}

int
compare_string_sensitive(const std::string& str1, const std::string& str2,
                         nom::size_type len)
{
  auto result = str1.compare(0, len, str2);

  return result;
}

int compare_string_sensitive(const std::string& str1, const std::string& str2)
{
  auto str1_len = str1.length();
  auto str2_len = str2.length();
  const auto STR_LENGTH = nom::clamp_min(str1_len, str2_len);

  auto result = nom::compare_string_sensitive(str1, str2, STR_LENGTH);

  return result;
}

void copy_string(const char* source, char* dest)
{
  std::strcpy(dest, source);
}

const char* duplicate_string(const char* str, nom::size_type length)
{
  // Buffer overflow protection
  if(length >= MAX_STRING_LENGTH) {
    // NOTE(jeff): One extra character is needed for a NULL-terminated string
    length = MAX_STRING_LENGTH - 1;
  }

  char* duplicate_string = NOM_SCAST(char*, malloc(length + 1));
  std::memcpy(duplicate_string, str, length);

  // NOTE(jeff): Add null-termination of the new string
  duplicate_string[length] = '\0';

  return duplicate_string;
}

const char* duplicate_string(const std::string& str, nom::size_type length)
{
  return nom::duplicate_string(str.c_str(), length);
}

void free_string(const char* ptr)
{
  auto str = NOM_CCAST(char*, ptr);
  std::free(str);
}

const char* to_lowercase_cstr(const char* str, nom::size_type len)
{
  auto result = NOM_CCAST(char*, nom::create_string(str));

  uint32 current_char = 0;
  for(uint32 char_pos = 0; char_pos != len; ++char_pos) {
    current_char = result[char_pos];
    result[char_pos] = tolower(current_char);
  }

  return result;
}

const char* to_lowercase_cstr(const char* str)
{
  auto str_len = nom::string_length(str);
  auto result =
    NOM_CCAST(char*, nom::to_lowercase_cstr(str, str_len));

  return result;
}

std::string to_lowercase_string(const std::string& str, nom::size_type len)
{
  auto result = str;

  uint32 current_char = 0;
  for(uint32 char_pos = 0; char_pos != len; ++char_pos) {
    current_char = result[char_pos];
    result[char_pos] = tolower(current_char);
  }

  return result;
}

std::string to_lowercase_string(const std::string& str)
{
  auto str_len = str.length();
  auto result = nom::to_lowercase_string(str, str_len);

  return result;
}

const char* to_uppercase_cstr(const char* str, nom::size_type len)
{
  auto result = NOM_CCAST(char*, nom::create_string(str));

  uint32 current_char = 0;
  for(uint32 char_pos = 0; char_pos != len; ++char_pos) {
    current_char = result[char_pos];
    result[char_pos] = toupper(current_char);
  }

  return result;
}

const char* to_uppercase_cstr(const char* str)
{
  auto str_len = nom::string_length(str);
  auto result = nom::to_uppercase_cstr(str, str_len);

  return result;
}

std::string to_uppercase_string(const std::string& str, nom::size_type len)
{
  auto result = str;

  uint32 current_char = 0;
  for(uint32 char_pos = 0; char_pos != len; ++char_pos) {
    current_char = result[char_pos];
    result[char_pos] = tolower(current_char);
  }

  return result;
}

std::string to_uppercase_string(const std::string& str)
{
  auto str_len = str.length();
  auto result = nom::to_uppercase_string(str, str_len);

  return result;
}

} // namespace nom
