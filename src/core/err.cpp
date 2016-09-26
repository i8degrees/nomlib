/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/core/err.hpp"

namespace nom {

// Global err buffer
static err err_buffer;

err::err()
  : message("\0")
{
  // :-(
}

err::~err()
{
  // NOTE: An explicitly-declared destructor is necessary for building with
  // MSVCPP 2013
}

err::err(const err& rhs)
  : message(rhs.message.str())
  // message(rhs.message)
{
  // NOTE: An explicitly-declared copy constructor is necessary for building
  // with MSVCPP 2013
}

#if 0
std::stringstream& operator <<(std::stringstream& os, const err& error)
{
  os << error.message.rdbuf();

  return os;
}
#endif

bool error_state()
{
  auto string_length = err_buffer.message.str().length();
  bool error_state = false;

  if(string_length > 0) {
    error_state = true;
  }

  return error_state;
}

std::string error()
{
  return err_buffer.message.str();
  // return err_buffer.message;
}

err make_error(const char* message)
{
  err error;

  if( message != nullptr ) {
    error.message << message;
    // error.message = message;
  }

  return error;
}

void set_error(const err& error)
{
  nom::clear_error();

  err_buffer.message << error.message.str();
  // err_buffer.message = error.message;
}

void set_error(const char* message)
{
  nom::clear_error();

  if(message != nullptr) {
    err_buffer.message << message;
    // err_buffer.message = message;
  }
}

void set_error(const std::string& message)
{
  nom::clear_error();

  if(message.length() > 0) {
    err_buffer.message << message;
  }
}

void clear_error()
{
  err_buffer.message.str("");
  // err_buffer.message = "\0";
}

} // namespace nom
