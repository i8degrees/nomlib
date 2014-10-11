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
#ifndef NOMLIB_CORE_SDL2_ASSERTION_HELPERS_HPP
#define NOMLIB_CORE_SDL2_ASSERTION_HELPERS_HPP

#include <SDL.h>

#include "nomlib/config.hpp"

namespace nom {

/// \brief A custom assertion callback handler for disabling both the dialog
/// prompt and assertion report.
///
/// \remarks This method is used in libRocketDataGridTest::DataSourceModel
/// in order to ignore all the assertions that are triggered when testing
/// invalid positions.
///
/// \note By default, NOM_ASSERT uses SDL's assertion facilities. This handler
/// will do nothing when NOM_ASSERT has been disabled.
SDL_assert_state null_assert_handler(const SDL_assert_data*, void*);

/// \brief A custom assertion callback handler for reporting failed assertions
/// at the end of the program execution (no dialog prompt).
///
/// \remarks This method is used in libRocketDataGridTest::DataSourceModel
/// in order to ignore all the assertions that are triggered when testing
/// invalid positions.
///
/// \note By default, NOM_ASSERT uses SDL's assertion facilities. This handler
/// will do nothing when NOM_ASSERT has been disabled.
SDL_assert_state log_assert_handler(const SDL_assert_data* item, void*);

/// \brief Set an application-defined assertion handler.
///
/// \param callback The function to call when an assertion fails or nullptr for
/// the default handler; see Remarks for details
///
/// \param userdata A pointer that is passed to the handler callback.
///
/// \note This callback is **not** reset to SDL's internal handler upon a call
/// to SDL_Quit().
///
/// \see https://wiki.libsdl.org/SDL_SetAssertionHandler
void set_assertion_handler(SDL_AssertionHandler callback, void* userdata);

} // namespace nom

#endif // include guard defined
