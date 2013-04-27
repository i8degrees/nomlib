/******************************************************************************
    font.cpp

    SDL-based Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "font.h"

Font::Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::Font (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->font = NULL;

  if ( TTF_Init () == -1 )
  {
    #ifdef DEBUG_FONT
      std::cout << "ERR in Font::Font (): " << TTF_GetError() << std::endl;
    #endif
    exit ( EXIT_FAILURE ); // TODO: Reconsider
  }

  this->text_color = { 0, 0, 0 };
  this->coords = { 0, 0, 0, 0 };
}

Font::~Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::~Font (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->font != NULL )
  {
    TTF_CloseFont ( this->font );
    this->font = NULL;
  }

  TTF_Quit ();
}

unsigned int Font::GetTextWidth ( void )
{
  return this->coords.w;
}

unsigned int Font::GetTextHeight ( void )
{
  return this->coords.h;
}

std::string Font::GetTextBuffer ( void )
{
  return this->text_buffer;
}

void Font::SetTextBuffer ( std::string text )
{
  signed int width, height = 0;

/*
TODO: Finish ERR checks:

  if ( text.length() > 0 )
*/
    if ( TTF_SizeText ( this->font, text.c_str(), &width, &height ) != -1 )
    {
      this->coords.w = width;
      this->coords.h = height;
      this->text_buffer = text;
    }
}

SDL_Color Font::GetTextColor ( void )
{
  return this->text_color;
}

void Font::SetTextColor ( unsigned r, unsigned g, unsigned b )
{
  this->text_color.r = r;
  this->text_color.g = g;
  this->text_color.b = b;
}

bool Font::LoadTTF ( std::string filename, unsigned int font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == NULL )
  {
    #ifdef DEBUG_FONT
      std::cout << "ERR: " << TTF_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Font::DrawText ( Gfx *engine, unsigned int x, unsigned int y )
{
  SDL_Surface *video_buffer = NULL;
  this->coords.x = x;
  this->coords.y = y;

  video_buffer = TTF_RenderText_Solid ( this->font, this->GetTextBuffer().c_str(), this->text_color );

  // TODO: ERR checking

  if ( engine->DrawSurface ( video_buffer, x, y ) == false )
  {
    std::cout << "ERR in Font::DrawText(): " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_FreeSurface ( video_buffer );
  video_buffer = NULL;

  return true;
}
