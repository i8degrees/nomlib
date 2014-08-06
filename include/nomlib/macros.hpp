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
#ifndef NOMLIB_CORE_MACROS_HPP
#define NOMLIB_CORE_MACROS_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/core/clock.hpp"  // nom::timestamp

// Note that the following are just the general-purpose (engine-wide) macros.
// Other major sources of macro definitions: nomlib/config.hpp, nomlib/types.hpp,
// nomlib/platforms.hpp.

#define NOM_DYN_SHARED_PTR_CAST(type, expr) \
  ( std::dynamic_pointer_cast<type>(expr) )

#define NOM_DYN_PTR_CAST(type, expr) \
  ( dynamic_cast<type>(expr) )

#define NOM_SCAST(type, expression) static_cast<type>(expression)
#define NOM_CCAST(type, expression) const_cast<type>(expression)

#define NOM_ENDL(reserved) ( std::cout << std::endl )
#define NOM_DASHED_ENDL(reserved) ( std::cout << "---" << std::endl )

#define NOM_TIMESTAMP(reserved) \
  ( std::cout << nom::timestamp() << std::endl )

#define NOM_DELETE_PTR(var) \
  if( var != nullptr ) delete var; var = nullptr;

#define NOM_DELETE_SMART_PTR(var) \
  if( var != nullptr ) var.reset();

#define NOM_DELETE_VOID_PTR(var) \
  if( var != nullptr ) free(var); var = nullptr;

#endif // include guard defined
