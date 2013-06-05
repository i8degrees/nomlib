/******************************************************************************
    SDL_Font.cpp

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Font.h"

using namespace nom;

SDL_Font::SDL_Font ( void )
{
  #ifdef DEBUG_SDL_FONT_OBJ
    std::cout << "SDL_Font::SDL_Font (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->font = NULL;
  this->font_buffer = NULL;
  this->coords.setCoords ( 0, 0, 0, 0 );
  this->text_color.setColor ( 0, 0, 0 );
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

  if ( this->font != NULL )
    TTF_CloseFont ( this->font );

  this->font = NULL;

  if ( this->font_buffer != NULL )
    SDL_FreeSurface ( this->font_buffer );

  this->font_buffer = NULL;

  TTF_Quit ();
}

signed int SDL_Font::getTextWidth ( void )
{
  return this->coords.getWidth();
}

signed int SDL_Font::getTextHeight ( void )
{
  return this->coords.getHeight();
}

void SDL_Font::setText ( std::string text )
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
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR in SDL_Font::setText(): " << SDL_GetError() << std::endl;
    #endif
  }
}

bool SDL_Font::Load ( std::string filename, unsigned int font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == NULL )
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR: " << TTF_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool SDL_Font::Draw ( SDL_Surface *video_buffer )
{
  if ( this->font_buffer != NULL )
  {

    if ( Gfx::DrawSurface ( this->font_buffer, video_buffer, this->coords ) == false )
    {
      #ifdef DEBUG_SDL_FONT
        std::cout << "ERR in SDL_Font::Draw(): " << SDL_GetError() << std::endl;
      #endif
      return false;
    }
  }

  return true;
}
