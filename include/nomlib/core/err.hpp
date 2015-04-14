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
#ifndef NOMLIB_CORE_ERR_HPP
#define NOMLIB_CORE_ERR_HPP

#include <sstream>

#include "nomlib/config.hpp"

namespace nom {

struct err
{
  std::stringstream message;
};

inline std::stringstream& operator <<(std::stringstream& os, const err& error);

/// \brief Get the current error.
std::string error();

/// \brief Create an error.
err make_error(const char* message);

/// \brief Set the error message.
///
/// \remarks The global error buffer is cleared.
void set_error(const err& error);

/// \brief Set the error message.
///
/// \remarks The global error buffer is cleared.
void set_error(const char* message);

/// \brief Clears the global error buffer.
void clear_error();

// Common error types

const err OUT_OF_MEMORY_ERR =
  nom::make_error("Failed to allocate memory");

const err NULL_ARGUMENT_ERR =
  nom::make_error("Passed NULL argument");

} // namespace nom

#endif // include guard defined

/// \class nom::err
/// \ingroup core
///
/// ## Usage Examples
///
/// \code
///
/// #include <nomlib/core.hpp>
///
/// err error;
/// error.message << "My error message";
/// nom::set_error(error);
/// NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, nom::error() );
///
