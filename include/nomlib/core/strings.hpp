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
#ifndef NOMLIB_CORE_STRINGS_HPP
#define NOMLIB_CORE_STRINGS_HPP

#include "export.hpp"
#include "nomlib/config.hpp"

#include <memory>
#include <string>

namespace nom {

/// \brief The maximum size, in bytes, a string may consume in memory.
///
/// \todo Additional buffer size validation -- underflows and overflows -- need
/// to be happening in these functions!
const nom::size_type MAX_STRING_LENGTH = 255;

/// \brief Allocate memory for a string.
///
/// \param str The string to copy in place of the allocated memory. A NULL
/// value is valid and will skip a memory copy of the existing string.
///
/// \param num_bytes The number of bytes to allocate for the string.
///
/// \returns On success, a NULL-terminated string will be returned. On failure
/// to allocate the string, such as insufficient memory, or passage of a NULL
/// string, will result in a NULL pointer being returned.
///
/// \remarks The string length must not exceed nom::MAX_STRING_LENGTH. A string
/// that exceeds this number will be clamped to the length of
/// nom::MAX_STRING_LENGTH.
NOM_EXPORT const char*
create_string(const char* str, nom::size_type num_bytes);

/// \brief Allocate memory for a string, using the existing string length as
/// the number of bytes to allocate.
///
/// \param str The string to copy in place of the allocated memory. A NULL
/// value is invalid.
///
/// \returns On success, a NULL-terminated string will be returned. On failure
/// to allocate the string, such as insufficient memory, or passage of a NULL
/// string, will result in a NULL pointer being returned.
///
/// \remarks The string length must not exceed nom::MAX_STRING_LENGTH. A string
/// that exceeds this number will be clamped to the length of
/// nom::MAX_STRING_LENGTH.
NOM_EXPORT const char*
create_string(const char* str);

/// \brief Interpret an integer value from a string.
///
/// \param str The string to convert to a signed integer. A NULL value is
/// invalid.
///
/// \param input_base The number base of the given string, i.e.: two (2) for
/// binary, eight (8) for octal, ten (10) for decimal, sixteen (16) for
/// hexadecimal. A value of zero (0) will result in an attempt to auto-detect
/// the number base of the string.
///
/// \returns On success, a signed integer representing the string's integer
/// value. On failure, such as passing a NULL string value, results in a return
/// of zero (0).
///
/// \remarks The following rules regarding the number base auto-detection are
/// in place:
///
/// A string prefix of zero (0) indicates octal base; this applies when the
/// input base is eight (8) or zero (0).
///
/// A string prefix of '0x' or '0X' indicates hexadecimal base -- this applies
/// when the input base is sixteen (16) or zero (0).
NOM_EXPORT int string_to_int(const char* str, int input_base);

/// \brief Interpret an integer value from a string.
///
/// \param str The string to convert to an unsigned integer. A NULL value is
/// invalid.
///
/// \param input_base The number base of the given string, i.e.: two (2) for
/// binary, eight (8) for octal, ten (10) for decimal, sixteen (16) for
/// hexadecimal. A value of zero (0) will result in an attempt to auto-detect
/// the number base of the string.
///
/// \returns On success, an unsigned integer representing the string's integer
/// value. On failure, such as passing a NULL string value, results in a return
/// of zero (0).
///
/// \remarks The following rules regarding the number base auto-detection are
/// in place:
///
/// A string prefix of zero (0) indicates octal base; this applies when the
/// input base is eight (8) or zero (0).
///
/// A string prefix of '0x' or '0X' indicates hexadecimal base -- this applies
/// when the input base is sixteen (16) or zero (0).
NOM_EXPORT uint string_to_uint(const char* str, int input_base);

/// \brief Interpret an integer value from a string.
///
/// \param str The string to convert to a signed integer. A NULL value is
/// invalid.
///
/// \returns On success, a signed integer representing the string's integer
/// value. On failure, such as passing a NULL string value, results in a return
/// of zero (0).
///
/// \remarks The following rules regarding the number base auto-detection are
/// in place:
///
/// A string prefix of zero (0) indicates octal base; this applies when the
/// input base is eight (8) or zero (0).
///
/// A string prefix of '0x' or '0X' indicates hexadecimal base -- this applies
/// when the input base is sixteen (16) or zero (0).
NOM_EXPORT int string_to_int(const char* str);

/// \brief Interpret an integer value from a string.
///
/// \param str The string to convert to an unsigned integer. A NULL value is
/// invalid.
///
/// \returns On success, an unsigned integer representing the string's integer
/// value. On failure, such as passing a NULL string value, results in a return
/// of zero (0).
///
/// \remarks The following rules regarding the number base auto-detection are
/// in place:
///
/// A string prefix of zero (0) indicates octal base; this applies when the
/// input base is eight (8) or zero (0).
///
/// A string prefix of '0x' or '0X' indicates hexadecimal base -- this applies
/// when the input base is sixteen (16) or zero (0).
NOM_EXPORT uint string_to_uint(const char* str);

NOM_EXPORT const char* integer_to_cstr(int number);
NOM_EXPORT std::string integer_to_string(int number);
NOM_EXPORT nom::size_type string_length(const char* str);
NOM_EXPORT
nom::size_type string_length(const std::string& str);

NOM_EXPORT
int compare_cstr_insensitive(const char* str1, const char* str2,
                            nom::size_type len);

NOM_EXPORT
int compare_cstr_insensitive(const char* str1, const char* str2);

NOM_EXPORT
int compare_cstr_sensitive(const char* str1, const char* str2,
                            nom::size_type len);

NOM_EXPORT int compare_cstr_sensitive(const char* str1, const char* str2);

NOM_EXPORT int
compare_string_insensitive(const std::string& str1, const std::string& str2,
                           nom::size_type len);
NOM_EXPORT int
compare_string_insensitive(const std::string& str1, const std::string& str2);

NOM_EXPORT
int compare_string_sensitive(const std::string& str1, const std::string& str2,
                             nom::size_type len);
NOM_EXPORT
int compare_string_sensitive(const std::string& str1, const std::string& str2);

NOM_EXPORT
void copy_string(const char* source, char* dest);

/// \brief Create a deep-copy instance a C style string.
///
/// \param length Size of the string to copy.
///
/// \returns Null-terminated string up to MAX_STRING_LENGTH.
///
/// \todo Remove this function; it is a duplicate of nom::create_string
NOM_DEPRECATED_EXPORT
const char* duplicate_string(const char* str, nom::size_type length);

NOM_EXPORT
const char* duplicate_string(const std::string& str, nom::size_type length);

NOM_EXPORT void free_string(const char* ptr);

NOM_EXPORT
const char* to_lowercase_cstr(const char* str, nom::size_type len);
NOM_EXPORT const char* to_lowercase_cstr(const char* str);

NOM_EXPORT
std::string to_lowercase_string(const std::string& str, nom::size_type len);
NOM_EXPORT std::string to_lowercase_string(const std::string& str);

NOM_EXPORT
const char* to_uppercase_cstr(const char* str, nom::size_type len);
NOM_EXPORT const char* to_uppercase_cstr(const char* str);

NOM_EXPORT
std::string to_uppercase_string(const std::string& str, nom::size_type len);
NOM_EXPORT std::string to_uppercase_string(const std::string& str);

} // namespace nom

#endif // include guard defined
