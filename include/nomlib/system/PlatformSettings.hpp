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
#ifndef NOMLIB_SYSTEM_PLATFORM_SETTINGS_HPP
#define NOMLIB_SYSTEM_PLATFORM_SETTINGS_HPP

// Public header file

#include "nomlib/config.hpp"
#include "nomlib/system/init.hpp"
#include "nomlib/math/Color4.hpp"

namespace nom {

// Forward declarations
class Font;

enum SystemColorType
{
  ButtonLabelText
};

enum SystemFontType
{
  /// Fixed-pitch (mono-spaced) bitmap font.
  FixedBitmap,

  /// Variable-pitch (proportional) bitmap font.
  ///
  /// \note Not used; reserved for future implementation.
  VariableBitmap,

  /// Fixed-pitch (mono-spaced) TrueType font.
  FixedTrueType,

  /// Variable-pitch (proportional) TrueType font.
  VariableTrueType
};

/// \brief Environment (OS) specific configuration interface
///
/// \remarks This class interface depends on initialization of the font cache --
/// see the nom::enumerate_fonts function.
///
/// \note Inspired by wxSystemSettings.
///
/// \note SystemFonts::initialize and nom::enumerate_fonts *must* have been
/// called before using this interface. Normally, this is done for us, through
/// nom::init or SDLApp::initialize.
class PlatformSettings
{
  public:
    /// \brief Get the status of fonts cache.
    static bool initialized( void );

    /// \brief Create the settings for the platform.
    ///
    /// \note nom::enumerate_fonts is called within this method.
    static void initialize( void );

    static Color4i get_system_color( SystemColorType index );
    static Font* get_system_font( SystemFontType index );
    static Font* find_system_font( const std::string& key );

    /// \brief Initialization of available font resources for both internal and
    /// end-user (developer) access. Note that the function implementation is
    /// platform-specific.
    ///
    /// \remarks Fonts are enumerated using standard font paths (both system-wide &
    /// known engine resource paths), and if found, appended onto the global (static)
    /// resource cache for fonts.
    static void enumerate_fonts( void );

  private:
    /// \brief Track object's state in order to ensure enumeration of fonts has
    /// been done.
    static bool initialized_;
};

} // namespace nom

#endif // include guard defined
