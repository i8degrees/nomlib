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
#ifndef NOMLIB_GRAPHICS_FONTS_FONT_HPP
#define NOMLIB_GRAPHICS_FONTS_FONT_HPP

#include "nomlib/config.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"

namespace nom {

/// \brief Font interface wrapper for nom::IFont derived objects.
///
/// \see nom::IFont, nom::BitmapFont, nom::TrueTypeFont, nom::BMFont
class Font
{
  public:
    typedef Font self_type;
    typedef IFont font_type;

    /// \brief Default constructor; initialize the font to NULL and the
    /// sharable state to false.
    Font();

    /// \brief Destructor.
    ~Font();

    /// \brief Constructor for initializing an object from a std::shared_ptr
    /// object<nom::IFont> object.
    Font(const std::shared_ptr<font_type>& font);

    /// \brief Constructor for initializing an object from a nom::IFont derived
    /// raw pointer.
    Font(font_type* font);

    /// \brief Read-write pointer operator overload.
    ///
    /// \remarks This method allows you to access the wrapped font object
    /// directly. This will be one of the nom::IFont object types.
    ///
    /// \see nom::BitmapFont, nom::TrueTypeFont.
    font_type* operator ->() const;

    /// \brief Equality comparison overload.
    ///
    /// \remarks The comparison check is done only on the pointers to the stored
    /// font object -- whether or not the objects point to the same memory
    /// location -- not the actual data stored in the font (as this would be
    /// inefficient).
    bool operator ==(const self_type& rhs) const;

    /// \brief In-equality comparison overload.
    ///
    /// \remarks The comparison check is done only on the pointers to the stored
    /// font object -- whether or not the objects point to the same memory
    /// location -- not the actual data stored in the font (as this would be
    /// inefficient).
    bool operator !=(const self_type& rhs) const;

    /// \brief Get the object's unique state.
    ///
    /// \remarks The uniqueness of the object -- whether or not it is being held
    /// by multiple objects -- depends on the implementation of std::shared_ptr.
    bool unique() const;

    /// \brief Determine the type of font by filename extension.
    ///
    /// \param filename The file name (absolute or relative) with the full
    /// extension. File extensions recognized are 'png', 'bmp', 'ttf', 'ttc'
    /// 'otf', 'ttf' and 'fnt'.
    ///
    /// \returns One of the IFont::FontType enumerations.
    ///
    /// \remarks This method is used by ::load in order to determine the type
    /// of font interface to initialize.
    IFont::FontType type(const std::string& filename) const;

    /// \brief Load a font from a filename.
    ///
    /// \remarks This is a convenience method provided for ease of access.
    ///
    /// \note If you bypass this method (not recommended), you must ensure that
    /// you have constructed the nom::Font object with a valid
    /// nom::IFont-derived object that has been loaded into memory with its
    /// appropriate ::load call. The nom::Font(font_type* font) constructor is
    /// used for the constructing call.
    ///
    /// \todo Rename to load_file
    bool load(const std::string& filename);

  private:
    std::shared_ptr<Font::font_type> font_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Font
/// \ingroup graphics
///
/// A class wrapper interface for a nom::IFont derived objects. This wrapper
/// has the side intention of being conveniently used within a
/// nom::ResourceCache (it does not support pure abstract objects, such as
/// nom::IFont).
///
/// \code
///
/// // Automatic initialization of an object; uses filename extension to
/// // determine the type of object to initialize / use:
/// nom::Font font;
///
/// if( font.load( file_path ) == false )
/// {
///   // Handle Err
/// }
///
/// if( font.valid() )
/// {
///   font.set_point_size( 24 );
///
///   /// ...
/// }
///
/// // Manual initialization of an object (the old style used in the engine):
/// nom::Font font( std::shared_ptr<IFont>( new BitmapFont() );
/// // nom::Font font( std::shared_ptr<IFont>( new TrueTypeFont() );
///
/// if( font->load( file_path ) == false )
/// {
///   // Handle err
/// }
///
/// if( font->valid() )
/// {
///   font->set_point_size( 24 );
/// }
///
/// \endcode
///
