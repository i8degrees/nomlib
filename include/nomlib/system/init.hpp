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
#ifndef NOMLIB_SYSTEM_INIT_HPP
#define NOMLIB_SYSTEM_INIT_HPP

#include <iostream>

#include "nomlib/config.hpp"
#include "nomlib/system/resource_types.hpp"

namespace nom {

  /// \brief Class interface for accessing global font resources
///
/// \note A nom::RenderWindow must be initialized before the resource
/// loading method call can be used -- nom::FontCache::load_resource.
///
/// \see nom::PlatformSettings, nom::ResourceCache.
class SystemFonts
{
  public:
    /// \brief Get the status of fonts cache.
    static bool initialized( void );

    /// \brief Create the global fonts resource cache.
    static void initialize( void );

    /// \brief Get the cache used for font resources.
    ///
    /// \returns The reference to the global font cache.
    ///
    /// \remarks This method call will automatically initialize the fonts cache
    /// if it is not yet initialized.
    static FontCache& cache( void );

    /// \brief Destructor; clear the fonts resource cache.
    ///
    /// \note this must be done *before* SDL2_ttf is shutdown, as we do not own
    /// the pointers that it will invalidate upon its shutdown.
    ///
    /// \see nom::quit.
    static void shutdown( void );

  private:
    /// \brief The global list of available fonts for use by the engine and
    /// end-user.
    static std::shared_ptr<FontCache> cache_;

    /// \brief Track object's state in order to ensure one-time initialization.
    static bool initialized_;
};

// Forward declarations
class ColorDatabase;

/// \brief Class interface for accessing global color resources
///
/// \see nom::PlatformSettings, nom::ColorDatabase.
class SystemColors
{
  public:
    /// \brief Get the status of the global color database.
    static bool initialized( void );

    /// \brief Create the global color database.
    static void initialize( void );

    /// \brief Get an object reference to the global color database.
    ///
    /// \returns The object reference to the global color database.
    ///
    /// \remarks This method call will automatically initialize the color
    /// database if it is not yet initialized.
    static ColorDatabase& colors( void );

    /// \brief Destructor.
    static void shutdown( void );

  private:
    /// \brief The global color database for use by the engine and end-user.
    static std::unique_ptr<ColorDatabase> colors_;

    /// \brief Track object's state in order to ensure one-time initialization.
    static bool initialized_;
};

bool init_third_party( uint32 flags );

/// Initialize the engine.
///
/// \remarks The system fonts cache is initialized in this method call.
///
/// \note The function call will attempt to change the working directory to
/// the location of the binary being executed. This is not guaranteed to
/// succeed.
///
/// \note This should be called before calling any other library methods.
///
/// \todo Rename to nom_init.
bool init( int argc, char* argv[] );

/// \brief Shutdown the engine.
///
/// \remarks The order of shutdown starts with freeing global (static) engine
/// caches (such as the system fonts cache) and ends with shutting down
/// third-party libraries in their respective order (SDL2_ttf, SDL2_image, SDL2).
///
/// \fixme If we use objects that utilize SDL2_ttf (nom::TrueTypeFont), we crash
/// with a segfault (11) if we do not use atexit with this function call, because
/// ResourceCache<Font> needs to be destructed *last*. I *think* if we
/// implemented some sort of reference counting, as is done in SFML's sf::Font
/// class, we may resolve the issue all together, since we could check the
/// actual ref count within our custom deleter (see nom::priv::TTF_FreeFont).
///
/// \todo Rename to nom_quit.
void quit( void );

} // namespace nom

#endif // include guard defined
