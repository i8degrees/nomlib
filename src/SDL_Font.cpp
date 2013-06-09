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

  this->font = nullptr;
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

  if ( this->font != nullptr )
    TTF_CloseFont ( this->font );

  this->font = nullptr;

  TTF_Quit ();
}

const int32_t SDL_Font::getX ( void ) const
{
  return this->coords.getX();
}

const int32_t SDL_Font::getY ( void ) const
{
  return this->coords.getY();
}

const Coords& SDL_Font::getXY ( void ) const
{
  return this->coords;
}

void SDL_Font::setX ( int32_t x_ )
{
  this->coords.setX ( x_ );
}

void SDL_Font::setY ( int32_t y_ )
{
  this->coords.setY ( y_ );
}

void SDL_Font::setXY ( int32_t x_, int32_t y_ )
{
  this->coords.setXY ( x_, y_ );
}

int32_t SDL_Font::getTextWidth ( void )
{
  return this->coords.getWidth();
}

int32_t SDL_Font::getTextHeight ( void )
{
  return this->coords.getHeight();
}

const std::string& SDL_Font::getText ( void ) const
{
  return this->text_buffer;
}

void SDL_Font::setText ( std::string text )
{
  int32_t width, height = 0;

  if ( text.length() > 0 )
  {
    if ( TTF_SizeText ( this->font, text.c_str(), &width, &height ) != -1 )
    {
      this->coords.setDimensions ( width, height );
      this->text_buffer = text;
    }
  }

  if ( this->font != nullptr )
  {
    if ( this->getText().c_str() != nullptr )
      this->font_buffer.setCanvas ( TTF_RenderText_Solid  ( this->font, this->getText().c_str(),
                                                  this->text_color.getSDL_Color()
                                                ) );
  }
  else
  {
    #ifdef DEBUG_SDL_FONT
      std::cout << "ERR in SDL_Font::setText(): " << SDL_GetError() << std::endl;
    #endif
  }
}

const nom::Color& SDL_Font::getTextColor ( void ) const
{
  return this->text_color;
}

void SDL_Font::setTextColor ( const Color &color )
{
  this->text_color = color;
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

void SDL_Font::Draw ( void* video_buffer )
{
  if ( this->font_buffer.get() != nullptr )
  {
    this->font_buffer.setPosition ( this->coords );
    this->font_buffer.Draw ( video_buffer );
  }
}
