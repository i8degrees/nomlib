/******************************************************************************
    Font.cpp

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "font.h" // #include "Font.h"

std::string Font::getTextBuffer ( void ) const
{
  return text_buffer;
}

bool Font::Draw ( Gfx *engine, unsigned int x, unsigned int y ) const
{
  SDL_Surface *video_buffer = NULL;

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
    if ( engine->DrawSurface ( video_buffer, x, y ) == false )
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
