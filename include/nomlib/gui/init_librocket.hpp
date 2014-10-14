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
#ifndef NOMLIB_GUI_INIT_LIBROCKET_HPP
#define NOMLIB_GUI_INIT_LIBROCKET_HPP

#include "nomlib/config.hpp"

// Forward declarations (third-party)
namespace Rocket {
namespace Core {

class Context;
class FileInterface;
class SystemInterface;

} // namespace Core
} // namespace Rocket

namespace nom {

/// \brief Initialize libRocket interface.
///
/// \remarks The pointer ownership transfers to libRocket.
///
/// \note This must be done before the creation of a libRocket context.
///
/// \param fs   Platform-dependent file interface implementation.
/// \param sys  Platform-dependent system interface implementation.
///
/// \see nom::UIContext.
/// \see nom::RocketFileInterface
/// \see nom::RocketSDL2SystemInterface
bool init_librocket(  Rocket::Core::FileInterface* fs,
                      Rocket::Core::SystemInterface* sys );

/// \brief Shutdown libRocket interface.
void shutdown_librocket();

} // namespace nom

#endif // include guard defined
