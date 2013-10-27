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
#ifndef NOMLIB_SPRITE_SHEET_HPP
#define NOMLIB_SPRITE_SHEET_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <memory>

// nomlib/config.hpp MUST be defined before using conditional preprocessing
// statements for detecting platform
#include "nomlib/config.hpp"
#include "nomlib/version.hpp"
#include "nomlib/math/Coords.hpp"

/*
#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif

#include <nomlib/json.hpp>
*/

namespace nom {

//#define NOM_DEBUG_SPRITE_SHEET

/// \brief Specialized class container for the creation of sprite sheets via
/// JSON-compliant file input.
class SpriteSheet
{
  public:
    typedef std::shared_ptr<SpriteSheet> SharedPtr;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    SpriteSheet ( void );

    /// Construct a sprite sheet from an existing sprite sheet file
    SpriteSheet ( const std::string& filename );

    /// Construct a sprite sheet from a given list of arguments. The total tile
    /// count is used if the num_sprites argument is not initialized.
    ///
    /// The filename specified is used only as meta-data.
    ///
    /// Padding is applied on all four sides. Spacing is applied between each
    /// tile.
    SpriteSheet (
                  const std::string& filename,
                  int32 sheet_width, int32 sheet_height,
                  int32 sprite_width, int32 sprite_height,
                  int32 spacing, int32 padding, int32 num_sprites
                );

    /// Destructor.
    ~SpriteSheet ( void );

    /// Make a duplicate of this object's instance
    SpriteSheet::SharedPtr clone ( void ) const;

    /// Get the calculations made for a particular ID number.
    const Coords dimensions ( int32 index ) const;

    /// Obtain the number of frames this object contains
    int32 frames ( void ) const;

    /// Save the current sprite sheet data calculations to a file as a series
    /// of RFC 4627 compliant JSON objects.
    bool save ( const std::string& filename );

    /// Load saved sprite sheet data from a file encoded as an RFC 4627
    /// compliant JSON object.
    bool load ( const std::string& filename );

  private:
    /// Our sprite sheet values container.
    std::vector<Coords> sheet;

    /// Source filename used is saved with the output (meta-data)
    std::string sheet_filename;

    /// Source number of sprites specified; this is saved with the resulting
    /// output as meta-data.
    int32 sheet_sprites;

    /// Source spacing used is saved with the output (meta-data)
    int32 sheet_spacing;

    /// Source padding used is saved with the output (meta-data)
    int32 sheet_padding;

    /// Source sheet_width used is saved with the output (meta-data)
    int32 sheet_width;

    /// Source sheet_height used is saved with the output (meta-data)
    int32 sheet_height;
};


} // namespace nom

#endif // include guard defined

/// \class nom::SpriteSheet
/// \ingroup graphics
///
///         [DESCRIPTION STUB]
///
/// \code
///
/// Example of 4X4 Sheet
///   ________________
///  | 0 | 1 | 2 | 3 |
///  |===============|
///  | 0 | 1 | 2 | 3 |
///  |===============|
///  | 0 | 1 | 2 | 3 |
///  |===============|
///  | 0 | 1 | 2 | 3 |
///  |---------------|
///
/// \endcode
///
/// Usage example:
/// \code
///
/// #include <nomlib/graphics/SpriteSheet.hpp>
/// #include <nomlib/graphics/Sprite.hpp>
///
/// nom::SpriteSheet card_faces_sheet ( "faces.png", 256, 262, 64, 64, 0, 1, 16 );
/// card_faces_sheet.save( "faces.json" );
///
/// nom::Sprite card_face = nom::Sprite ( card_faces_sheet );
///
/// \endcode
///
/// ...or!
///
/// \code
///
/// nom::Sprite card_face ( nom::SpriteSheet ( card_faces_sheet ( "faces.json" ) ) );
///
/// \endcode
///
/// \code
///
///     REFERENCES
///
/// 1.  http://www.dreamincode.net/forums/topic/179429-sdl-sprite-sheet/
/// \endcode
