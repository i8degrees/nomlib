/******************************************************************************

    SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Font.hpp"

using namespace nom;

SDL_Font::SDL_Font ( void )
{
  #ifdef DEBUG_SDL_FONT_OBJ
    std::cout << "SDL_Font::SDL_Font (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->font = nullptr;

  this->coords = nom::Coords ( 0, 0, 0, 0 );
  this->color = nom::Color ( 0, 0, 0 );
  this->text_buffer = "\0";

  if ( TTF_Init () == -1 )
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR in SDL_Font::SDL_Font (): " << TTF_GetError() << std::endl;
    #endif
  }
}

SDL_Font::~SDL_Font ( void )
{
  #ifdef DEBUG_SDL_FONT_OBJ
    std::cout << "SDL_Font::~SDL_Font (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->font != nullptr )
    TTF_CloseFont ( this->font );

  this->font = nullptr;

  TTF_Quit ();
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
    if ( TTF_SizeText ( this->font, text.c_str(), &width, &height ) != -1 )
      this->coords.setSize ( width, height );
  }
  else
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR in SDL_Font::setText(): " << TTF_GetError() << std::endl;
    #endif
  }
}

const nom::Color& SDL_Font::getTextColor ( void ) const
{
  return this->color;
}

void SDL_Font::setTextColor ( const nom::Color& color )
{
  this->color = color;
}

bool SDL_Font::Load ( std::string filename, uint32_t font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == nullptr )
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR: " << TTF_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void nom::SDL_Font::Update ( void )
{
  // Update display coordinates
  this->font_buffer.setPosition ( this->coords );

  // We must free the rendered font surface here in order to evade leaks
  if ( this->font_buffer.valid() )
    this->font_buffer.destroy();

  // Update the rendered text surface here for drawing
  if ( this->font != nullptr )
  {
    if ( this->getText().c_str() != nullptr )
      this->font_buffer.setCanvas ( TTF_RenderText_Solid
                                    (
                                      this->font,
                                      this->getText().c_str(),
                                      this->color.getSDL_Color()
                                    )
                                  );

  }
  else
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR in SDL_Font::setText(): " << SDL_GetError() << std::endl;
    #endif
  }
}

void SDL_Font::Draw ( void* video_buffer ) const
{
  if ( this->font_buffer.valid() )
    this->font_buffer.Draw ( video_buffer );
}
