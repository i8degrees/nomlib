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
  this->coords.setCoords ( 0, 0, 0, 0 );

  if ( TTF_Init () == -1 )
  {
    #ifdef DEBUG_SDL_TFONT
      std::cout << "ERR in SDL_TFont::SDL_TFont (): " << TTF_GetError() << std::endl;
    #endif
    exit ( EXIT_FAILURE ); // TODO: Reconsider
  }
}

SDL_TFont::~SDL_TFont ( void )
{
  #ifdef DEBUG_SDL_TFONT_OBJ
    std::cout << "SDL_TFont::~SDL_TFont (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->font != NULL )
  {
    TTF_CloseFont ( this->font );
    this->font = NULL;
  }

  TTF_Quit ();
}

unsigned int SDL_TFont::getTextWidth ( void )
{
  return this->coords.getW();
}

unsigned int SDL_TFont::getTextHeight ( void )
{
  return this->coords.getH();
}

std::string SDL_TFont::getTextBuffer ( void )
{
  return this->text_buffer;
}

void SDL_TFont::setTextBuffer ( std::string text )
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
}

SDL_Color SDL_TFont::getTextColor ( void )
{
  return this->text_color;
}

void SDL_TFont::setTextColor ( unsigned r, unsigned g, unsigned b )
{
  this->text_color.r = r;
  this->text_color.g = g;
  this->text_color.b = b;
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

bool SDL_TFont::Draw ( unsigned int x, unsigned int y )
{
  SDL_Surface *video_buffer = NULL;

  this->coords.setXY ( x, y );

  if ( this->getTextBuffer().c_str() != NULL )
    video_buffer = TTF_RenderText_Solid ( this->font, this->getTextBuffer().c_str(), this->text_color );
  else
  {
    std::cout << "ERR in SDL_TFont::Draw(): " << SDL_GetError() << std::endl;

    SDL_FreeSurface ( video_buffer );
    video_buffer = NULL;

    return false;
  }

  if ( video_buffer != NULL )
  {
    if ( Gfx::DrawSurface ( video_buffer, x, y ) == false )
    {
      std::cout << "ERR in SDL_TFont::Draw(): " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface ( video_buffer );
    video_buffer = NULL;

    return false;
  }

  SDL_FreeSurface ( video_buffer );
  video_buffer = NULL;

  return true;
}
