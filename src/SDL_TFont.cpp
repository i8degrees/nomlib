/******************************************************************************
    SDL_TFont.cpp

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_TFont.h"

SDL_TFont::SDL_TFont ( void )
{
  #ifdef DEBUG_SDL_TFONT_OBJ
    std::cout << "SDL_TFont::SDL_TFont (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->font = NULL;
  this->font_buffer = NULL;
  this->coords.setCoords ( 0, 0, 0, 0 );
  this->text_color.setColor ( 0, 0, 0 );
  this->text_buffer = "\0";

  if ( TTF_Init () == -1 )
  {
    #ifdef DEBUG_SDL_TFONT
      std::cout << "ERR in SDL_TFont::SDL_TFont (): " << TTF_GetError() << std::endl;
    #endif
  }
}

SDL_TFont::~SDL_TFont ( void )
{
  #ifdef DEBUG_SDL_TFONT_OBJ
    std::cout << "SDL_TFont::~SDL_TFont (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->font != NULL )
    TTF_CloseFont ( this->font );

  this->font = NULL;

  if ( this->font_buffer != NULL )
    SDL_FreeSurface ( this->font_buffer );

  this->font_buffer = NULL;

  TTF_Quit ();
}

signed int SDL_TFont::getTextWidth ( void )
{
  return this->coords.getWidth();
}

signed int SDL_TFont::getTextHeight ( void )
{
  return this->coords.getHeight();
}

void SDL_TFont::setText ( std::string text )
{
  signed int width, height = 0;

  if ( text.length() > 0 )
  {
    if ( TTF_SizeText ( this->font, text.c_str(), &width, &height ) != -1 )
    {
      this->coords.setDimensions ( width, height );
      this->text_buffer = text;
    }
  }

  if ( this->font != NULL )
  {
    if ( this->getText().c_str() != NULL )
      this->font_buffer = TTF_RenderText_Solid  ( this->font, this->getText().c_str(),
                                                  this->text_color.getSDL_Color()
                                                );
  }
  else
  {
    #ifdef DEBUG_SDL_TFONT
      std::cout << "ERR in SDL_TFont::setText(): " << SDL_GetError() << std::endl;
    #endif
  }
}



bool SDL_TFont::Load ( std::string filename, unsigned int font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == NULL )
  {
    #ifdef DEBUG_SDL_TFONT
      std::cout << "ERR: " << TTF_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool SDL_TFont::Draw ( SDL_Surface *video_buffer )
{
  if ( this->font_buffer != NULL )
  {
    // GCoords ( -1, -1, -1, -1 ) is equivalent to NULL for SDL_BlitSurface it seems
    if ( Gfx::DrawSurface ( this->font_buffer, video_buffer, this->coords, nom::Coords ( -1, -1, -1, -1 ) ) == false )
    {
      std::cout << "ERR in SDL_TFont::Draw(): " << SDL_GetError() << std::endl;
      return false;
    }
  }

  return true;
}
