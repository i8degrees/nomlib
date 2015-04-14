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
#ifndef NOMLIB_CORE_HELPERS_HPP
#define NOMLIB_CORE_HELPERS_HPP

#include <cstring>
#include <memory>

#include "nomlib/config.hpp"

namespace nom {

// TODO: Remove nom::priv namespace for the string helper funcs
namespace priv {

/// \brief Maximum size a nom::Value string type may be
///
/// \remarks Buffer overflow protection.
const nom::size_type MAX_STRING_LENGTH = 256;

int string_to_integer(const char* str);
int string_to_integer(const std::string& str);

nom::size_type string_length(const char* str);
nom::size_type string_length(const std::string& str);

int compare_string(const char* str1, const char* str2);
int compare_string(const std::string& str1, const std::string& str2);

void copy_string(const char* source, char* dest);

/// \brief Create a deep-copy instance a C style string.
///
/// \param length Size of the string to copy.
///
/// \returns Null-terminated string up to MAX_STRING_LENGTH.
const char* duplicate_string(const char* str, nom::size_type length);

const char* duplicate_string(const std::string& str, nom::size_type length);

void free_string(const char* ptr);

} // namespace priv

/// Convenience helper for providing a version of std::make_unique for
/// std::unique_ptr -- C++11 forgot to provide one like they did for
/// std::shared_ptr!
///
/// This version only works for non-array types. Custom deleter support also has
/// not been added.
///
///   References
///
/// 1. http://stackoverflow.com/questions/7038357/make-unique-and-perfect-forwarding
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>( new T( std::forward<Args>( args ) ... ) );
}

} // namespace nom

#endif // include guard defined
