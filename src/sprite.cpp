/******************************************************************************
    sprite.cpp

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

Sprite::Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->sprite_buffer = NULL;

  this->coords.x = 0;
  this->coords.y = 0;
  this->coords.width = 0;
  this->coords.height = 0;
  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.width = 0;
  this->offsets.height = 0;
  this->state = 0;

  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = 0;
  this->sheet.height = 0;
  this->sheet.id = -1;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

Sprite::Sprite ( unsigned int width, unsigned int height )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->sprite_buffer = NULL;

  this->coords.x = 0;
  this->coords.y = 0;
  this->coords.width = width;
  this->coords.height = height;
  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.width = width;
  this->offsets.height = height;
  this->state = 0;

  this->sheet.id = -1;
  this->sheet.sprite_width = width;
  this->sheet.sprite_height = height;
  this->sheet.width = width;
  this->sheet.height = height;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

Sprite::~Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::~Sprite (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->sprite_buffer != NULL )
  {
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;
  }
}

unsigned int Sprite::GetX ( void )
{
  return this->coords.x;
}

unsigned int Sprite::GetY ( void )
{
  return this->coords.y;
}

unsigned int Sprite::GetWidth ( void )
{
  return this->coords.width;
}

unsigned int Sprite::GetHeight ( void )
{
  return this->coords.height;
}

unsigned int Sprite::GetXOffset ( void )
{
  return this->offsets.x;
}

unsigned int Sprite::GetYOffset ( void )
{
  return this->offsets.y;
}

unsigned int Sprite::GetWidthOffset ( void )
{
  return this->offsets.width;
}

unsigned int Sprite::GetHeightOffset ( void )
{
  return this->offsets.height;
}

void Sprite::SetX ( unsigned int x )
{
  this->coords.x = x;
}

void Sprite::SetY ( unsigned int y )
{
  this->coords.y = y;
}

void Sprite::SetXY ( unsigned int x, unsigned int y )
{
  this->coords.x = x;
  this->coords.y = y;
}

void Sprite::UpdateXY ( unsigned int x, unsigned int y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void Sprite::SetWidth ( unsigned int width )
{
  this->coords.width = width;
}

void Sprite::SetHeight ( unsigned int height )
{
  this->coords.height = height;
}

void Sprite::SetXOffset ( unsigned int x_offset )
{
  this->offsets.x = x_offset;
}

void Sprite::SetYOffset ( unsigned int y_offset )
{
  this->offsets.y = y_offset;
}

void Sprite::SetWidthOffset ( unsigned int width_offset )
{
  this->offsets.width = width_offset;
}

void Sprite::SetHeightOffset ( unsigned int height_offset )
{
  this->offsets.height = height_offset;
}

unsigned int Sprite::GetState ( void )
{
  return this->state;
}

void Sprite::SetState ( unsigned int state )
{
  this->state = state;
}

signed int Sprite::GetSheetID ( void )
{
  return this->sheet.id;
}

void Sprite::SetSheetID ( signed int id )
{
  this->sheet.id = id;
}

/*
struct sheet Sprite::GetSheetDimensions ( void )
{
  return this->sheet;
}
*/

void Sprite::SetSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->sheet.sprite_width = this->GetWidth();
  this->sheet.sprite_height = this->GetHeight();
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool Sprite::LoadImage ( std::string filename, SDL_Color colorkey, unsigned int flags )
{
  this->sprite_buffer = Gfx::LoadImage ( filename, colorkey, flags );

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::LoadImage (): " << SDL_GetError() << std::endl;
    #endif
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;

    return false;
  }

  return true;
}

bool Sprite::Draw ( Gfx *engine )
{
  SDL_Rect offsets; // temporary struct to hold our clipping coords (x, y, width, height)

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Draw(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( this->sheet.id != -1 )
  {
    // FIXME: Presently, we assume every sprite on our sheet is on the same row
    offsets.x = ( this->sheet.id * this->sheet.sprite_width );
    offsets.y = 0;

    offsets.w = this->sheet.sprite_width;
    offsets.h = this->sheet.sprite_height;
  }
  else
  {
    offsets.x = this->GetXOffset();
    offsets.y = this->GetYOffset();
    offsets.w = this->GetWidth();
    offsets.h = this->GetHeight();
  }

  if ( engine->DrawSurface ( this->sprite_buffer, this->GetX(), this->GetY(), &offsets ) == false )
  {
    return false;
  }

  return true;
}
