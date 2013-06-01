/******************************************************************************
    sprite.cpp

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

nom::Sprite::Sprite ( void )
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

nom::Sprite::Sprite ( unsigned int width, unsigned int height )
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

nom::Sprite::~Sprite ( void )
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

unsigned int nom::Sprite::getX ( void )
{
  return this->coords.x;
}

unsigned int nom::Sprite::getY ( void )
{
  return this->coords.y;
}

unsigned int nom::Sprite::getWidth ( void )
{
  return this->coords.width;
}

unsigned int nom::Sprite::getHeight ( void )
{
  return this->coords.height;
}

unsigned int nom::Sprite::getXOffset ( void )
{
  return this->offsets.x;
}

unsigned int nom::Sprite::getYOffset ( void )
{
  return this->offsets.y;
}

unsigned int nom::Sprite::getWidthOffset ( void )
{
  return this->offsets.width;
}

unsigned int nom::Sprite::getHeightOffset ( void )
{
  return this->offsets.height;
}

void nom::Sprite::setX ( unsigned int x )
{
  this->coords.x = x;
}

void nom::Sprite::setY ( unsigned int y )
{
  this->coords.y = y;
}

void nom::Sprite::setXY ( unsigned int x, unsigned int y )
{
  this->coords.x = x;
  this->coords.y = y;
}

void nom::Sprite::updateXY ( unsigned int x, unsigned int y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void nom::Sprite::setWidth ( unsigned int width )
{
  this->coords.width = width;
}

void nom::Sprite::setHeight ( unsigned int height )
{
  this->coords.height = height;
}

void nom::Sprite::setXOffset ( unsigned int x_offset )
{
  this->offsets.x = x_offset;
}

void nom::Sprite::setYOffset ( unsigned int y_offset )
{
  this->offsets.y = y_offset;
}

void nom::Sprite::setWidthOffset ( unsigned int width_offset )
{
  this->offsets.width = width_offset;
}

void nom::Sprite::setHeightOffset ( unsigned int height_offset )
{
  this->offsets.height = height_offset;
}

unsigned int nom::Sprite::getState ( void )
{
  return this->state;
}

void nom::Sprite::setState ( unsigned int state )
{
  this->state = state;
}

signed int nom::Sprite::getSheetID ( void )
{
  return this->sheet.id;
}

void nom::Sprite::setSheetID ( signed int id )
{
  this->sheet.id = id;
}

/*
struct sheet nom::Sprite::getSheetDimensions ( void )
{
  return this->sheet;
}
*/

void nom::Sprite::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->sheet.sprite_width = this->getWidth();
  this->sheet.sprite_height = this->getHeight();
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool nom::Sprite::Load ( std::string filename, GColor colorkey, unsigned int flags )
{
  this->sprite_buffer = Gfx::LoadImage ( filename, colorkey, flags );

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Load (): " << SDL_GetError() << std::endl;
    #endif
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;

    return false;
  }

  return true;
}

bool nom::Sprite::Draw ( SDL_Surface *video_buffer )
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
    offsets.x = this->getXOffset();
    offsets.y = this->getYOffset();
    offsets.w = this->getWidth();
    offsets.h = this->getHeight();
  }

  if ( Gfx::DrawSurface ( this->sprite_buffer, video_buffer, this->getX(), this->getY(), offsets.x, offsets.y, offsets.w, offsets.h ) == false )
  {
    return false;
  }

  return true;
}

/* FUTURE VER
bool nom::Sprite::Draw ( SDL_Surface *video_buffer ) // const
{
  GCoords coords, offsets; // temporary object to hold our coords

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Draw(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  coords.setXY ( this->getX(), this->getY() );

  if ( this->getSheetID() != -1 )
  {
    // FIXME: Presently, we assume every sprite on our sheet is on the same row
    offsets.setCoords ( this->sheet.id * this->sheet.sprite_width, 0, this->sheet.sprite_width, this->sheet.sprite_height );
  }
  else
  {
    offsets.setCoords ( this->getXOffset(), this->getYOffset(),
                        this->getWidth(),   this->getHeight()
                      );
  }

  if ( Gfx::DrawSurface ( this->sprite_buffer, video_buffer, coords, offsets ) == false )
  {
    return false;
  }

  return true;
}
*/
