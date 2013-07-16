/******************************************************************************

    SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gfx/SDL_Font.hpp"

namespace nom {
  namespace priv {

void TTF_FreeSurface ( TTF_Font* font )
{
  TTF_CloseFont ( font );
}

  } // namespace priv
} // namespace nom


namespace nom {

SDL_Font::SDL_Font ( void )
{
NOMLIB_LOG_INFO;

  this->font = nullptr;

  this->coords = Coords ( 0, 0, 0, 0 );
  this->color = Color ( 0, 0, 0 );
  this->text_buffer = "\0";

  if ( TTF_Init () == -1 )
  {
NOMLIB_LOG_ERR ( TTF_GetError() );
  }
}

SDL_Font::~SDL_Font ( void )
{
NOMLIB_LOG_INFO;

  this->font.reset(); // nullptr

  TTF_Quit();
}

bool SDL_Font::SDL_Font::valid ( void ) const
{
  if ( this->font.get() != nullptr )
    return true;
  else
    return false;
}

int32_t SDL_Font::getTextWidth ( void )
{
  return this->coords.width;
}

int32_t SDL_Font::getTextHeight ( void )
{
  return this->coords.height;
}

const std::string& SDL_Font::getText ( void ) const
{
  return this->text_buffer;
}

void SDL_Font::setText ( const std::string& text )
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
NOMLIB_LOG_ERR ( "Text length must be greater than zero" );
  }
}

const Color& SDL_Font::getTextColor ( void ) const
{
  return this->color;
}

void SDL_Font::setTextColor ( const Color& color )
{
  this->color = color;
}

bool SDL_Font::Load ( std::string filename, uint32_t font_size )
{
  this->font = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), font_size ), nom::priv::TTF_FreeSurface );

  if ( this->valid() == false )
  {
NOMLIB_LOG_ERR ( "Could not load TTF file: " + filename );
    return false;
  }

  return true;
}

void SDL_Font::Update ( void )
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
  }
}

void SDL_Font::Draw ( void* video_buffer ) const
{
NOMLIB_ASSERT ( this->font_buffer.valid() );

  this->font_buffer.Draw ( video_buffer );
}


} // namespace nom
