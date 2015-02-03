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
#ifndef NOMLIB_GRAPHICS_SPRITE_SPRITE_SHEET_HPP
#define NOMLIB_GRAPHICS_SPRITE_SPRITE_SHEET_HPP

#include <string>
#include <map>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/version.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/ptree.hpp"

// #define NOM_DEBUG_SPRITE_SHEET_JSON_LOAD

namespace nom {

/// \brief Container class for deserialized sprite sheet data
class SpriteSheet
{
  public:
    static const VersionInfo VERSION;

    typedef std::shared_ptr<SpriteSheet> SharedPtr;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    SpriteSheet();

    /// Destructor.
    ~SpriteSheet();

    /// Make a duplicate of this object's instance
    SpriteSheet::SharedPtr clone() const;

    /// Get the calculations made for a particular ID number.
    const IntRect& dimensions(int index) const;

    /// Obtain the number of frames this object contains
    int frames() const;

    /// \brief Obtain whether the sprite sheet is empty or not.
    bool empty() const;

    /// \brief Get the current version of the sprite sheet file format.
    std::string version() const;

    /// \brief Get the sprite sheet's filename.
    ///
    /// \remarks This data corresponds to the meta-data object node's 'filename'
    /// value.
    const std::string& sheet_filename() const;

    /// \brief Get the total sprite sheet width.
    ///
    /// \remarks This data corresponds to the meta-data object node's 'width'
    /// value.
    int sheet_width() const;

    /// \brief Get the total sprite sheet height.
    ///
    /// \remarks This data corresponds to the meta-data object node's 'height'
    /// value.
    int sheet_height() const;

    /// \brief Get the sprite sheet's padding.
    ///
    /// \remarks This data corresponds to the meta-data object node's 'padding'
    /// value.
    int sheet_padding() const;

    /// \brief Get the sprite sheet's spacing.
    ///
    /// \remarks This data corresponds to the meta-data object node's 'spacing'
    /// value.
    int sheet_spacing() const;

    /// \brief Get the total number of sprite frames.
    ///
    /// \remarks This data corresponds to the meta-data object node's
    /// 'total_frames' value.
    int total_frames() const;

    /// \brief De-serialize an existing JSON file.
    ///
    /// \param filename The absolute file path to the JSON input.
    ///
    /// \returns An object filled with complete data from a file on success, or
    /// a non-modified object upon failure.
    ///
    /// \todo Rename to load_sheet_file?
    bool load_file(const std::string& filename);

    /// \brief Load a sprite sheet from a de-serialized object.
    ///
    /// \param object An existing, de-serialized object to use.
    bool load_sheet_object(const Value& object);

    /// \brief Erase an existing frame from the sheet.
    bool remove_frame(nom::size_type frame);

    /// Dump the state of this object instance
    void dump() const;

  private:
    /// \brief Sprite sheet container.
    ///
    /// \remarks The key field of this container (an int) represents the frame's
    /// identifier, whereas its second field represents the frame's data.
    std::map<int, IntRect> sheet_;

    /// \brief Source filename used is saved with the output (meta-data)
    std::string sheet_filename_;

    /// \brief Source spacing used is saved with the output (meta-data)
    int sheet_spacing_;

    /// \brief Source padding used is saved with the output (meta-data)
    int sheet_padding_;

    /// \brief Source sheet_width used is saved with the output (meta-data)
    int sheet_width_;

    /// \brief Source sheet_height used is saved with the output (meta-data)
    int sheet_height_;

    /// \brief The total number of sprite frames; as per the number of objects
    /// in the frames object node.
    int total_frames_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::SpriteSheet
/// \ingroup graphics
///
/// TexturePacker with a custom exporter for nomlib is used for saving sprite
/// frame data. See Resources/SharedSupport/TexturePacker for the custom
/// exporter used.
///
