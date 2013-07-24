/******************************************************************************

    SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gfx/TrueTypeFont.hpp"

namespace nom {
  namespace priv {

void TTF_FreeSurface ( TTF_Font* font )
{
  TTF_CloseFont ( font );
}

  } // namespace priv
} // namespace nom


namespace nom {

TrueTypeFont::TrueTypeFont ( void )
{
NOM_LOG_CLASSINFO;

  this->font = nullptr;
  this->coords = Coords ( 0, 0, 0, 0 );
  this->color = Color ( 0, 0, 0 );
  this->text_buffer = "\0";

  this->text_style = FontStyle::Regular; // default text styling effect
  this->style_options = 0;
  this->filename = "\0";

  if ( TTF_Init () == -1 )
  {
NOM_LOG_ERR ( TTF_GetError() );
  }
}

TrueTypeFont::~TrueTypeFont ( void )
{
NOM_LOG_CLASSINFO;

  this->font.reset(); // nullptr

  TTF_Quit();
}

bool TrueTypeFont::valid ( void ) const
{
  if ( this->font.get() != nullptr )
    return true;
  else
    return false;
}

const std::string& TrueTypeFont::getText ( void ) const
{
  return this->text_buffer;
}

int32 TrueTypeFont::getFontWidth ( void ) const
{
  return this->coords.width;
}

int32 TrueTypeFont::getFontHeight ( void ) const
{
  return this->coords.height;
}

FontStyle TrueTypeFont::getFontStyle ( void ) const
{
  return this->text_style;
}

void TrueTypeFont::setFontSize ( int32 point_size )
{
  if ( this->Load ( this->filename, nom::Color::Black, point_size ) == false )
  {
NOM_LOG_ERR ( "Could not set font size." );
  }
}

void TrueTypeFont::setFontStyle ( uint8 style, uint8 options )
{
  enum FontStyle current_style;

  switch ( style )
  {
    default:
    break;
    case FontStyle::Regular:
    case FontStyle::Bold:
    case FontStyle::Italic:
    case FontStyle::Underlined:
      // Do nothing stub
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case FontStyle::Faded:
    {
      this->text_style = FontStyle::Faded;
      this->style_options = options;
    break;
    }
  }
}

void TrueTypeFont::setText ( const std::string& text )
{
  int32_t width, height = 0; // holds the return values; width, height

  this->text_buffer = text;

  // Update the width & height of text string, if we can
  if ( text.length() > 0 )
  {
    if ( TTF_SizeText ( this->font.get(), text.c_str(), &width, &height ) != -1 )
      this->coords.setSize ( width, height );
  }
  else
  {
NOM_LOG_ERR ( "Text length must be greater than zero" );
  }
}

bool TrueTypeFont::Load ( const std::string& filename, const Color& colorkey,
                      int32 font_size, bool use_cache
                    )
{
  this->font.reset();
  this->font = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), font_size ), nom::priv::TTF_FreeSurface );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "Could not load TTF file: " + filename );
    return false;
  }

  this->filename = filename;

  return true;
}

void TrueTypeFont::Update ( void )
{
  // Update display coordinates
  this->font_buffer.setPosition ( this->coords );

  // Update the rendered text surface here for drawing
  if ( this->getText().c_str() != nullptr )
  {
    this->font_buffer.setCanvas ( TTF_RenderText_Solid
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    getSDL_Color ( this->color )
                                  )
                                );

    if ( this->text_style == FontStyle::Faded )
    {
      this->font_buffer.setAlpha ( this->style_options );
    }
  }
}

void TrueTypeFont::Draw ( void* video_buffer ) /*const*/
{
NOM_ASSERT ( this->font_buffer.valid() );

  if ( this->font_buffer.valid() && this->text_buffer.length() > 0 )
  {
    this->font_buffer.Draw ( video_buffer );
  }
}


} // namespace nom
