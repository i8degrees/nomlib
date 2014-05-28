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

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/BitmapFont.hpp"
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

// #define NOM_DEBUG_FONT_SHARING

namespace nom {

static const int DEFAULT_FONT_SIZE = 12;

/// \brief Font interface wrapper for nom::IFont derived objects.
///
/// \see nom::IFont, nom::BitmapFont, nom::TrueTypeFont
class Font
{
  public:
    typedef Font self_type;
    typedef IFont font_type;
    typedef std::shared_ptr<font_type> value_type;

    /// \brief Default constructor; initialize the font to NULL and the
    /// sharable state to false.
    Font( void );

    /// \brief Destructor.
    ~Font( void );

    /// \brief Constructor for initializing an object from a nom::IFont derived
    /// object.
    // Font( const font_type& font );

    /// \brief Constructor for initializing an object from a std::shared_ptr
    /// object<nom::IFont> object.
    Font( const value_type& font );

    // \brief Constructor for initializing an object from a nom::IFont derived
    // raw pointer.
    //
    // \note Used by Text::set_font( font_type* font ).
    // Font( font_type* font );

    /// \brief Copy assignment operator for Font object pointers.
    ///
    /// \note Used by Text::set_font.
    self_type& operator =( const self_type* rhs );

    // const font_type& operator *( void ) const;

    /// \remarks Font::share is called beforehand.
    // font_type& operator *( void );

    /// \brief Read-only pointer operator overload.
    ///
    /// \remarks This method is provided for read-only access of the stored
    /// font object. You should probably not be using this method, but be using
    /// the convenience methods provided in this class (see below).
    ///
    /// \note Used by nom::Text.
    ///
    /// \todo Consider removal of this method?.
    const font_type* operator ->( void ) const;

    /// \brief Read-write pointer operator overload.
    ///
    /// \remarks This method is provided for read-write access of the stored
    /// font object. You should probably not be using this method, but be using
    /// the convenience methods provided in this class (see below).
    ///
    /// \note Font::share is called beforehand.
    ///
    /// \note Used by nom::Text.
    ///
    /// \todo Consider removal of this method?.
    font_type* operator ->( void );

    /// \brief Equality comparison overload.
    ///
    /// \remarks The comparison check is done only on the pointers to the stored
    /// font object -- whether or not the objects point to the same memory
    /// location -- not the actual data stored in the font (as this would be
    /// inefficient).
    bool operator ==( const self_type& rhs ) const;

    /// \brief In-equality comparison overload.
    ///
    /// \remarks The comparison check is done only on the pointers to the stored
    /// font object -- whether or not the objects point to the same memory
    /// location -- not the actual data stored in the font (as this would be
    /// inefficient).
    bool operator !=( const self_type& rhs ) const;

    /// \brief Get the object's unique state.
    ///
    /// \remarks The uniqueness of the object -- whether or not it is being held
    /// by multiple objects -- depends on the implementation of std::shared_ptr.
    bool unique( void ) const;

    /// \brief Get the object's validity state.
    bool valid( void ) const;

    /// \brief Get the object's sharable state.
    bool sharable( void ) const;

    /// \brief Determine the type of font by filename extension.
    ///
    /// \param filename The file name (absolute or relative) with the full
    /// extension. File extensions recognized are 'png', 'bmp', 'ttf', 'ttc',
    /// and 'otf'.
    ///
    /// \returns One of the IFont::FontType enumerations.
    ///
    /// \remarks This method is used by ::load in order to determine the type
    /// of font object to initialize.
    IFont::FontType type( const std::string& filename ) const;

    /// \brief Load a font from a filename.
    ///
    /// \remarks This is a convenience method provided for ease of access.
    ///
    /// \note If the nom::Font::operator -> method is used (not recommended),
    /// you must ensure that you first initialize the nom::Font object with the
    /// proper nom::IFont derived object type.
    bool load( const std::string& filename );

    /// \brief Set the stored font's point size.
    bool set_point_size( int point_size );

    /// \brief Set the stored font's hinting.
    void set_hinting( int type );

    /// \brief Set the stored font's outline.
    bool set_outline( int outline );

    /// \brief Set the stored font's sharable state.
    ///
    /// \remarks This method is provided to allow the end-user the final call
    /// in whether or not to consider the stored font object a resource in
    /// which can be shared (separate, new cloned object) or not (the same
    /// pointer shared across multiple objects). This ultimately has an effect
    /// on things like memory consumption.
    ///
    /// \see Default constructor of nom::Font for default sharable state.
    ///
    /// \see Used by the default font load handler, nom::create_font.
    void set_sharable( bool state );

    /// \brief Make a shared (separate, new clone) copy of the font.
    ///
    /// \remarks Before the stored font object is shared, it must meet all of
    /// the following criteria: a) the stored font object must not be NULL; b)
    /// the stored font object must not be unique (see ::unique); c) this object
    /// instance must be marked shared (see ::sharable, ::set_sharable).
    ///
    /// \note This is where the copy-on-write implementation lies.
    void share( void );

  private:
    value_type font_;
    bool sharable_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Font
/// \ingroup system
///
/// A class interface for a nom::IFont derived object (a common interface
/// handling the differences different font types, such as TrueType and Bitmap).
/// Reference counting via std::shared_ptr and optionally, copy-on-write
/// (lazy copying) are built-in features.
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
/// \see Derives from the implementation at http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-on-write
/// \see http://archive.today/20121222041326/cdumez.blogspot.tw/2011/03/implicit-explicit-data-sharing-with-qt.html
/// \see http://www.gotw.ca/publications/optimizations.htm
/// \see http://seanmiddleditch.com/journal/2014/01/dangers-of-stdshared_ptr/
