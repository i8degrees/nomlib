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
#ifndef NOMLIB_SPRITE_SHEET_HPP
#define NOMLIB_SPRITE_SHEET_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/serializers.hpp"
#include "nomlib/ptree.hpp"

namespace nom {

extern const int NOM_SPRITE_SHEET_MAJOR_VERSION;
extern const int NOM_SPRITE_SHEET_MINOR_VERSION;
extern const int NOM_SPRITE_SHEET_PATCH_VERSION;

//#define NOM_DEBUG_SPRITE_SHEET

/// Not implemented; reserved for future usage
#define NOM_DEBUG_SPRITE_SHEET_JSON_SAVE

// #define NOM_DEBUG_SPRITE_SHEET_JSON_LOAD

/// \brief Specialized class container for the creation of sprite sheets via
/// RFC 4627 JSON-compliant file input
class SpriteSheet
{
  public:
    typedef std::shared_ptr<SpriteSheet> SharedPtr;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    SpriteSheet ( void );

    /// Destructor.
    ~SpriteSheet ( void );

    /// Construct a sprite sheet from an existing sprite sheet file
    SpriteSheet ( const std::string& filename );

    /// Construct a sprite sheet from a given list of arguments. The total tile
    /// count is used if the num_sprites argument is not initialized.
    ///
    /// The filename specified is used only as meta-data.
    ///
    /// Padding is applied on all four sides. Spacing is applied between each
    /// tile.
    ///
    /// \todo Re-order these arguments to match the order of instance variables
    /// \todo Add additional sanity-check asserts for spacing, padding
    /// and so on
    SpriteSheet (
                  const std::string& filename,
                  int32 sheet_width, int32 sheet_height,
                  int32 sprite_width, int32 sprite_height,
                  int32 spacing, int32 padding, int32 num_sprites
                );

    /// Make a duplicate of this object's instance
    SpriteSheet::SharedPtr clone ( void ) const;

    /// Get the calculations made for a particular ID number.
    const IntRect& dimensions ( int32 index ) const;

    /// Obtain the number of frames this object contains
    int32 frames ( void ) const;

    /// \brief Obtain whether the sprite sheet is empty or not.
    bool empty( void ) const;

    const std::string& sheet_filename ( void ) const;

    /// \brief Serialize the existing object data to a JSON file.
    ///
    /// \param filename The absolute file path to the resulting JSON output.
    ///
    /// \remarks The requested file path is created upon success.
    bool save( const std::string& filename );

    /// \brief Un-serialize an existing JSON file.
    ///
    /// \param filename The absolute file path to the JSON input.
    ///
    /// \returns An object filled with complete data from a file on success, or
    /// a non-modified object upon failure.
    bool load( const std::string& filename );

    /// Dump the state of this object instance
    void dump ( void ) const;

  private:
    /// Our sprite sheet values container
    std::vector<IntRect> sheet;

    /// Source filename used is saved with the output (meta-data)
    std::string sheet_filename_;

    /// Source number of sprites specified; this is saved with the resulting
    /// output as meta-data
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
/// #include <nomlib/graphics/graphics.hpp>
///
/// nom::SpriteSheet card_faces_sheet ( "faces.png", 256, 262, 64, 64, 0, 1, 16 );
/// card_faces_sheet.save( "faces.json" );
///
/// nom::Sprite card_face = nom::Sprite ( card_faces_sheet );
///
/// // ...or!
///
/// nom::Sprite card_face ( nom::SpriteSheet ( card_faces_sheet ( "faces.json" ) ) );
///
/// \endcode
///
///
/// \code
///
///     REFERENCES
///
/// 1.  http://www.dreamincode.net/forums/topic/179429-sdl-sprite-sheet/
/// \endcode
///
/// \todo Re-design the JSON layout used, perhaps with something like:
/// { "metadata": { }, "<filename>": { "<ID>": { } } }
///
/// \todo Re-consider our sheet vector's data type (nom::IntRect); we should
/// ideally be loading the object's ID into this vector
///
/// \todo Create structs for our instance vars
///
/// \todo Buffer the file meta-data like we do the other JSON objects inside
/// load method
