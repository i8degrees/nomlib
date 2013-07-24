/******************************************************************************

    SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_TTF_FONT_HEADERS
#define NOMLIB_SDL_TTF_FONT_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <SDL/SDL_ttf.h>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/gfx/IFont.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gfx/Canvas.hpp"
#include "nomlib/gfx/IDrawable.hpp"

namespace nom {
  namespace priv {

/// Custom deleter for TTF_Font* smart pointers; can be used as a debugging aid.
void TTF_FreeSurface ( TTF_Font* );

  } // namespace priv
} // namespace nom


namespace nom {

class SDL_Font: //  public IDrawable, //  "is a" inheritance
                    public IFont // "is-a" relationship
{
  public:
    /// Default constructor; we initialize the SDL_ttf extension here
    SDL_Font ( void );

    /// Default destructor; we shutdown the SDL_ttf extension here
    ~SDL_Font ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Obtains set text string buffer; defaults to \0
    const std::string& getText ( void ) const;

    /// Compute the width in pixels of the set text string; defaults to zero (0)
    int32 getFontWidth ( void ) const;

    /// Compute the height in pixels of the set text string; defaults to zero (0)
    int32 getFontHeight ( void ) const;

    FontStyle getFontStyle ( void ) const;

    void setFontStyle ( uint8 style, uint8 options );

    /// Set a new text point size
    void setFontSize ( int32 point_size );

    /// \brief Set a new text string for drawing; defaults to \0
    ///
    /// NOTE: We render the font drawing surface here
    void setText ( const std::string& text );

    /// \brief Load a new TTF or FON from a file
    ///
    /// Support for the file format is determined
    /// by the SDL_ttf extension
    bool Load ( const std::string& filename, const Color& colorkey,
                int32 font_size = 12, bool use_cache = 0
              );

    void Update ( void );

    /// Draw the set text string to the video surface
    void Draw ( void* video_buffer ) /*const*/;

  private:
    /// Surface where font for drawing is rendered to
    Canvas font_buffer;
    /// Font file data, used by SDL_ttf extension
    std::shared_ptr<TTF_Font> font;
    /// holds contents of text as a string buffer
    std::string text_buffer;

    /// Current text effect set
    enum FontStyle text_style;
    uint8 style_options;

    /// Store the file path so we can change font sizes on the fly
    std::string filename;
};


} // namespace nom

#endif // NOMLIB_SDL_TTF_FONT_HEADERS defined
