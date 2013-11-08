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
#ifndef NOMLIB_JSON_CONFIG_HPP
#define NOMLIB_JSON_CONFIG_HPP

#include "json.h" // JSONCPP

#include "nomlib/config.hpp"

namespace nom {
namespace JSON {

/// Platform / library dependent definition
typedef Json::Value ValueType;
typedef enum Json::ValueType JSONValueType;

/// Formatting options (not presently used).
///
/// \todo Look into jsoncpp documentation and see if there's an equivalent for
/// any of these formatting options?
enum
{
  NoFormatting = 0, // no formatting applied
  PrettyPrint = 1, // json_spirit::pretty_print
  CompactArrays = 2 // json_spirit::single_line_arrays; implies PrettyPrint
};

/// Number of spaces to pass to jsoncpp for tab indention
const std::string indention_spaces = "  "; // two spaces


} // namespace JSON
} // namespace nom

#endif // include guard defined

/// \brief Configuration options for nom::JSON engine
///
/// \ingroup json
///
///   [TO BE WRITTEN]
///
