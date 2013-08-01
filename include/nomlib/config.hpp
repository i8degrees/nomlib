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
#ifndef NOMLIB_CONFIG_HEADERS
#define NOMLIB_CONFIG_HEADERS

// nomlib version
#include "nomlib/version.hpp"

// nomlib data types
#include "nomlib/types.hpp"

// Platform detection
#include "nomlib/platforms.hpp"

/// \brief nomlib Debugging Options
///
/// Available preprocessor flags:
///
///     DEBUG
/// Activates output of all info, warning and error messages.
///
///     DEBUG_ASSERT
/// Activate run-time assert checks scattered throughout the code-base.
///
///     DEBUG_TRACE
/// Special debug level for activating output of each class object construction
/// and destruction that occurs. Unless you are an active developer of nomlib,
/// or perhaps suspect that an error in your software could be caused by nomlib,
/// you are unlikely to be interested in this debug level -- this option
/// generates a hefty amount of output text!
///
/// These definitions are passed at build time for activation like so:
///
///     cd build && cmake -DDEBUG=on ..
///
/// You may mix and match these flags however you wish.

#ifndef __cplusplus
  #warning "nomlib requires a C++11 compiler"
#endif


#endif // NOMLIB_CONFIG_HEADERS defined
